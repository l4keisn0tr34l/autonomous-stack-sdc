#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>
using std::placeholders::_1;
using namespace std;

class pidControlNode : public rclcpp::Node{
public:
  pidControlNode(): Node("pid"){
    this->declare_parameter<string>("topicError", "/path_error");
    this->declare_parameter<string>("topicControl", "/cmd");
    this->declare_parameter<float>("kp", 1.5);
    this->declare_parameter<float>("kd", 0.1);
    this->declare_parameter<float>("ki", 0.1);
    this->declare_parameter<float>("velocity", 0.4);
    this->declare_parameter<float>("maxError", 6.0);
    this->declare_parameter<float>("maxSteeringAngle", 0.5);
    this->declare_parameter<float>("minInterval", 0.01);

    topicError_ = this->get_parameter("topicError").as_string();
    topicControl_ = this->get_parameter("topicControl").as_string();
    kp_ = this->get_parameter("kp").as_double();
    kd_ = this->get_parameter("kd").as_double();
    ki_ = this->get_parameter("ki").as_double();
    velocity_ = this->get_parameter("velocity").as_double();
    maxError_ = this->get_parameter("maxError").as_double();
    maxSteeringAngle_ = this->get_parameter("maxSteeringAngle").as_double();
    minInterval_ = this->get_parameter("minInterval").as_double();

    error_sub_ = this->create_subscription<std_msgs::msg::Float32>(topicError_, 1, bind(&pidControlNode::errorCallback, this, _1));
    control_pub_ = this->create_publisher<ackermann_msgs::msg::AckermannDriveStamped>(topicControl_, 1);
  }
  
private:
  string topicError_, topicControl_;
  float kp_, ki_, kd_, minInterval_, velocity_, maxError_, maxSteeringAngle_;
  float previous_error_ = 0.0;
  float integral_ = 0.0;
  bool firstMsg = true;
  rclcpp::Time prevTime;

  //publishers and subscribers
  rclcpp::Publisher<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr control_pub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr error_sub_;

  float clamp(float v, float lo, float hi) {
    return std::max(lo, std::min(v, hi));
  }

  void errorCallback(const std_msgs::msg::Float32::SharedPtr error_msg){
    kp_ = this->get_parameter("kp").as_double(); kd_ = this->get_parameter("kd").as_double(); ki_ = this->get_parameter("ki").as_double(); velocity_ = this->get_parameter("velocity").as_double();
    if(firstMsg){ previous_error_ = error_msg->data; firstMsg = false; prevTime = this->get_clock()->now(); return;}
    rclcpp::Time currentTime = this->get_clock()->now();
    float dt = (currentTime - prevTime).seconds();
    if(dt<minInterval_) return;
    
    float error = error_msg->data;
    error = error/maxError_;
    error = clamp(error, -1.0, 1.0);

    integral_ += (error * dt);
    integral_ = clamp(integral_, -1.0, 1.0);

    float derivative = (error - previous_error_)/dt;

    float control_signal = -((kp_ * error) + (ki_ * integral_) + (kd_ * derivative));
    control_signal = clamp(control_signal, -maxSteeringAngle_, maxSteeringAngle_);
    cout << "Error: " << error << " Control Signal: " << control_signal << endl;
    
    auto control_msg = ackermann_msgs::msg::AckermannDriveStamped();
    control_msg.header.stamp = currentTime;
    control_msg.header.frame_id = "base_link";
    control_msg.drive.steering_angle = control_signal;
    control_msg.drive.speed = velocity_; //constant speed
    control_pub_->publish(control_msg);

    previous_error_ = error;
    prevTime = currentTime;
    
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<pidControlNode>());
  rclcpp::shutdown();
  return 0;
}