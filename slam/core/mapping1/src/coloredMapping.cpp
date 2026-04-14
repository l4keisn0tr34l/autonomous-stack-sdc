#include <rclcpp/rclcpp.hpp>
#include <sdc_msgs/msg/coordinate_list.hpp>
#include <sdc_msgs/msg/coordinate.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include "tf2_ros/static_transform_broadcaster.h"
#include "message_filters/subscriber.h"
#include "message_filters/synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <vector>
#include <string>
#include <cmath>

using std::placeholders::_1;
using std::placeholders::_2;
using namespace std;

class coloredMappingNode : public rclcpp::Node {
public:
  coloredMappingNode() : Node("coloredMapping") {

    // parameters
    this->declare_parameter<string>("topicClusters", "/mapping_input");
    this->declare_parameter<string>("topicOdom", "/odom");
    this->declare_parameter<string>("topicMapArr", "/map_arr");
    this->declare_parameter<string>("topicMapVis", "/map");
    this->declare_parameter<float>("matchDistanceThresh", 2.0);
    this->declare_parameter<float>("coneDistanceThresh", 12.0);

    topicClusters_ = this->get_parameter("topicClusters").as_string();
    topicOdom_ = this->get_parameter("topicOdom").as_string();
    topicMapArr_ = this->get_parameter("topicMapArr").as_string();
    topicMapVis_ = this->get_parameter("topicMapVis").as_string();

    // global variables
    firstMsg = true;

    // publishers and subscribers
    clusters_sub_.subscribe(this, topicClusters_);
    odom_sub_.subscribe(this, topicOdom_);

    uint32_t queue_size = 10;
    sync = std::make_shared<
      message_filters::Synchronizer<
        message_filters::sync_policies::ApproximateTime<
          sdc_msgs::msg::Arrofarr,
          nav_msgs::msg::Odometry>>>(
            message_filters::sync_policies::ApproximateTime<
              sdc_msgs::msg::Arrofarr,
              nav_msgs::msg::Odometry>(queue_size),
            clusters_sub_,
            odom_sub_);

    sync->setAgePenalty(0.50);
    sync->registerCallback(std::bind(&coloredMappingNode::SyncCallback, this, _1, _2));

    mapArr_pub_ = this->create_publisher<sdc_msgs::msg::Arrofarr>(topicMapArr_, 1);
    mapVis_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(topicMapVis_, 1);
    tf_static_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
    make_transforms();

    RCLCPP_INFO(this->get_logger(), "coloredMapping node initialized");
  }

private:
  // parameters
  string topicClusters_, topicMapArr_, topicMapVis_, topicOdom_;
  float matchDistanceThresh_;
  float coneDistanceThresh_;

  // global variables
  sdc_msgs::msg::Arrofarr mapArr;
  geometry_msgs::msg::TransformStamped transformStamped;
  bool firstMsg;

  // publishers and subscribers
  message_filters::Subscriber<sdc_msgs::msg::Arrofarr> clusters_sub_;
  message_filters::Subscriber<nav_msgs::msg::Odometry> odom_sub_;
  std::shared_ptr<
    message_filters::Synchronizer<
      message_filters::sync_policies::ApproximateTime<
        sdc_msgs::msg::Arrofarr,
        nav_msgs::msg::Odometry>>> sync;

  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr mapVis_pub_;
  rclcpp::Publisher<sdc_msgs::msg::Arrofarr>::SharedPtr mapArr_pub_;
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_static_broadcaster_;

  void make_transforms() {
    transformStamped.header.stamp = this->get_clock()->now();
    transformStamped.header.frame_id = "map";
    transformStamped.child_frame_id = "odom";
    transformStamped.transform.translation.x = 0.0;
    transformStamped.transform.translation.y = 0.0;
    transformStamped.transform.translation.z = 0.0;
    transformStamped.transform.rotation.x = 0.0;
    transformStamped.transform.rotation.y = 0.0;
    transformStamped.transform.rotation.z = 0.0;
    transformStamped.transform.rotation.w = 1.0;

    tf_static_broadcaster_->sendTransform(transformStamped);
  }

  void visualise() {
    visualization_msgs::msg::MarkerArray marker_array;

    visualization_msgs::msg::Marker delete_all;
    delete_all.action = visualization_msgs::msg::Marker::DELETEALL;
    marker_array.markers.push_back(delete_all);

    int id = 100;
    for (const auto &mappedCluster : mapArr.data) {
      if (mappedCluster.data.size() < 3) continue;

      visualization_msgs::msg::Marker marker;
      marker.header.frame_id = "map";
      marker.header.stamp = this->get_clock()->now();
      marker.ns = "mapped_points";
      marker.id = id++;
      marker.type = visualization_msgs::msg::Marker::CYLINDER;
      marker.action = visualization_msgs::msg::Marker::ADD;

      // Position
      marker.pose.position.x = mappedCluster.data[0];
      marker.pose.position.y = mappedCluster.data[1];
      marker.pose.position.z = 0.0;

      // Orientation
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w = 1.0;

      // Scale
      marker.scale.x = 0.25;
      marker.scale.y = 0.25;
      marker.scale.z = 0.4;
      if (mappedCluster.data[2] == 3.0) marker.scale.z = 0.8;

      // Color
      marker.color.a = 1.0f;
      if (mappedCluster.data[2] == 0.0) { marker.color.r = 0.0f;   marker.color.g = 0.0f;   marker.color.b = 1.0f;   } // blue
      if (mappedCluster.data[2] == 1.0) { marker.color.r = 1.0f;   marker.color.g = 1.0f;   marker.color.b = 0.0f;   } // yellow
      if (mappedCluster.data[2] == 2.0 || mappedCluster.data[2] == 3.0) {
        marker.color.r = 1.0f; marker.color.g = 0.647f; marker.color.b = 0.0f;
      } // orange/big orange
      if (mappedCluster.data[2] == 4.0 || mappedCluster.data[2] < 0.0) {
        marker.color.r = 0.647f; marker.color.g = 0.647f; marker.color.b = 0.647f;
      } // unknown

      marker.lifetime = rclcpp::Duration(0, 0);
      marker_array.markers.push_back(marker);
    }

    mapVis_pub_->publish(marker_array);
  }

  void SyncCallback(
    const sdc_msgs::msg::Arrofarr::ConstSharedPtr &clusters_msg,
    const nav_msgs::msg::Odometry::ConstSharedPtr &odom_msg) {

    sdc_msgs::msg::Arr temp;
    matchDistanceThresh_ = this->get_parameter("matchDistanceThresh").as_double();
    coneDistanceThresh_ = this->get_parameter("coneDistanceThresh").as_double();

    // get yaw
    tf2::Quaternion q(
      odom_msg->pose.pose.orientation.x,
      odom_msg->pose.pose.orientation.y,
      odom_msg->pose.pose.orientation.z,
      odom_msg->pose.pose.orientation.w
    );
    tf2::Matrix3x3 m(q);
    double ir, ip, iy;
    m.getRPY(ir, ip, iy);

    vector<vector<double>> arrivedClusters;

    // Expected input format:
    // arr.data[0] = x
    // arr.data[1] = y
    // arr.data[2] = colour
    for (const auto &arr : clusters_msg->data) {
      if (arr.data.size() < 3) continue;
      arrivedClusters.push_back({arr.data[0], arr.data[1], arr.data[2]});
    }

    for (auto seenCluster : arrivedClusters) {

      // ignore cones too far away in local frame
      float distFromEgo = (seenCluster[0] * seenCluster[0]) + (seenCluster[1] * seenCluster[1]);
      if (distFromEgo > coneDistanceThresh_ * coneDistanceThresh_) continue;

      // convert into global coordinates
      float gx = (seenCluster[0] * cos(iy)) - (seenCluster[1] * sin(iy)) + odom_msg->pose.pose.position.x;
      float gy = (seenCluster[0] * sin(iy)) + (seenCluster[1] * cos(iy)) + odom_msg->pose.pose.position.y;
      float gcolour = static_cast<float>(seenCluster[2]);

      if (firstMsg) {
        temp.data = {gx, gy, gcolour};
        mapArr.data.push_back(temp);
        continue;
      }

      // find closest mapped cluster
      float minDist = 99999.0;
      int closestMatchIdx = -1;
      int counter = 0;

      for (const auto &mappedCluster : mapArr.data) {
        if (mappedCluster.data.size() < 3) {
          counter++;
          continue;
        }

        float dist =
          ((gx - mappedCluster.data[0]) * (gx - mappedCluster.data[0])) +
          ((gy - mappedCluster.data[1]) * (gy - mappedCluster.data[1]));

        if (dist < minDist) {
          minDist = dist;
          closestMatchIdx = counter;
        }
        counter++;
      }

      if (minDist <= matchDistanceThresh_ * matchDistanceThresh_ && closestMatchIdx >= 0) {
        // if same colour already exists nearby, ignore duplicate
        // if unknown comes in, don't overwrite known cone
        if (mapArr.data[closestMatchIdx].data[2] == gcolour || gcolour == 4.0 || gcolour < 0.0) {
          continue;
        } else {
          // if a colored cone replaces a wrong/older nearby cone, remove old one
          mapArr.data.erase(mapArr.data.begin() + closestMatchIdx);
        }
      }

      temp.data = {gx, gy, gcolour};
      mapArr.data.push_back(temp);
    }

    firstMsg = false;

    // publish
    mapArr.header.stamp = this->get_clock()->now();
    mapArr.header.frame_id = "map";
    mapArr_pub_->publish(mapArr);
    visualise();
  }
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<coloredMappingNode>());
  rclcpp::shutdown();
  return 0;
}
