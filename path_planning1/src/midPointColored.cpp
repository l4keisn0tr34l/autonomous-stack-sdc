#include <rclcpp/rclcpp.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <nav_msgs/msg/odometry.hpp>

// Message Filters for synchronization
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

// TF2 for Quaternion to Euler (Yaw) conversion
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

using std::placeholders::_1;
using std::placeholders::_2;
using namespace std;

class midPointColoredPathPlanningNode : public rclcpp::Node {
public:
    midPointColoredPathPlanningNode() : Node("midPointColored") {
        
        // Parameters
        this->declare_parameter<string>("topicOdom", "/odom");
        this->declare_parameter<string>("topicMapArr", "/map_arr");
        this->declare_parameter<string>("topicConePair", "/cone_pair");
        
        topicOdom_ = this->get_parameter("topicOdom").as_string();
        topicMapArr_ = this->get_parameter("topicMapArr").as_string();
        topicConePair_ = this->get_parameter("topicConePair").as_string();

        // Global variables
        followingPoint = make_pair(-1000.0, -1000.0);

        // 1. Initialize Message Filter Subscribers
        mapArr_sub_.subscribe(this, topicMapArr_);
        odom_sub_.subscribe(this, topicOdom_);

        // 2. Initialize the Synchronizer cleanly
        uint32_t queue_size = 10;
        sync_ = std::make_shared<message_filters::Synchronizer<SyncPolicy>>(
            SyncPolicy(queue_size), mapArr_sub_, odom_sub_);
        
        // 3. Register the callback
        sync_->registerCallback(std::bind(&midPointColoredPathPlanningNode::SyncCallback, this, _1, _2));

        // Publisher
        conePair_pub_ = this->create_publisher<sdc_msgs::msg::Arrofarr>(topicConePair_, 1);
        
        RCLCPP_INFO(this->get_logger(), "Midpoint Colored Path Planning Node Initialized (With Odometry Sync)");
    }

private:
    string topicOdom_, topicMapArr_, topicConePair_;
    sdc_msgs::msg::Arrofarr conePair_msg;
    pair<float, float> followingPoint;

    // Define the synchronization policy
    typedef message_filters::sync_policies::ApproximateTime<sdc_msgs::msg::Arrofarr, nav_msgs::msg::Odometry> SyncPolicy;

    // Subscribers and Synchronizer
    message_filters::Subscriber<sdc_msgs::msg::Arrofarr> mapArr_sub_;
    message_filters::Subscriber<nav_msgs::msg::Odometry> odom_sub_;
    std::shared_ptr<message_filters::Synchronizer<SyncPolicy>> sync_;

    rclcpp::Publisher<sdc_msgs::msg::Arrofarr>::SharedPtr conePair_pub_;

    bool isInFront(float xc, float yc, float yaw, float x, float y) {
        float hx = cos(yaw); 
        float hy = sin(yaw);
        float dx = x - xc; 
        float dy = y - yc;
        float s = (hx * dx) + (hy * dy);
        return s > 0.0;
    }

    inline float getDistanceSquared(float x1, float y1, float x2, float y2) {
        return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
    }

    // Updated Callback to receive both synchronized messages
    void SyncCallback(const sdc_msgs::msg::Arrofarr::ConstSharedPtr& mapArr_msg,
                      const nav_msgs::msg::Odometry::ConstSharedPtr& odom_msg) {
        
        // --- REAL ODOMETRY VALUES ---
        float carX = odom_msg->pose.pose.position.x;
        float carY = odom_msg->pose.pose.position.y;

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
        float carYaw = yaw;
        // -----------------------------

        sdc_msgs::msg::Arr selectedBlueCone;
        sdc_msgs::msg::Arr selectedYellowCone;

        // sort cones by distance from car
        auto distCompare = [&](const vector<float>& a, const vector<float>& b) {
            float adist_ = (a[0] - carX) * (a[0] - carX) + (a[1] - carY) * (a[1] - carY);
            float bdist_ = (b[0] - carX) * (b[0] - carX) + (b[1] - carY) * (b[1] - carY);
            return adist_ < bdist_;
        };
        
        vector<vector<float>> sortedMapArr;
        for (auto i : mapArr_msg->data) {
            sortedMapArr.push_back({i.data[0], i.data[1], i.data[2]});
        }
        sort(sortedMapArr.begin(), sortedMapArr.end(), distCompare);

        // find closest pair of blue and yellow cones
        bool blueFound = false;
        bool yellowFound = false;
        
        for (auto i : sortedMapArr) {
            if (i[2] == 0.0 && !blueFound) { // blue cone
                if (!isInFront(carX, carY, carYaw, i[0], i[1])) continue;
                selectedBlueCone.data = {i[0], i[1], i[2]};
                blueFound = true;
            }
            else if (i[2] == 1.0 && !yellowFound) { // yellow cone
                if (!isInFront(carX, carY, carYaw, i[0], i[1])) continue;
                selectedYellowCone.data = {i[0], i[1], i[2]};
                yellowFound = true;
            }
            if (blueFound && yellowFound) break;
        }

        // Trigger logic to publish new cone pair
        if (blueFound && yellowFound) {
            
            followingPoint.first = (selectedBlueCone.data[0] + selectedYellowCone.data[0]) / 2.0;
            followingPoint.second = (selectedBlueCone.data[1] + selectedYellowCone.data[1]) / 2.0;
            
            RCLCPP_INFO(this->get_logger(), "Following Point: (%.2f, %.2f)", followingPoint.first, followingPoint.second);
            
            conePair_msg.data.clear();
            conePair_msg.data.push_back(selectedBlueCone);
            conePair_msg.data.push_back(selectedYellowCone);
            
            conePair_msg.header.stamp = this->get_clock()->now();
            conePair_msg.header.frame_id = "map";
            
            conePair_pub_->publish(conePair_msg);
        }
    }
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<midPointColoredPathPlanningNode>());
    rclcpp::shutdown();
    return 0;
}