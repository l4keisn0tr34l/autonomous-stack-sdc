#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/filter.h>
#include <tbb/parallel_for.h>
#include <tbb/spin_mutex.h>
#include <tbb/combinable.h>
using namespace std;

class GroundRemovalNodeSimpleZ : public rclcpp::Node{
public:
  GroundRemovalNodeSimpleZ(): Node("simpleZ"){
        
    this->declare_parameter<string>("topicNonGround", "/nonground");
    this->declare_parameter<float>("heightLidar", 0.7);
    this->declare_parameter<float>("leftRightRotation", 0.0);
    this->declare_parameter<float>("frontBackRotation", -0.086);
    this->declare_parameter<string>("frameLidar", "robosense");
    this->declare_parameter<string>("topicLidar", "/rslidar_points");
    this->declare_parameter<bool>("applyLimit", false);
    this->declare_parameter<float>("leftLimit", 5.0);
    this->declare_parameter<float>("rightLimit", 5.0);     
    this->declare_parameter<float>("topLimit", 2.0);
    this->declare_parameter<float>("distanceLimit", 20.0); 
    this->declare_parameter<float>("verticalInclinationFactor", 0.00);

        
    frameLidar_ = this->get_parameter("frameLidar").as_string();
    topicLidar_ = this->get_parameter("topicLidar").as_string();
    topicNonGround_ = this->get_parameter("topicNonGround").as_string();
        

    nonground_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(topicNonGround_, 1);
    subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(topicLidar_, 1, bind(&GroundRemovalNodeSimpleZ::cloud_callback, this, std::placeholders::_1));
  }

private:

  float heightLidar_, leftRightRotation_, frontBackRotation_, leftLimit_, rightLimit_, topLimit_, distanceLimit_, verticalInclinationFactor_;
  string frameLidar_, topicLidar_, topicNonGround_;
  bool applyLimit_;

  tbb::spin_mutex callback_mutex_;

  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr nonground_pub_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;

  void cloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg){
    tbb::spin_mutex::scoped_lock lock(callback_mutex_);

    //reading some parameters again in case of dynamic parameter changes
    heightLidar_ = this->get_parameter("heightLidar").as_double();
    leftRightRotation_ = this->get_parameter("leftRightRotation").as_double();
    frontBackRotation_ = this->get_parameter("frontBackRotation").as_double();
    applyLimit_ = this->get_parameter("applyLimit").as_bool();
    leftLimit_ = this->get_parameter("leftLimit").as_double();
    rightLimit_ = this->get_parameter("rightLimit").as_double();
    topLimit_ = this->get_parameter("topLimit").as_double();
    distanceLimit_ = this->get_parameter("distanceLimit").as_double();
    verticalInclinationFactor_ = this->get_parameter("verticalInclinationFactor").as_double();

    
    // Convert ROS2 message to PCL and removing NaN points
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromROSMsg(*msg, cloud);
    pcl::PointCloud<pcl::PointXYZ> cloudtomanipulate;
    std::vector<int> indices;
    pcl::removeNaNFromPointCloud(cloud, cloudtomanipulate, indices);
    RCLCPP_INFO(this->get_logger(), "Received point cloud with %zu points", cloudtomanipulate.points.size());

    //finding non ground points
    pcl::PointCloud<pcl::PointXYZ> nonGroundCloud;
    tbb::combinable<pcl::PointCloud<pcl::PointXYZ>> local_clouds;
    tbb::parallel_for(size_t(0), cloudtomanipulate.points.size(), [&](size_t i){
      auto& pt = cloudtomanipulate.points[i];

      // left-right rotation (rotation about x-axis)
      float y0 = pt.y;
      float z0 = pt.z;
      pt.y = y0 * cosf(leftRightRotation_) + z0 * sinf(leftRightRotation_);
      pt.z = -y0 * sinf(leftRightRotation_) + z0 * cosf(leftRightRotation_);

      // front-back rotation (rotation about y-axis)
      float x0 = pt.x;
      z0 = pt.z;
      pt.x = x0 * cosf(frontBackRotation_) - z0 * sinf(frontBackRotation_);
      pt.z = x0 * sinf(frontBackRotation_) + z0 * cosf(frontBackRotation_);

      // Print XYZ coordinates after rotation (every 100th point to avoid flooding terminal)
      if (i % 100 == 0)
        RCLCPP_INFO(this->get_logger(), "Point %zu after rotation: x=%.3f y=%.3f z=%.3f", i, pt.x, pt.y, pt.z); 

      float dist = sqrtf((pt.x * pt.x) + (pt.y*pt.y));
      if(applyLimit_){ if(pt.y >leftLimit_ || pt.y<-rightLimit_ || pt.z >topLimit_ || dist > distanceLimit_) return;}
      float groundLimit = expf(verticalInclinationFactor_*pt.x) - 1;
      float distance = 29; 

      if(pt.z > -heightLidar_ + groundLimit /* && pow(pt.x, 2)+ pow(pt.y,2) + pow(pt.z, 2) < distance*/ ) local_clouds.local().points.push_back(pt);
      

    });
    local_clouds.combine_each([&](const pcl::PointCloud<pcl::PointXYZ>& local) {nonGroundCloud += local;});

    // Publish non-ground point cloud
    sensor_msgs::msg::PointCloud2 nonGroundMsg;
    pcl::toROSMsg(nonGroundCloud, nonGroundMsg);
    nonGroundMsg.header.frame_id = cloud.header.frame_id.empty() ? frameLidar_ : cloud.header.frame_id;
    nonGroundMsg.header.stamp = msg->header.stamp;

    nonground_pub_->publish(nonGroundMsg);
    RCLCPP_INFO(this->get_logger(), "Published non-ground point cloud with %zu points", nonGroundCloud.points.size());
    RCLCPP_INFO(this->get_logger(), "------------------------\n");
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GroundRemovalNodeSimpleZ>());
  rclcpp::shutdown();
  return 0;
}