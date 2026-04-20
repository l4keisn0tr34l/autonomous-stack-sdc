#include <rclcpp/rclcpp.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <vector>
#include <cmath>

using std::placeholders::_1;

class RealLifeMappingNode : public rclcpp::Node {
public:
    RealLifeMappingNode() : Node("real_life_mapping") {
        // Parameters
        this->declare_parameter<std::string>("topicClusters", "/mapping_input");
        this->declare_parameter<std::string>("topicMapArr", "/map_arr");
        this->declare_parameter<std::string>("topicMapVis", "/map_vis");
        this->declare_parameter<double>("matchDistanceThresh", 1.5);
        this->declare_parameter<double>("coneDistanceThresh", 15.0);

        topicClusters_ = this->get_parameter("topicClusters").as_string();
        topicMapArr_ = this->get_parameter("topicMapArr").as_string();
        topicMapVis_ = this->get_parameter("topicMapVis").as_string();

        firstMsg = true;

        // Standard ROS 2 Subscription (No Synchronizer needed for testing)
        clusters_sub_ = this->create_subscription<sdc_msgs::msg::Arrofarr>(
            topicClusters_, 10, std::bind(&RealLifeMappingNode::SyncCallback, this, _1));

        // Publishers
        mapArr_pub_ = this->create_publisher<sdc_msgs::msg::Arrofarr>(topicMapArr_, 1);
        mapVis_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(topicMapVis_, 1);
        
        RCLCPP_INFO(this->get_logger(), "Real-life Mapping Node Initialized (Testing Mode: No Odom)");
    }

private:
    std::string topicClusters_, topicMapArr_, topicMapVis_;
    sdc_msgs::msg::Arrofarr mapArr;
    bool firstMsg;

    rclcpp::Subscription<sdc_msgs::msg::Arrofarr>::SharedPtr clusters_sub_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr mapVis_pub_;
    rclcpp::Publisher<sdc_msgs::msg::Arrofarr>::SharedPtr mapArr_pub_;

    void SyncCallback(const sdc_msgs::msg::Arrofarr::SharedPtr clusters_msg) {
        
        double matchThresh = this->get_parameter("matchDistanceThresh").as_double();
        double coneThresh = this->get_parameter("coneDistanceThresh").as_double();

        // --- TESTING MODE: Assumed Odometry Values ---
        // When you are ready to use real odometry, replace these with the data from odom_msg
        double odom_x = 0.0;
        double odom_y = 0.0;
        double yaw = 0.0;
        // ---------------------------------------------

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

            // Original logic restored: Erase on color conflict
            if (closestIdx != -1 && minDistSq <= (matchThresh * matchThresh)) {
                if (mapArr.data[closestIdx].data[2] == color || color == 4.0) {
                    continue; // Colors match, or new detection is unknown. Ignore new detection.
                } else {
                    // Conflicting color detected, erase the old one.
                    mapArr.data.erase(mapArr.data.begin() + closestIdx);
                }
            }

            // Add the new detection (applies to brand new cones, OR cones that just had their old versions erased)
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

        // 2. Start cone IDs at 1 to avoid conflicting with the delete marker (which is 0)
        int id = 1; 
        
        for (const auto &cone : mapArr.data) {
            visualization_msgs::msg::Marker m;
            m.header.frame_id = "map";
            m.header.stamp = this->get_clock()->now();
            
            m.ns = "cones";   // <-- Added namespace
            m.id = id++;      // <-- Now starts at 1, 2, 3...
            
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