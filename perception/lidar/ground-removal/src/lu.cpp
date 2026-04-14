#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/filter.h>
#include <visualization_msgs/msg/marker_array.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <tbb/parallel_for.h>
#include <tbb/spin_mutex.h>
#include <tbb/combinable.h>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

class GroundRemovalNodeLU : public rclcpp::Node{
public:
    GroundRemovalNodeLU(): Node("lu"){

        //declaring parameters
        this->declare_parameter<float>("heightLidar", 0.8);
        this->declare_parameter<float>("startAngleLidar", M_PI/6);
        this->declare_parameter<float>("endAngleLidar", (5*M_PI)/6);
        this->declare_parameter<string>("frameLidar", "robosense");
        this->declare_parameter<string>("topicLidar", "/rslidar_points");
        this->declare_parameter<float>("sectorAngle", M_PI/12); //must be a factor of endAngle-startAngle
        this->declare_parameter<float>("ringLength", 1.0);
        this->declare_parameter<int>("startRing", 0);
        this->declare_parameter<int>("lastRing", 20);
        this->declare_parameter<float>("removalThresh", 0.1);
        this->declare_parameter<bool>("applyLimit", false);
        this->declare_parameter<float>("leftLimit", 5.0);
        this->declare_parameter<float>("rightLimit", 5.0);
        this->declare_parameter<float>("topLimit", 2.0);
        this->declare_parameter<bool>("applyAverage", false);
        this->declare_parameter<int>("pNaught", 5);
        this->declare_parameter<float>("tau", 1.0);
        this->declare_parameter<bool>("enablePlaneVis", false);
        this->declare_parameter<bool>("enableRingSectorVis", false);

        //reading parameters
        heightLidar_ = this->get_parameter("heightLidar").as_double();
        startAngleLidar_ = this->get_parameter("startAngleLidar").as_double();
        endAngleLidar_ = this->get_parameter("endAngleLidar").as_double();
        frameLidar_ = this->get_parameter("frameLidar").as_string();
        topicLidar_ = this->get_parameter("topicLidar").as_string();
        sectorAngle_ = this->get_parameter("sectorAngle").as_double();
        ringLength_ = this->get_parameter("ringLength").as_double();
        startRing_ = this->get_parameter("startRing").as_int();
        lastRing_ = this->get_parameter("lastRing").as_int();
        
        

        //initializing global variables
        oneRingBins = static_cast<int>(round((endAngleLidar_-startAngleLidar_)/(sectorAngle_)));
        totalBins = (lastRing_-startRing_+1)*oneRingBins;
        ground.assign(4 * totalBins, -1000.f);
        binMinPoint.assign(totalBins, -1);
        binXMinPoints.resize(totalBins);
        visualisationPoints.assign(9 * totalBins, -1000.f);
        bin_mutexes = vector<tbb::spin_mutex>(totalBins);


        //publishers and subscriber
        nonground_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/nonground", 1);
        ground_pub_ = this->create_publisher<sdc_msgs::msg::Arrofarr>("/ground", 1);
        marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("/visualization_marker", 1);
        subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(topicLidar_, 1, bind(&GroundRemovalNodeLU::cloud_callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "Ground removal node initialized");
    }

private:
    //node parameters
    float heightLidar_, startAngleLidar_, endAngleLidar_, sectorAngle_, ringLength_, removalThresh_, leftLimit_, rightLimit_, topLimit_, tau_;
    int startRing_, lastRing_, pNaught_;
    bool applyLimit_, applyAverage_, enablePlaneVis_, enableRingSectorVis_;
    string frameLidar_, topicLidar_;
    

    //struct to hold minimum points in a bin for averaging
    struct minPointSack{
        vector<int> indices;
        float max_z = -1000.f;

        void insert(int idx, const pcl::PointCloud<pcl::PointXYZ>& cloud, int maxpoints){
            float z = cloud.points[idx].z;
            if(indices.size()<static_cast<size_t>(maxpoints)){
                indices.push_back(idx);
                if(z>max_z) max_z = z;
            }
            else if(z<max_z){
                int max_i = 0;
                for(size_t i=1; i<indices.size(); i++) if(cloud.points[indices[i]].z > cloud.points[indices[max_i]].z) max_i = i;
                indices[max_i] = idx;
                max_z = cloud.points[indices[0]].z;
                for(size_t i=1; i<indices.size(); i++) if(cloud.points[indices[i]].z > max_z) max_z = cloud.points[indices[i]].z;
            }
        }

        void clear(){
            indices.clear();
            max_z = -1000.f;
        }
    };


    //global variables
    int totalBins;
    int oneRingBins;
    vector<float> ground;
    vector<int> binMinPoint;
    vector<minPointSack> binXMinPoints;
    vector<float> visualisationPoints;

    //mutexes
    vector<tbb::spin_mutex> bin_mutexes;
    tbb::spin_mutex callback_mutex_;

    
    //publishers and subscriber
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr nonground_pub_;
    rclcpp::Publisher<sdc_msgs::msg::Arrofarr>::SharedPtr ground_pub_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;

    void visualizeRingsAndSectors()
    {
        visualization_msgs::msg::MarkerArray marker_array;

        // Common marker setup
        visualization_msgs::msg::Marker marker;
        marker.header.frame_id = frameLidar_;
        marker.header.stamp = this->now();
        marker.ns = "rings_and_sectors";
        marker.action = visualization_msgs::msg::Marker::ADD;
        marker.type = visualization_msgs::msg::Marker::LINE_LIST;
        marker.color.a = 1.0;
        marker.color.r = 0.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        marker.scale.x = 0.014; // Line thickness

        // ---------------------- Rings -----------------------
        for (int ring = startRing_; ring <= lastRing_; ++ring)
        {
            float radius = ring * ringLength_;
            int num_segments = 100;
            for (int i = 0; i < num_segments; ++i)
            {
                float theta1 = startAngleLidar_ + (i * (endAngleLidar_ - startAngleLidar_) / num_segments);
                float theta2 = startAngleLidar_ + ((i + 1) * (endAngleLidar_ - startAngleLidar_) / num_segments);

                geometry_msgs::msg::Point p1, p2;
                p1.x = radius * sin(theta1);
                p1.y = -radius * cos(theta1);
                p1.z = -heightLidar_;

                p2.x = radius * sin(theta2);
                p2.y = -radius * cos(theta2);
                p2.z = -heightLidar_;

                marker.points.push_back(p1);
                marker.points.push_back(p2);
            }
        }

        // ---------------------- Sectors -----------------------
        for (int bin = 0; bin <= oneRingBins; ++bin)
        {
            float angle = startAngleLidar_ + bin * sectorAngle_;

            geometry_msgs::msg::Point inner, outer;
            inner.x = (startRing_ * ringLength_) * sin(angle);
            inner.y = -(startRing_ * ringLength_) * cos(angle);
            inner.z = -heightLidar_;

            outer.x = (lastRing_ * ringLength_) * sin(angle);
            outer.y = -(lastRing_ * ringLength_) * cos(angle);
            outer.z = -heightLidar_;

            marker.points.push_back(inner);
            marker.points.push_back(outer);
        }

        marker_array.markers.push_back(marker);
        marker_pub_->publish(marker_array);
    }

    void visualizeGroundPlanes(){
        visualization_msgs::msg::MarkerArray marker_array;
        visualization_msgs::msg::Marker marker;
        marker.header.frame_id = frameLidar_; marker.header.stamp = this->now();
        marker.ns = "ground_planes"; marker.id = 0;
        marker.type = visualization_msgs::msg::Marker::TRIANGLE_LIST;
        marker.action = visualization_msgs::msg::Marker::ADD;
        marker.color.a = 1.0; marker.color.r = 0.0; marker.color.g = 1.0; marker.color.b = 0.0;
        marker.scale.x = 1.0; marker.scale.y = 1.0; marker.scale.z = 1.0;
        
        for(int bin_num=0; bin_num<totalBins; bin_num++){
            if(visualisationPoints[9*bin_num]==-1000.f) continue;
            
            geometry_msgs::msg::Point p1, p2, p3;
            p1.x = visualisationPoints[9*bin_num]; p1.y = visualisationPoints[9*bin_num+1]; p1.z = visualisationPoints[9*bin_num+2];
            p2.x = visualisationPoints[9*bin_num+3]; p2.y = visualisationPoints[9*bin_num+4]; p2.z = visualisationPoints[9*bin_num+5];
            p3.x = visualisationPoints[9*bin_num+6]; p3.y = visualisationPoints[9*bin_num+7]; p3.z = visualisationPoints[9*bin_num+8];
            marker.points.push_back(p1); marker.points.push_back(p2); marker.points.push_back(p3);
            
        }
        marker_array.markers.push_back(marker);
        marker_pub_->publish(marker_array);
    }


    inline int computeBin(const pcl::PointXYZ& pt){
        int ring = static_cast<int>(sqrtf((pt.x * pt.x) + (pt.y*pt.y))/ringLength_); if(ring<startRing_ || ring>lastRing_) return -1;
        float angle = atan2(pt.x, -(pt.y)) - startAngleLidar_; if(angle < 0) angle += 2 * M_PI; if(angle >= (endAngleLidar_-startAngleLidar_) || angle <= 0) return -1;
        int bin = static_cast<int>((angle)/(sectorAngle_)) + oneRingBins*ring; if(bin<0 || bin>=totalBins) return -1;
        return bin;
    }

    void cloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg){
        tbb::spin_mutex::scoped_lock lock(callback_mutex_);
        removalThresh_ = this->get_parameter("removalThresh").as_double();
        applyLimit_ = this->get_parameter("applyLimit").as_bool();
        leftLimit_ = this->get_parameter("leftLimit").as_double();
        rightLimit_ = this->get_parameter("rightLimit").as_double();
        topLimit_ = this->get_parameter("topLimit").as_double();
        applyAverage_ = this->get_parameter("applyAverage").as_bool();
        pNaught_ = this->get_parameter("pNaught").as_int();
        tau_ = this->get_parameter("tau").as_double();
        enablePlaneVis_ = this->get_parameter("enablePlaneVis").as_bool();
        enableRingSectorVis_ = this->get_parameter("enableRingSectorVis").as_bool();

        // Convert ROS2 message to PCL and removing NaN points
        pcl::PointCloud<pcl::PointXYZ> cloud;
        pcl::fromROSMsg(*msg, cloud);
        pcl::PointCloud<pcl::PointXYZ> cloudtomanipulate;
        std::vector<int> indices;
        pcl::removeNaNFromPointCloud(cloud, cloudtomanipulate, indices);
        RCLCPP_INFO(this->get_logger(), "Received point cloud with %zu points", cloudtomanipulate.points.size());


        //clearing previous data
        
        if(enablePlaneVis_) std::fill(visualisationPoints.begin(), visualisationPoints.end(), -1000.f);
        std::fill(ground.begin(), ground.end(), -1000.f);
        std::fill(binMinPoint.begin(), binMinPoint.end(), -1);
        if(applyAverage_) {for(auto &b : binXMinPoints) b.clear();}



        //---------------finding the minimum point in each bin-----------------
        tbb::parallel_for(size_t(0), cloudtomanipulate.points.size(), [&](size_t i){

            const auto& pt = cloudtomanipulate.points[i];
            if(applyLimit_){ if(pt.y >leftLimit_ || pt.y<-rightLimit_ || pt.z >topLimit_) return;}
            int bin = computeBin(pt); if(bin==-1) return;
            
            //finding the minimum point in that bin
            tbb::spin_mutex::scoped_lock lock(bin_mutexes[bin]);
            if(binMinPoint[bin]==-1 || cloudtomanipulate.points[i].z < cloudtomanipulate.points[binMinPoint[bin]].z) binMinPoint[bin] = i;
            if(applyAverage_){int ring = static_cast<int>(sqrtf((pt.x * pt.x) + (pt.y*pt.y))/ringLength_); int maxpoints = floor(pNaught_*expf(-ring/tau_))+1; binXMinPoints[bin].insert(i, cloudtomanipulate, maxpoints);}
        });

        //taking average of lowest points in each bin. Then updating the z value of the lowest point in that bin to the average
        if(applyAverage_){
            for(int bin_num=0; bin_num<totalBins; bin_num++){
                auto& b = binXMinPoints[bin_num];
                if(!b.indices.empty()){
                    float zsum = 0.f;
                    for(int idx : b.indices) zsum += cloudtomanipulate.points[idx].z;
                    cloudtomanipulate.points[binMinPoint[bin_num]].z = zsum / b.indices.size();
                }
            }
        }
        //-------------------------------------------------------------------------


        //declaring the ground message
        sdc_msgs::msg::Arrofarr ground_msg;
        sdc_msgs::msg::Arr temp;
        std::mutex ground_mutex;

        //------------------defining the ground planes------------------------
        tbb::parallel_for(0, totalBins, [&](int bin_num){

            //do not run for 0th ring or sectors at even index
            if(bin_num%oneRingBins==0 || bin_num%2==0) return;
            //if no points in this bin and the bin to its right, skip
            if(binMinPoint[bin_num]==-1 && binMinPoint[bin_num-1]==-1) return;

            //defining the plane in bin_num using the lowest point in bin_num, bin_num-1, and middle point of previous ring's corresponding bins
            float x1, y1, z1, x2, y2, z2;
            if(binMinPoint[bin_num]!=-1){x1 = cloudtomanipulate.points[binMinPoint[bin_num]].x; y1 = cloudtomanipulate.points[binMinPoint[bin_num]].y; z1 = cloudtomanipulate.points[binMinPoint[bin_num]].z;}
            if(binMinPoint[bin_num-1]!=-1){x2 = cloudtomanipulate.points[binMinPoint[bin_num-1]].x; y2 = cloudtomanipulate.points[binMinPoint[bin_num-1]].y; z2 = cloudtomanipulate.points[binMinPoint[bin_num-1]].z;}
            if(binMinPoint[bin_num]==-1) {x1 = x2+0.2; y1 = y2+0.2; z1 = z2;} if(binMinPoint[bin_num-1]==-1) {x2 = x1-0.2; y2 = y1-0.2; z2 = z1;}
 
            
            float x3, y3, z3; int lastring_bin_num = bin_num - oneRingBins;
            while(lastring_bin_num>0 && binMinPoint[lastring_bin_num]==-1 && binMinPoint[lastring_bin_num-1]==-1) lastring_bin_num -= oneRingBins;
            if(lastring_bin_num<0){x3=0; y3=0; z3=-heightLidar_;}
            
            else{
                if(binMinPoint[lastring_bin_num]==-1){x3 = cloudtomanipulate.points[binMinPoint[lastring_bin_num-1]].x;
                                                      y3 = cloudtomanipulate.points[binMinPoint[lastring_bin_num-1]].y;
                                                      z3 = cloudtomanipulate.points[binMinPoint[lastring_bin_num-1]].z; }

                else if(binMinPoint[lastring_bin_num-1]==-1){x3 = cloudtomanipulate.points[binMinPoint[lastring_bin_num]].x;
                                                             y3 = cloudtomanipulate.points[binMinPoint[lastring_bin_num]].y;
                                                             z3 = cloudtomanipulate.points[binMinPoint[lastring_bin_num]].z; }
                else{
                    x3 = (cloudtomanipulate.points[binMinPoint[lastring_bin_num]].x + cloudtomanipulate.points[binMinPoint[lastring_bin_num-1]].x)/2;
                    y3 = (cloudtomanipulate.points[binMinPoint[lastring_bin_num]].y + cloudtomanipulate.points[binMinPoint[lastring_bin_num-1]].y)/2;
                    z3 = (cloudtomanipulate.points[binMinPoint[lastring_bin_num]].z + cloudtomanipulate.points[binMinPoint[lastring_bin_num-1]].z)/2;
                }
            }

            float a = ((y2-y1)*(z3-z1)) - ((z2-z1)*(y3-y1));
            float b = ((z2-z1)*(x3-x1)) - ((x2-x1)*(z3-z1));
            float c = ((x2-x1)*(y3-y1)) - ((y2-y1)*(x3-x1));
            float d = -(a*x1 + b*y1 + c*z1);

            if(enablePlaneVis_){
                visualisationPoints[9*bin_num] = x1; visualisationPoints[9*bin_num+1] = y1; visualisationPoints[9*bin_num+2] = z1;
                visualisationPoints[9*bin_num+3] = x2; visualisationPoints[9*bin_num+4] = y2; visualisationPoints[9*bin_num+5] = z2;
                visualisationPoints[9*bin_num+6] = x3; visualisationPoints[9*bin_num+7] = y3; visualisationPoints[9*bin_num+8] = z3;
            }

            std::lock_guard<std::mutex> lock(ground_mutex);
            ground[4*(bin_num-1)] = a; ground[4*(bin_num-1)+1] = b; ground[4*(bin_num-1)+2] = c; ground[4*(bin_num-1)+3] = d; temp.data = {float(bin_num-1), a, b, c, d}; ground_msg.data.push_back(temp);
            ground[4*bin_num] = a; ground[4*bin_num+1] = b; ground[4*bin_num+2] = c; ground[4*bin_num+3] = d; temp.data = {float(bin_num), a, b, c, d}; ground_msg.data.push_back(temp);
            // if((bin_num-1)%oneRingBins==0){ ground[4*(bin_num-1)] = a; ground[(4*(bin_num-1))+1] = b; ground[(4*(bin_num-1))+2] = c; ground[(4*(bin_num-1))+3] = d; temp.data = {float(bin_num-1), a, b, c, d}; ground_msg.data.push_back(temp);}
        });
        //-------------------------------------------------------------------------

        if(enablePlaneVis_) visualizeGroundPlanes();
        if(enableRingSectorVis_) visualizeRingsAndSectors();

        //declaring nonGroundCloud
        pcl::PointCloud<pcl::PointXYZ> nonGroundCloud;
        tbb::combinable<pcl::PointCloud<pcl::PointXYZ>> local_clouds;

        //----------------------finding non ground points---------------------------
        tbb::parallel_for(size_t(0), cloudtomanipulate.points.size(), [&](size_t i){

            const auto& pt = cloudtomanipulate.points[i];
            if(applyLimit_){ if(pt.y >leftLimit_ || pt.y<-rightLimit_ || pt.z >topLimit_) return;}
            int bin = computeBin(pt); if(bin==-1) return;
            
            //including the point in nonGroundCloud if distance from ground plane > removalThresh
            if(ground[4*bin]==-1000.f) return;
            float a = ground[4*bin]; float b = ground[4*bin+1]; float c = ground[4*bin+2]; float d = ground[4*bin+3];
            float denominator = sqrtf(a*a + b*b + c*c); if(denominator==0) return;
            float distance = fabs((a*pt.x + b*pt.y + c*pt.z + d)/denominator);
            if(distance > removalThresh_) local_clouds.local().points.push_back(pt);
            
        });
        local_clouds.combine_each([&](const pcl::PointCloud<pcl::PointXYZ>& local) {nonGroundCloud += local;});

        // -------------------------------------------------------------------------

        //converting nonGroundCloud to ROS2 message and publishing
        sensor_msgs::msg::PointCloud2 nonGroundMsg;
        pcl::toROSMsg(nonGroundCloud, nonGroundMsg);
        nonGroundMsg.header.frame_id = cloud.header.frame_id.empty() ? frameLidar_ : cloud.header.frame_id;
        nonGroundMsg.header.stamp = this->get_clock()->now();

        nonground_pub_->publish(nonGroundMsg);
        RCLCPP_INFO(this->get_logger(), "Published non-ground point cloud with %zu points", nonGroundCloud.points.size());
        ground_msg.header.stamp = this->get_clock()->now();
        ground_msg.header.frame_id = cloud.header.frame_id.empty() ? frameLidar_ : cloud.header.frame_id;
        ground_pub_->publish(ground_msg);
        RCLCPP_INFO(this->get_logger(), "Published ground planes for %zu bins", ground_msg.data.size());
        RCLCPP_INFO(this->get_logger(), "----------------------------------------\n");

    }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GroundRemovalNodeLU>());
  rclcpp::shutdown();
  return 0;
}