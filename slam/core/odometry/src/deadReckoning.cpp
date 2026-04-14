#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include "message_filters/subscriber.h"
#include "message_filters/synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_ros/transform_broadcaster.h>
#include <chrono>

using std::placeholders::_1;
using std::placeholders::_2;
using namespace std;

class DeadReckoningNode : public rclcpp::Node{
public:
  DeadReckoningNode(): Node("deadReckoning"){
    this->declare_parameter<string>("topicOdom", "/odom");
    this->declare_parameter<string>("topicVel", "/mc/vel");
    this->declare_parameter<string>("topicIMU", "/oak/imu/data");
    this->declare_parameter<double>("minTimeInterval", 0.1);

    topicOdom_ = this->get_parameter("topicOdom").as_string();
    topicVel_ = this->get_parameter("topicVel").as_string();
    topicIMU_ = this->get_parameter("topicIMU").as_string();
    

    firstMsg = true; xPos = 0.0; yPos = 0.0;

    odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>(topicOdom_, 1);
    vel_sub_.subscribe(this, topicVel_);
    imu_sub_.subscribe(this, topicIMU_);
    uint32_t queue_size = 10;
    sync = std::make_shared<message_filters::Synchronizer<message_filters::sync_policies::
           ApproximateTime<geometry_msgs::msg::TwistStamped, sensor_msgs::msg::Imu>>>(
           message_filters::sync_policies::ApproximateTime<geometry_msgs::msg::TwistStamped,
           sensor_msgs::msg::Imu>(queue_size), vel_sub_, imu_sub_);
    sync->setAgePenalty(1.00);
    sync->registerCallback(std::bind(&DeadReckoningNode::SyncCallback, this, _1, _2));
    tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
    
  }

private:
  //parameters and global variables
  string topicOdom_, topicVel_, topicIMU_;
  double minTimeInterval_;

  bool firstMsg;
  double xPos, yPos, yaw, initialYaw;
  geometry_msgs::msg::Twist lastVel_;
  rclcpp::Time lastTime_;

  //messages
  nav_msgs::msg::Odometry odom_msg;
  geometry_msgs::msg::TransformStamped transformStamped;

  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
  message_filters::Subscriber<geometry_msgs::msg::TwistStamped> vel_sub_;
  message_filters::Subscriber<sensor_msgs::msg::Imu> imu_sub_;
  std::shared_ptr<message_filters::Synchronizer<message_filters::sync_policies::ApproximateTime<geometry_msgs::msg::TwistStamped, sensor_msgs::msg::Imu>>> sync;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  void SyncCallback(const geometry_msgs::msg::TwistStamped::ConstSharedPtr & vel_msg, const sensor_msgs::msg::Imu::ConstSharedPtr & imu_msg){
    
    //extracting yaw from IMU
    tf2::Quaternion q(imu_msg->orientation.x, imu_msg->orientation.y, imu_msg->orientation.z, imu_msg->orientation.w); tf2::Matrix3x3 m(q);
    double ir, ip, iy;
    m.getRPY(ir, ip, iy);

    //initialise current time
    rclcpp::Time currentTime_ = this->now();
    minTimeInterval_ = this->get_parameter("minTimeInterval").as_double();

    //ignore the first message to initialize lastVel and lastTime
    if(firstMsg){firstMsg = false; lastVel_ = vel_msg->twist; initialYaw=iy; lastTime_ = currentTime_; return;}

    //compute acceleration
    double dt = (currentTime_ - lastTime_).seconds(); if(dt < minTimeInterval_) return;
    double ax = (vel_msg->twist.linear.x - lastVel_.linear.x)/dt;
    double ay = (vel_msg->twist.linear.y - lastVel_.linear.y)/dt;

    //update position using last velocity and acceleration
    xPos += (lastVel_.linear.x * dt) + (0.5 * ax * dt * dt);
    yPos += (lastVel_.linear.y * dt) + (0.5 * ay * dt * dt);
    yaw = iy - initialYaw;
    yaw = atan2(sin(yaw), cos(yaw)); //normalize yaw to [-pi, pi]

    //loop closure
    lastVel_ = vel_msg->twist;
    lastTime_ = currentTime_;

    //publish odometry message
    odom_msg.header.stamp = currentTime_;
    odom_msg.header.frame_id = "odom";
    odom_msg.child_frame_id = "base_footprint";
    odom_msg.pose.pose.position.x = xPos;
    odom_msg.pose.pose.position.y = yPos;
    odom_msg.pose.pose.position.z = 0.0;
    tf2::Quaternion odom_quat;
    odom_quat.setRPY(0, 0, yaw);
    odom_msg.pose.pose.orientation.x = odom_quat.x();
    odom_msg.pose.pose.orientation.y = odom_quat.y();
    odom_msg.pose.pose.orientation.z = odom_quat.z();
    odom_msg.pose.pose.orientation.w = odom_quat.w();
    odom_msg.twist.twist = vel_msg->twist;
    odom_pub_->publish(odom_msg);

    //publish transform for rviz visualization
    transformStamped.header.stamp = currentTime_;
    transformStamped.header.frame_id = "odom";
    transformStamped.child_frame_id = "base_footprint";
    transformStamped.transform.translation.x = xPos;
    transformStamped.transform.translation.y = yPos;
    transformStamped.transform.translation.z = 0.0;
    transformStamped.transform.rotation.x = odom_quat.x();
    transformStamped.transform.rotation.y = odom_quat.y();
    transformStamped.transform.rotation.z = odom_quat.z();
    transformStamped.transform.rotation.w = odom_quat.w();
    tf_broadcaster_->sendTransform(transformStamped);

  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DeadReckoningNode>());
  rclcpp::shutdown();
  return 0;
}
