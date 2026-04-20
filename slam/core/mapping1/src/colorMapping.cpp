#include <rclcpp/rclcpp.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <nav_msgs/msg/odometry.hpp>

// QoS Profiles
#include <rmw/qos_profiles.h>

// Message Filters for synchronization
#include <message_filters/subscriber.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/synchronizer.h>

// TF2 for Quaternion to Euler (Yaw) conversion
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <vector>
#include <cmath>
#include <string>

using std::placeholders::_1;
using std::placeholders::_2;

class RealLifeMappingNode : public rclcpp::Node {
public:
    RealLifeMappingNode() : Node("real_life_mapping") {
        // Parameters
        this->declare_parameter<std::string>("topicClusters", "/mapping_input");
        this->declare_parameter<std::string>("topicOdom", "/odom");
        this->declare_parameter<std::string>("topicMapArr", "/map_arr");
        this->declare_parameter<std::string>("topicMapVis", "/map_vis");
        this->declare_parameter<double>("matchDistanceThresh", 1.5);
        this->declare_parameter<double>("coneDistanceThresh", 15.0);

        topicClusters_ = this->get_parameter("topicClusters").as_string();
        topicOdom_ = this->get_parameter("topicOdom").as_string();
        topicMapArr_ = this->get_parameter("topicMapArr").as_string();
        topicMapVis_ = this->get_parameter("topicMapVis").as_string();

        firstMsg = true;

        // 1. Initialize Message Filter Subscribers with SensorData QoS
        // This is critical for listening to topics like /odom and LiDAR/Camera outputs
        rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;
        
        clusters_sub_.subscribe(this, topicClusters_, qos_profile);
        odom_sub_.subscribe(this, topicOdom_, qos_profile);

        // 2. Initialize the Synchronizer with an ApproximateTime policy
        // Increased queue size to 100 to handle different publish rates (e.g., fast odom vs slow clusters)
        sync_ = std::make_shared<message_filters::Synchronizer<SyncPolicy>>(
            SyncPolicy(100), clusters_sub_, odom_sub_);
        
        // 3. Register the callback that takes BOTH synchronized messages
        sync_->registerCallback(std::bind(&RealLifeMappingNode::SyncCallback, this, _1, _2));

        // Publishers
        mapArr_pub_ = this->create_publisher<sdc_msgs::msg::Arrofarr>(topicMapArr_, 1);
        mapVis_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(topicMapVis_, 1);
        
        RCLCPP_INFO(this->get_logger(), "Real-life Mapping Node Initialized (With Odometry Sync)");
    }

private:
    std::string topicClusters_, topicOdom_, topicMapArr_, topicMapVis_;
    sdc_msgs::msg::Arrofarr mapArr;
    bool firstMsg;

    // Define the synchronization policy
    typedef message_filters::sync_policies::ApproximateTime<sdc_msgs::msg::Arrofarr, nav_msgs::msg::Odometry> SyncPolicy;
    
    // Message Filter Subscribers and Synchronizer
    message_filters::Subscriber<sdc_msgs::msg::Arrofarr> clusters_sub_;
    message_filters::Subscriber<nav_msgs::msg::Odometry> odom_sub_;
    std::shared_ptr<message_filters::Synchronizer<SyncPolicy>> sync_;

    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr mapVis_pub_;
    rclcpp::Publisher<sdc_msgs::msg::Arrofarr>::SharedPtr mapArr_pub_;

    // Updated Callback to receive both synchronized messages
    void SyncCallback(const sdc_msgs::msg::Arrofarr::ConstSharedPtr& clusters_msg,
                      const nav_msgs::msg::Odometry::ConstSharedPtr& odom_msg) {
        
        double matchThresh = this->get_parameter("matchDistanceThresh").as_double();
        double coneThresh = this->get_parameter("coneDistanceThresh").as_double();

        // --- REAL ODOMETRY VALUES ---
        double odom_x = odom_msg->pose.pose.position.x;
        double odom_y = odom_msg->pose.pose.position.y;

        // Convert Quaternion to Euler to get Yaw
        tf2::Quaternion q(
            odom_msg->pose.pose.orientation.x,
            odom_msg->pose.pose.orientation.y,
            odom_msg->pose.pose.orientation.z,
            odom_msg->pose.pose.orientation.w
        );
        tf2::Matrix3x3 m(q);
        double roll, pitch, yaw;
        m.getRPY(roll, pitch, yaw);
        // -----------------------------

        // Upgraded logging to standard ROS 2 logger
        RCLCPP_INFO(this->get_logger(), "Sync Triggered: odom_x=%.2f, odom_y=%.2f, clusters_count=%zu", 
                    odom_x, odom_y, clusters_msg->data.size());

        for (const auto &cone_data : clusters_msg->data) {
            // Check message integrity: expect [x, y, color]
            if (cone_data.data.size() < 3) continue;

            float lx = cone_data.data[0]; 
            float ly = cone_data.data[1]; 
            float color = cone_data.data[2];

            // Filter by distance (Ego-relative)
            float distSq = (lx * lx) + (ly * ly);
            if (distSq > (coneThresh * coneThresh)) continue;

            // Coordinate Transformation: Local to Global Map
            float gx = (lx * std::cos(yaw)) - (ly * std::sin(yaw)) + odom_x;
            float gy = (lx * std::sin(yaw)) + (ly * std::cos(yaw)) + odom_y;

            if (firstMsg) {
                sdc_msgs::msg::Arr newPoint;
                newPoint.data = {gx, gy, color};
                mapArr.data.push_back(newPoint);
                continue;
            }

            // Simple Nearest Neighbor Search in existing map
            float minDistSq = 1e6;
            int closestIdx = -1;

            for (size_t i = 0; i < mapArr.data.size(); ++i) {
                float dx = gx - mapArr.data[i].data[0];
                float dy = gy - mapArr.data[i].data[1];
                float dSq = (dx * dx) + (dy * dy);
                if (dSq < minDistSq) {
                    minDistSq = dSq;
                    closestIdx = i;
                }
            }

            // Erase on color conflict
            if (closestIdx != -1 && minDistSq <= (matchThresh * matchThresh)) {
                if (mapArr.data[closestIdx].data[2] == color || color == 4.0) {
                    continue; // Colors match, or new detection is unknown. Ignore new detection.
                } else {
                    // Conflicting color detected, erase the old one.
                    mapArr.data.erase(mapArr.data.begin() + closestIdx);
                }
            }

            // Add the new detection
            sdc_msgs::msg::Arr newPoint;
            newPoint.data = {gx, gy, color};
            mapArr.data.push_back(newPoint);
        }

        firstMsg = false;

        // Publish Global Map Array
        mapArr.header.stamp = this->get_clock()->now();
        mapArr.header.frame_id = "map";
        mapArr_pub_->publish(mapArr);
        
        visualise();
    }

    void visualise() {
        visualization_msgs::msg::MarkerArray marker_array;
        
        // 1. The Delete Marker
        visualization_msgs::msg::Marker delete_marker;
        delete_marker.action = visualization_msgs::msg::Marker::DELETEALL;
        marker_array.markers.push_back(delete_marker);

        // 2. Start cone IDs at 1
        int id = 1; 
        
        for (const auto &cone : mapArr.data) {
            visualization_msgs::msg::Marker m;
            m.header.frame_id = "map";
            m.header.stamp = this->get_clock()->now();
            
            m.ns = "cones";   
            m.id = id++;      
            
            m.type = visualization_msgs::msg::Marker::CYLINDER;
            m.action = visualization_msgs::msg::Marker::ADD;
            m.pose.position.x = cone.data[0];
            m.pose.position.y = cone.data[1];
            
            // Physical properties
            m.scale.x = 0.25; m.scale.y = 0.25;
            m.scale.z = (cone.data[2] == 3.0) ? 0.8 : 0.4; 

            // Color Logic (0=Blue, 1=Yellow)
            m.color.a = 1.0;
            if (cone.data[2] == 0.0) { // Blue
                m.color.r = 0.0; m.color.g = 0.0; m.color.b = 1.0;
            } else if (cone.data[2] == 1.0) { // Yellow
                m.color.r = 1.0; m.color.g = 1.0; m.color.b = 0.0;
            }

            marker_array.markers.push_back(m);
        }
        mapVis_pub_->publish(marker_array);
    }
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RealLifeMappingNode>());
    rclcpp::shutdown();
    return 0;
}