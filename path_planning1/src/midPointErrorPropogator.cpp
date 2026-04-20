#include <rclcpp/rclcpp.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <std_msgs/msg/float32.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <nav_msgs/msg/odometry.hpp>

// Message Filters for synchronization
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

// TF2 for Quaternion to Euler (Yaw) conversion
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <mutex>
#include <vector>
#include <cmath>
#include <string>

using std::placeholders::_1;
using std::placeholders::_2;
using namespace std;

class midPointErrorPropogator : public rclcpp::Node {
public:
  midPointErrorPropogator(): Node("midPointErrorPropogator"){
    
    // Parameters
    this->declare_parameter<string>("topicOdom", "/odom");
    this->declare_parameter<string>("topicConePair", "/cone_pair");
    this->declare_parameter<string>("topicError", "/path_error");
    this->declare_parameter<float>("projectAheadDistance", 2.0);
    
    topicOdom_ = this->get_parameter("topicOdom").as_string();
    topicConePair_ = this->get_parameter("topicConePair").as_string();
    topicError_ = this->get_parameter("topicError").as_string();

    // Global variables
    followingPoint = make_pair(0.5, 0.0);
    lastPoint = make_pair(0.0, 0.0);

    // 1. Initialize Message Filter Subscribers
    conePair_sub_.subscribe(this, topicConePair_);
    odom_sub_.subscribe(this, topicOdom_);

    // 2. Initialize the Synchronizer cleanly
    uint32_t queue_size = 10;
    sync_ = std::make_shared<message_filters::Synchronizer<SyncPolicy>>(
        SyncPolicy(queue_size), conePair_sub_, odom_sub_);
    
    // 3. Register the synchronized callback
    sync_->registerCallback(std::bind(&midPointErrorPropogator::SyncCallback, this, _1, _2));

    // Publishers
    marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("/path", 10);
    error_pub_ = this->create_publisher<std_msgs::msg::Float32>(topicError_, 1);

    RCLCPP_INFO(this->get_logger(), "Error Propagator Node Initialized (With Odometry Sync)");
  }

private:
  string topicOdom_, topicConePair_, topicError_;
  float projectAheadDistance_;
  pair<float, float> followingPoint;
  pair<float, float> lastPoint;
  std::mutex path_mutex_;

  // Define the synchronization policy
  typedef message_filters::sync_policies::ApproximateTime<sdc_msgs::msg::Arrofarr, nav_msgs::msg::Odometry> SyncPolicy;

  // Subscribers and Synchronizer
  message_filters::Subscriber<sdc_msgs::msg::Arrofarr> conePair_sub_;
  message_filters::Subscriber<nav_msgs::msg::Odometry> odom_sub_;
  std::shared_ptr<message_filters::Synchronizer<SyncPolicy>> sync_;

  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr error_pub_;

  float signedDistancePointToLine(const pair<float, float>&A, const pair<float, float>&B, const pair<float, float>&P) {
    float dx = B.first - A.first;
    float dy = B.second - A.second;
    float numerator = (dx * (P.second - A.second)) - (dy * (P.first  - A.first));
    float denominator = std::sqrt((dx * dx) + (dy * dy));
    if (denominator == 0.0) return 0.0;
    return numerator / denominator;
  }

  void visualizeLineAB( const std::pair<float, float>& A, const std::pair<float, float>& B){
    static int line_id = 0;
    visualization_msgs::msg::Marker line;
    line.header.frame_id = "map";
    line.header.stamp = this->now();

    line.ns = "desired_lines";
    line.id = line_id++;   
    line.type = visualization_msgs::msg::Marker::LINE_STRIP;
    line.action = visualization_msgs::msg::Marker::ADD;

    line.scale.x = 0.05;
    line.color.g = 1.0;
    line.color.a = 1.0;
    line.lifetime = rclcpp::Duration::from_seconds(0);

    geometry_msgs::msg::Point p1, p2;
    p1.x = A.first; p1.y = A.second;
    p2.x = B.first; p2.y = B.second;
    line.points = {p1, p2};
    marker_pub_->publish(line);
  }

  void visualizeTrajectory(const std::pair<float, float>& P){
    static visualization_msgs::msg::Marker traj;

    // Initialize once
    if (traj.points.empty()) {
      traj.header.frame_id = "map";
      traj.ns = "trajectory";
      traj.id = 1;
      traj.type = visualization_msgs::msg::Marker::LINE_STRIP;
      traj.action = visualization_msgs::msg::Marker::ADD;

      traj.scale.x = 0.04;
      traj.color.r = 1.0f;
      traj.color.g = 0.0f;
      traj.color.b = 0.0f;
      traj.color.a = 1.0f;

      traj.lifetime = rclcpp::Duration::from_seconds(0);
    }
    if (traj.points.size() > 2000) traj.points.erase(traj.points.begin());

    traj.header.stamp = this->now();
    geometry_msgs::msg::Point pt;
    pt.x = P.first;
    pt.y = P.second;
    pt.z = 0.0;
    traj.points.push_back(pt);
    marker_pub_->publish(traj);
  }

  void SyncCallback(const sdc_msgs::msg::Arrofarr::ConstSharedPtr& conePair_msg,
                    const nav_msgs::msg::Odometry::ConstSharedPtr& odom_msg) {
                        
    if (conePair_msg->data.size() < 2 || conePair_msg->data[0].data.size() < 2 || conePair_msg->data[1].data.size() < 2) {
      RCLCPP_WARN(this->get_logger(), "Invalid cone pair message");
      return;
    }
    
    projectAheadDistance_ = this->get_parameter("projectAheadDistance").as_double();

    pair<float, float> blueCone, yellowCone;
    blueCone = make_pair(conePair_msg->data[0].data[0], conePair_msg->data[0].data[1]);
    yellowCone = make_pair(conePair_msg->data[1].data[0], conePair_msg->data[1].data[1]);
    
    std::lock_guard<std::mutex> lock(path_mutex_);
    lastPoint = followingPoint;
    followingPoint.first = (blueCone.first + yellowCone.first) / 2.0;
    followingPoint.second = (blueCone.second + yellowCone.second) / 2.0;
    
    visualizeLineAB(lastPoint, followingPoint);

    // --- REAL ODOMETRY VALUES ---
    float odom_x = odom_msg->pose.pose.position.x;
    float odom_y = odom_msg->pose.pose.position.y;

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
    
    // Calculate projected look-ahead point using real odometry
    float carX = odom_x + (projectAheadDistance_ * std::cos(carYaw));
    float carY = odom_y + (projectAheadDistance_ * std::sin(carYaw));
    // -------------------------------------------------

    visualizeTrajectory(make_pair(carX, carY));

    // Calculate and publish the cross-track error
    float error = signedDistancePointToLine(lastPoint, followingPoint, make_pair(carX, carY));
    
    auto error_msg = std_msgs::msg::Float32();
    error_msg.data = error;
    error_pub_->publish(error_msg);
    
    RCLCPP_INFO(this->get_logger(), "Calculated Path Error: %.3f", error);
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<midPointErrorPropogator>());
  rclcpp::shutdown();
  return 0;
}