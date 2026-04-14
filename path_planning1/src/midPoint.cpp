#include <rclcpp/rclcpp.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <sdc_msgs/msg/coordinate_list.hpp>
#include <sdc_msgs/msg/coordinate.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include "message_filters/subscriber.h"
#include "message_filters/synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"
#include <vector>
#include <cmath>

using std::placeholders::_1;
using std::placeholders::_2;
using namespace std;

class midPointPathPlanningNode : public rclcpp::Node{
public:
  midPointPathPlanningNode(): Node("midPoint"){
    this->declare_parameter<string>("topicOdom", "/odom");
    this->declare_parameter<string>("topicMapArr", "/map_arr");
    this->declare_parameter<string>("topicConeCluster", "/Clusters");
    this->declare_parameter<string>("topicConePair", "/cone_pair");
    topicOdom_ = this->get_parameter("topicOdom").as_string();
    topicMapArr_ = this->get_parameter("topicMapArr").as_string();
    topicConeCluster_ = this->get_parameter("topicConeCluster").as_string();
    topicConePair_ = this->get_parameter("topicConePair").as_string();

    //global variables
    minDistance = 10000.0;
    minLeftCoordinate = make_pair(-1000.0, -1000.0);
    minRightCoordinate = make_pair(-1000.0, -1000.0);
    carCoordinate = make_pair(0.0, 0.0);

    //publishers and subscribers
    // mapArr_sub_.subscribe(this, topicMapArr_);
    mapArr_sub_.subscribe(this, topicConeCluster_);
    odom_sub_.subscribe(this, topicOdom_);
    uint32_t queue_size = 10;
    // sync = std::make_shared<message_filters::Synchronizer<message_filters::sync_policies::
    //        ApproximateTime<sdc_msgs::msg::Arrofarr, nav_msgs::msg::Odometry>>>(
    //        message_filters::sync_policies::ApproximateTime<sdc_msgs::msg::Arrofarr,
    //        nav_msgs::msg::Odometry>(queue_size), mapArr_sub_, odom_sub_);
    sync = std::make_shared<message_filters::Synchronizer<message_filters::sync_policies::
           ApproximateTime<sdc_msgs::msg::CoordinateList, nav_msgs::msg::Odometry>>>(
           message_filters::sync_policies::ApproximateTime<sdc_msgs::msg::CoordinateList,
           nav_msgs::msg::Odometry>(queue_size), mapArr_sub_, odom_sub_);
    sync->setAgePenalty(0.50);
    sync->registerCallback(std::bind(&midPointPathPlanningNode::SyncCallback, this, _1, _2));

    conePair_pub_ = this->create_publisher<sdc_msgs::msg::Arrofarr>(topicConePair_, 1);
    RCLCPP_INFO(this->get_logger(), "mid point path planning node initialized");
  }

private: 
  string topicOdom_, topicMapArr_, topicConePair_, topicConeCluster_;

  //global variables
  vector<pair<float, float>> cones, leftConeCoordinates, rightConeCoordinates;
  float minDistance;
  pair<float,  float> minLeftCoordinate, minRightCoordinate, carCoordinate;

  //publishers and subscribers
  // message_filters::Subscriber<sdc_msgs::msg::Arrofarr> mapArr_sub_;
  message_filters::Subscriber<sdc_msgs::msg::CoordinateList> mapArr_sub_;
  message_filters::Subscriber<nav_msgs::msg::Odometry> odom_sub_;
  // std::shared_ptr<message_filters::Synchronizer<message_filters::sync_policies::ApproximateTime<sdc_msgs::msg::Arrofarr, nav_msgs::msg::Odometry>>> sync;
  std::shared_ptr<message_filters::Synchronizer<message_filters::sync_policies::ApproximateTime<sdc_msgs::msg::CoordinateList, nav_msgs::msg::Odometry>>> sync;
  rclcpp::Publisher<sdc_msgs::msg::Arrofarr>::SharedPtr conePair_pub_;
  
  inline float distance(pair<float, float> x, pair<float, float> y){
    return std::sqrt(((x.first-y.first)*(x.first-y.first)) + ((x.second-y.second)*(x.second-y.second)));
  }

  // void SyncCallback(const sdc_msgs::msg::Arrofarr::ConstSharedPtr & mapArr_msg, const nav_msgs::msg::Odometry::ConstSharedPtr & odom_msg){
  void SyncCallback(const sdc_msgs::msg::CoordinateList::ConstSharedPtr & mapArr_msg, const nav_msgs::msg::Odometry::ConstSharedPtr & odom_msg){
    // RCLCPP_INFO(this->get_logger(), "--------got-messages-------");

    //putting into cones vector and into carCoordinate
    cones.clear();
    // for(auto i: mapArr_msg->data) cones.push_back(make_pair(i.data[0], i.data[1]));
    tf2::Quaternion q(odom_msg->pose.pose.orientation.x, odom_msg->pose.pose.orientation.y, odom_msg->pose.pose.orientation.z, odom_msg->pose.pose.orientation.w); tf2::Matrix3x3 m(q);
    double ir, ip, iy;
    m.getRPY(ir, ip, iy);
    for(auto seenCluster: mapArr_msg->cone_coordinates){
      //convert into global coordinates
      float gx = (seenCluster.x*cos(iy)) - (seenCluster.y*sin(iy)) + odom_msg->pose.pose.position.x;
      float gy = (seenCluster.x*sin(iy)) + (seenCluster.y*cos(iy)) + odom_msg->pose.pose.position.y;
      cones.push_back(make_pair(gx, gy));
    }

    carCoordinate = make_pair(odom_msg->pose.pose.position.x, odom_msg->pose.pose.position.y);
    
    //deciding reference
    pair<float, float> reference;
    if(leftConeCoordinates.size()==0 || rightConeCoordinates.size()==0) reference = make_pair(carCoordinate.first, carCoordinate.second);
    else reference = make_pair((leftConeCoordinates.back().first + rightConeCoordinates.back().first)/2.0, (leftConeCoordinates.back().second + rightConeCoordinates.back().second)/2.0);

    for(int i=0; static_cast<size_t>(i)<cones.size(); i++){
      for(int j=i+1; static_cast<size_t>(j)<cones.size(); j++){

        bool z=true;
        float vec1_i=1.0, vec1_j=1.0, vec2_i=1.0, vec2_j=1.0, angle=180.0;
        if(leftConeCoordinates.size()>0){
          if(distance(cones[i], leftConeCoordinates.back()) < 1.4){
            z=false;
            cones[i].first = leftConeCoordinates.back().first;
            cones[i].second = leftConeCoordinates.back().second;
            vec1_i = cones[j].first - leftConeCoordinates.back().first;
            vec1_j = cones[j].second - leftConeCoordinates.back().second;
            vec2_i = rightConeCoordinates.back().first - leftConeCoordinates.back().first;
            vec2_j = rightConeCoordinates.back().second - leftConeCoordinates.back().second;
          }
          else if(distance(cones[i], rightConeCoordinates.back()) < 1.4){
            z=false;
            cones[i].first = rightConeCoordinates.back().first;
            cones[i].second = rightConeCoordinates.back().second;
            vec1_i = cones[j].first - rightConeCoordinates.back().first;
            vec1_j = cones[j].second - rightConeCoordinates.back().second;
            vec2_i = leftConeCoordinates.back().first - rightConeCoordinates.back().first;
            vec2_j = leftConeCoordinates.back().second - rightConeCoordinates.back().second;
          }else if(distance(cones[j], leftConeCoordinates.back()) < 1.4){
            z = false;
            cones[j].first = leftConeCoordinates.back().first;
            cones[j].second = leftConeCoordinates.back().second;
            vec1_i = cones[i].first - leftConeCoordinates.back().first;
            vec1_j = cones[i].second - leftConeCoordinates.back().second;
            vec2_i = rightConeCoordinates.back().first - leftConeCoordinates.back().first;
            vec2_j = rightConeCoordinates.back().second - leftConeCoordinates.back().second;
          }else if(distance(cones[j], rightConeCoordinates.back()) < 1.4){
            z=false;
            cones[j].first = rightConeCoordinates.back().first;
            cones[j].second = rightConeCoordinates.back().second;
            vec1_i = cones[i].first - rightConeCoordinates.back().first;
            vec1_j = cones[i].second - rightConeCoordinates.back().second;
            vec2_i = leftConeCoordinates.back().first - rightConeCoordinates.back().first;
            vec2_j = leftConeCoordinates.back().second - rightConeCoordinates.back().second;
          }
        }
        if(z==false){
          float dot = (vec1_i*vec2_i) + (vec1_j*vec2_j);
          float mag1 = std::sqrt((vec1_i*vec1_i) + (vec1_j*vec1_j));
          float mag2 = std::sqrt((vec2_i*vec2_i) + (vec2_j*vec2_j));
          float y=dot/(mag1*mag2);
          if(y>1.0) y = 1.0;
          angle = (std::acos(y))*((180.0)/(3.14159265));
        }

        float left_angle=1.0;
        if(leftConeCoordinates.size()>=2 && distance(leftConeCoordinates.back(), cones[j]) > 0.5){
          int k=static_cast<int>(leftConeCoordinates.size())-2;
          while(distance(leftConeCoordinates[k], leftConeCoordinates.back())<0.3){ 
            k--;
            if(k<0){k=0; break;}
          }

          if(k!=0){
            float v1_i = cones[j].first - leftConeCoordinates.back().first;
            float v1_j = cones[j].second - leftConeCoordinates.back().second;
            float v2_i = leftConeCoordinates.back().first - leftConeCoordinates[k].first;
            float v2_j = leftConeCoordinates.back().second - leftConeCoordinates[k].second;
            float dot = (v1_i*v2_i) + (v1_j*v2_j);
            float mag1 = std::sqrt((v1_i*v1_i) + (v1_j*v1_j));
            float mag2 = std::sqrt((v2_i*v2_i) + (v2_j*v2_j));
            float y=dot/(mag1*mag2);
            if(y>1.0) y = 1.0;
            left_angle = (std::acos(y))*((180.0)/(3.14159265));
          }
        }

        float right_angle=1.0;
        if(rightConeCoordinates.size()>=2 && distance(rightConeCoordinates.back(), cones[i])>0.5){
          int k=static_cast<int>(rightConeCoordinates.size())-2;
          while(distance(rightConeCoordinates[k], rightConeCoordinates.back())<0.3){
            k--;
            if(k<0){k=0; break;}
          }
          
          if(k!=0){
            float v1_i = cones[i].first - rightConeCoordinates.back().first;
            float v1_j = cones[i].second - rightConeCoordinates.back().second;
            float v2_i = rightConeCoordinates.back().first - rightConeCoordinates[k].first;
            float v2_j = rightConeCoordinates.back().second - rightConeCoordinates[k].second;
            float dot = (v1_i*v2_i) + (v1_j*v2_j);
            float mag1 = std::sqrt((v1_i*v1_i) + (v1_j*v1_j));
            float mag2 = std::sqrt((v2_i*v2_i) + (v2_j*v2_j));
            float y=dot/(mag1*mag2);
            if(y>1.0) y = 1.0;
            right_angle = std::acos(y)*(180.0/3.14159265);
          }
        }

        float ang=0.0;
        if(leftConeCoordinates.size()>1){
          float prev_dir_i = reference.first - ((leftConeCoordinates[leftConeCoordinates.size()-2].first + rightConeCoordinates[rightConeCoordinates.size()-2].first)/2.0);
          float prev_dir_j = reference.second - ((leftConeCoordinates[leftConeCoordinates.size()-2].second + rightConeCoordinates[rightConeCoordinates.size()-2].second)/2.0);
          float new_dir_i = ((cones[i].first + cones[j].first)/2.0) - reference.first;
          float new_dir_j = ((cones[i].second + cones[j].second)/2.0) - reference.second;
          float m1 = std::sqrt((prev_dir_i*prev_dir_i) + (prev_dir_j*prev_dir_j));
          float m2 = std::sqrt((new_dir_i*new_dir_i) + (new_dir_j*new_dir_j));
          float d = (prev_dir_i*new_dir_i) + (prev_dir_j*new_dir_j);
          float y = d/(m1*m2);
          if(y>1.0) y=1.0;
          ang = std::acos(y)*(180.0/3.14159265);
        }

        // bool u = true;
        // if(leftConeCoordinates.size()>1){
        //   u = false;
        //   if(distance(leftConeCoordinates.back(), cones[j]) < 7.0){ if(distance(rightConeCoordinates.back(), cones[i]) < 7.0) u = true;}
        // }

        float mid_x = (cones[i].first + cones[j].first)/2.0;
        float mid_y = (cones[i].second + cones[j].second)/2.0;
        pair<float, float> mid = make_pair(mid_x, mid_y);
        float minimum = distance(reference, mid);
        if((ang < 85.0) && (left_angle < 45.0) && (right_angle < 80.0) && (z || (angle < 60.0 && angle > 15.0)) && true && distance(cones[i], cones[j]) < 8.0){
          if((minimum < minDistance) && minimum > 0.8){
            minDistance = minimum;
            pair<float, float> l = {0,0}, r = {0,0};
            l.first = cones[j].first;
            l.second = cones[j].second;
            r.first = cones[i].first;
            r.second = cones[i].second;
            minLeftCoordinate = l;
            minRightCoordinate = r;
          }
        }
      }
    }

    if(leftConeCoordinates.size()==0 && distance(reference, carCoordinate) < 1.0){
      leftConeCoordinates.push_back(minLeftCoordinate);
      rightConeCoordinates.push_back(minRightCoordinate);

      //----------------publish----------------
      sdc_msgs::msg::Arrofarr conepair;
      sdc_msgs::msg::Arr leftCone, rightCone;
      leftCone.data = {minLeftCoordinate.first, minLeftCoordinate.second, 0.0};
      rightCone.data = {minRightCoordinate.first, minRightCoordinate.second, 0.0};
      conepair.data.push_back(leftCone);
      conepair.data.push_back(rightCone);
      cout << "Left Cone: " << minLeftCoordinate.first << ", " << minLeftCoordinate.second << endl;
      cout << "Right Cone: " << minRightCoordinate.first << ", " << minRightCoordinate.second << endl;
      conepair.header.stamp = this->get_clock()->now();
      conepair.header.frame_id = "map";
      conePair_pub_->publish(conepair);
      //---------------------------------------

      //reset variables
      minDistance = 10000.0;
      minLeftCoordinate = make_pair(-1000.0, -1000.0);
      minRightCoordinate = make_pair(-1000.0, -1000.0);

    }
    else{
      
      float A1 = leftConeCoordinates.back().second - rightConeCoordinates.back().second;
      float B1 = rightConeCoordinates.back().first - leftConeCoordinates.back().first;
      float C1 = (leftConeCoordinates.back().first * rightConeCoordinates.back().second) - (rightConeCoordinates.back().first * leftConeCoordinates.back().second);
      float dist = abs(A1*carCoordinate.first + B1*carCoordinate.second + C1)/std::sqrt(A1*A1 + B1*B1);

      if(dist < 0.3){
        leftConeCoordinates.push_back(minLeftCoordinate);
        rightConeCoordinates.push_back(minRightCoordinate);

        //----------------publish----------------
        sdc_msgs::msg::Arrofarr conepair;
        sdc_msgs::msg::Arr leftCone, rightCone;
        leftCone.data = {minLeftCoordinate.first, minLeftCoordinate.second, 0.0};
        rightCone.data = {minRightCoordinate.first, minRightCoordinate.second, 0.0};
        conepair.data.push_back(leftCone);
        conepair.data.push_back(rightCone);
        cout << "Left Cone: " << minLeftCoordinate.first << ", " << minLeftCoordinate.second << endl;
        cout << "Right Cone: " << minRightCoordinate.first << ", " << minRightCoordinate.second << endl;
        conepair.header.stamp = this->get_clock()->now();
        conepair.header.frame_id = "map";
        conePair_pub_->publish(conepair);
        //---------------------------------------

        //reset variables
        minDistance = 10000.0;
        minLeftCoordinate = make_pair(-1000.0, -1000.0);
        minRightCoordinate = make_pair(-1000.0, -1000.0);
      }
    }

  }
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<midPointPathPlanningNode>());
  rclcpp::shutdown();
  return 0;
}