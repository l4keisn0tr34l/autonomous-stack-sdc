#include <rclcpp/rclcpp.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

using std::placeholders::_1;
using namespace std;

class midPointColoredPathPlanningNode : public rclcpp::Node {
public:
    midPointColoredPathPlanningNode() : Node("midPointColored") {
        
        this->declare_parameter<string>("topicMapArr", "/map_arr");
        this->declare_parameter<string>("topicConePair", "/cone_pair");
        topicMapArr_ = this->get_parameter("topicMapArr").as_string();
        topicConePair_ = this->get_parameter("topicConePair").as_string();

        // global variables
        followingPoint = make_pair(-1000.0, -1000.0);

        // Standard Subscriber (No Synchronizer needed for testing without odom)
        mapArr_sub_ = this->create_subscription<sdc_msgs::msg::Arrofarr>(
            topicMapArr_, 10, std::bind(&midPointColoredPathPlanningNode::MapCallback, this, _1));

        conePair_pub_ = this->create_publisher<sdc_msgs::msg::Arrofarr>(topicConePair_, 1);
        
        RCLCPP_INFO(this->get_logger(), "Midpoint Colored Path Planning Node Initialized (Testing Mode: No Odom)");
    }

private:
    string topicConePair_, topicMapArr_;
    sdc_msgs::msg::Arrofarr conePair_msg;
    pair<float, float> followingPoint;

    rclcpp::Subscription<sdc_msgs::msg::Arrofarr>::SharedPtr mapArr_sub_;
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

    void MapCallback(const sdc_msgs::msg::Arrofarr::SharedPtr mapArr_msg) {
        
        // --- TESTING MODE: Assumed Stationary Odometry ---
        float carX = 0.0;
        float carY = 0.0;
        float carYaw = 0.0;
        // -------------------------------------------------

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
        if (blueFound && yellowFound /* && !isInFront(carX, carY, carYaw, followingPoint.first, followingPoint.second)*/ ) {
            
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