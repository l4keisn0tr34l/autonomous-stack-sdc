#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>

class ConeLineVisualizer : public rclcpp::Node
{
public:
    ConeLineVisualizer() : Node("cone_line_visualizer")
    {
        sub_ = this->create_subscription<sdc_msgs::msg::Arrofarr>(
            "/cone_pair", 10,
            std::bind(&ConeLineVisualizer::callback, this, std::placeholders::_1));

        marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>(
            "/cone_line_marker", 10);
    }

private:
    void callback(const sdc_msgs::msg::Arrofarr::SharedPtr msg)
    {
        // if (msg->data.size() < 2 ||
        //     msg->data[0].data.size() < 3 ||
        //     msg->data[1].data.size() < 3)
        // {
        //     RCLCPP_WARN(this->get_logger(), "Invalid /cone_pair message");
        //     return;
        // }

        geometry_msgs::msg::Point p1, p2;
        p1.x = msg->data[0].data[0];
        p1.y = msg->data[0].data[1];
        p1.z = msg->data[0].data[2];

        p2.x = msg->data[1].data[0];
        p2.y = msg->data[1].data[1];
        p2.z = msg->data[1].data[2];

        visualization_msgs::msg::Marker line_marker;
        line_marker.header.frame_id = "map";
        line_marker.header.stamp = this->get_clock()->now();
        line_marker.ns = "cone_line";
        line_marker.id = 0;
        line_marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
        line_marker.action = visualization_msgs::msg::Marker::ADD;
        line_marker.scale.x = 0.05;  // line thickness

        line_marker.color.r = 1.0;
        line_marker.color.g = 0.5;
        line_marker.color.b = 0.0;
        line_marker.color.a = 1.0;

        line_marker.points.push_back(p1);
        line_marker.points.push_back(p2);

        marker_pub_->publish(line_marker);
    }

    rclcpp::Subscription<sdc_msgs::msg::Arrofarr>::SharedPtr sub_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ConeLineVisualizer>());
    rclcpp::shutdown();
    return 0;
}
