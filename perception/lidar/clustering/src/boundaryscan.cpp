#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.hpp>
#include <pcl/filters/filter.h>
#include <sdc_msgs/msg/coordinate_list.hpp>
#include <sdc_msgs/msg/coordinate.hpp>
#include <sdc_msgs/msg/arr.hpp>
#include <sdc_msgs/msg/arrofarr.hpp>
#include <cmath>
#include <unordered_map>
#include <vector>

using namespace std;

#define heightlidar 0.8
#define ringlength 0.2
#define planespreadbeforering 30
#define lidarframe "rslidar"

#define xthresh 3

// ConeCheck
#define MaxHeight 0.4
#define MinHeight 0.05
// #define MinHeight 0.1 //0.2
// #define MaxPoints 2000
#define MinPoints 1
// #define MaxLen 1 //0.7
#define MaxWidth 0.6
#define MaxLen 0.5

float sectorangle  = M_PI/16;

// Thresholds
float EuclideanThreshold =  0.1;
float CentroidThreshold = 0.3;

std::unordered_map<int, std::vector<float>> ground;
pcl::PointXYZI lidarpoint;

class CustomCluster{
public:
    int clustersize = 0;
    pcl::PointXYZI Avg;
    pcl::PointXYZI Last;
    pcl::PointXYZI Left;
    pcl::PointXYZI Right;
    pcl::PointXYZI Front;
    pcl::PointXYZI Back;
    pcl::PointXYZI minheight;
    pcl::PointXYZI maxheight;

    CustomCluster(pcl::PointXYZI Point){
        Avg = Point;
        Last = Point;
        Left = Point;
        Right = Point;
        Front = Point;
        Back = Point;
        clustersize = 1;
        minheight = maxheight = Point;
    }
};

bool check_distance(const pcl::PointXYZI& Cluster_Point, const pcl::PointXYZI& point, float Threshold){
    float distance_sq =
        (Cluster_Point.x - point.x) * (Cluster_Point.x - point.x) +
        (Cluster_Point.y - point.y) * (Cluster_Point.y - point.y) +
        (Cluster_Point.z - point.z) * (Cluster_Point.z - point.z);

    return distance_sq < (Threshold * Threshold);
}
class Clustering : public rclcpp::Node
{
public:
    Clustering(): Node("boundaryscan")
    {
        lidarpoint.x = 0.f; lidarpoint.y = 0.f; lidarpoint.z = -heightlidar;
        Clusters = this->create_publisher<sdc_msgs::msg::CoordinateList>("Clusters", 1000);
        Clusters_pc = this->create_publisher<sensor_msgs::msg::PointCloud2>("Clusters_PointCloud", 1000);
        Surrounding_pc = this->create_publisher<sensor_msgs::msg::PointCloud2>("Surrounding_PointCloud", 1000);

        // subground = this->create_subscription<sdc_msgs::msg::Arrofarr>(
        //     "ground", 1, std::bind(&Clustering::callbackground, this, std::placeholders::_1));
        sub = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "nonground", 1, std::bind(&Clustering::callback, this, std::placeholders::_1));
    }

    void callbackground(const sdc_msgs::msg::Arrofarr::SharedPtr inputplanes){
        ground.clear();
        for(auto &i : inputplanes->data){
            ground[int(std::round(i.data[0]))] = {i.data[1], i.data[2], i.data[3], i.data[4]};
        }
    }

    void callback(const sensor_msgs::msg::PointCloud2::SharedPtr input){   
        // if (ground.size()==0) return;
        // if(line.size()!=0){ 
        sdc_msgs::msg::CoordinateList Cluster;
        Cluster.header.stamp = this->now();
        Cluster.header.frame_id = lidarframe;
        Cluster.size = 0;
        std::vector<CustomCluster> Clusters_Vector;
        CustomCluster StdCluster(pcl::PointXYZI(0.f));
        Clusters_Vector.push_back(StdCluster);

        pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);
        pcl::fromROSMsg (*input, *cloud);

        if (cloud->size() != 0) {
            pcl::PointCloud<pcl::PointXYZI>::Ptr cluster_pc (new pcl::PointCloud<pcl::PointXYZI>);
            cluster_pc->header.frame_id = lidarframe;
            pcl::PointCloud<pcl::PointXYZI>::Ptr surr_cloud (new pcl::PointCloud<pcl::PointXYZI>);
            surr_cloud->header.frame_id = lidarframe;

            for (auto &point : cloud->points){
                if (std::isfinite(point.x) && std::isfinite(point.y) && std::isfinite(point.z)){
                    // if ((point.x > xthresh) || (point.x < 1)) continue;
                    int found_cluster = 0;
                    for (size_t index = 0; index < Clusters_Vector.size(); index++){
                        CustomCluster &Iter_Cluster = Clusters_Vector[index];
                        if (
                            check_distance(Iter_Cluster.Last, point, EuclideanThreshold) ||
                            check_distance(Iter_Cluster.Avg, point, CentroidThreshold) ||
                            check_distance(Iter_Cluster.Left, point, EuclideanThreshold) ||
                            check_distance(Iter_Cluster.Right, point, EuclideanThreshold) ||
                            check_distance(Iter_Cluster.minheight, point, EuclideanThreshold) ||
                            check_distance(Iter_Cluster.maxheight, point, EuclideanThreshold) ||
                            check_distance(Iter_Cluster.Front, point, EuclideanThreshold) ||
                            check_distance(Iter_Cluster.Back, point, EuclideanThreshold)) {

                            int current_size = (Iter_Cluster.clustersize++);
                            double inv_current_size = 1.0 / (current_size + 1);

                            Iter_Cluster.Last = point;
                            Iter_Cluster.Left =  (Iter_Cluster.Left.y < point.y) ? point : Iter_Cluster.Left ;
                            Iter_Cluster.Right = (Iter_Cluster.Right.y > point.y) ? point : Iter_Cluster.Right ;
                            Iter_Cluster.Front =  (Iter_Cluster.Front.x > point.x) ? point: Iter_Cluster.Front ;
                            Iter_Cluster.Back = (Iter_Cluster.Back.x < point.x) ? point: Iter_Cluster.Back ;
                            Iter_Cluster.minheight =  (Iter_Cluster.minheight.z > point.z) ? point: Iter_Cluster.minheight ;
                            Iter_Cluster.maxheight = (Iter_Cluster.maxheight.z < point.z) ? point: Iter_Cluster.maxheight ;

                            Iter_Cluster.Avg.x = (1 - inv_current_size)*Iter_Cluster.Avg.x + (point.x*inv_current_size);
                            Iter_Cluster.Avg.y = (1 - inv_current_size)*Iter_Cluster.Avg.y + (point.y*inv_current_size);
                            Iter_Cluster.Avg.z = (1 - inv_current_size)*Iter_Cluster.Avg.z + (point.z*inv_current_size);

                            found_cluster++;

                            // if (Iter_Cluster.minheight.z > point.z){
                            //     Clusters_Vector[index].minheight = point;
                            // }
                            // if (Iter_Cluster.maxheight.z < point.z){
                            //     Clusters_Vector[index].maxheight = point;
                            // }

                            surr_cloud->push_back(point);
                            break;
                        }
                    }
                    if (!found_cluster){
                        CustomCluster NewCluster(point);
                        Clusters_Vector.push_back(NewCluster);
                    }
                }
            }

            // Convert and publish Surrounding point cloud
            pcl::PCLPointCloud2 pcl_surr;
            pcl::toPCLPointCloud2(*surr_cloud, pcl_surr);
            sensor_msgs::msg::PointCloud2 ros_surr_msg;
            pcl_conversions::fromPCL(pcl_surr, ros_surr_msg);
            ros_surr_msg.header.frame_id = lidarframe;
            ros_surr_msg.header.stamp = this->now();
            Surrounding_pc->publish(ros_surr_msg);

            cluster_pc->width = Clusters_Vector.size();
            cluster_pc->height = 1;
            // cluster_pc->push_back(pcl::PointXYZI(0.f));
            float denominator = 1;

            RCLCPP_INFO(this->get_logger(), "Clusters count: %ld", Clusters_Vector.size());
            for (size_t index = 0; index < Clusters_Vector.size(); index++){
                CustomCluster &Iter_Cluster = Clusters_Vector[index];
                if (Iter_Cluster.Avg.x == 0.0 && Iter_Cluster.Avg.y == 0.0 && Iter_Cluster.Avg.z == 0.0 ) continue;
                float dist_sq = std::pow(Iter_Cluster.Avg.x, 2.0) + std::pow(Iter_Cluster.Avg.y, 2.0) + std::pow(Iter_Cluster.Avg.z, 2.0);
                float expected_points = 2000.0f / (dist_sq + 1.0f); //remove dist =0
                if (!(Iter_Cluster.Avg.x == 0 && Iter_Cluster.Avg.y == 0)){
                    float ratio = Iter_Cluster.Avg.y / std::abs(Iter_Cluster.Avg.x);
                    if ((ratio > std::tan(10 * M_PI/180) && ratio < std::tan(15 * M_PI/180)) ||
                        (ratio > std::tan(30 * M_PI/180) && ratio < std::tan(40 * M_PI/180))){
                        expected_points *= 2;
                    }
                }
                
                // checking cone distance from lu planes
                // int ringclus = pow(pow(Iter_Cluster.Avg.x, 2)+pow(Iter_Cluster.Avg.y, 2), 0.5)/ringlength;
                // float angleclus = atan2(Iter_Cluster.Avg.x, -Iter_Cluster.Avg.y) - M_PI/6;
                // int key = int((angleclus)/(sectorangle)) + int((2*M_PI*ringclus)/(3*sectorangle));
                //
                // denominator = pow(pow(ground[key][0], 2) + pow(ground[key][1], 2) + pow(ground[key][2], 2), 0.5);
                // float distance = abs(Iter_Cluster.Avg.x*ground[key][0] + Iter_Cluster.Avg.y*ground[key][1] + Iter_Cluster.Avg.z*ground[key][2] + ground[key][3])/denominator;
                // float minzfromground = abs(Iter_Cluster.minheight.x*ground[key][0] + Iter_Cluster.minheight.y*ground[key][1] + Iter_Cluster.minheight.z*ground[key][2] + ground[key][3])/denominator;
                // float maxzfromground = abs(Iter_Cluster.maxheight.x*ground[key][0] + Iter_Cluster.maxheight.y*ground[key][1] + Iter_Cluster.maxheight.z*ground[key][2] + ground[key][3])/denominator;

                if (
                    1
                    // && (distance >=0.04 && distance < 0.32 )
                    // && (Iter_Cluster.Avg.z + heightlidar < 1.5)
                    // // && (Iter_Cluster.Avg.z + heightlidar < 0.2)
                    // // && (Iter_Cluster.Avg.z + heightlidar > MinHeight)
                    // && (maxzfromground >= 0.1)
                    // && (minzfromground <= 0.22 )
                    // && (Iter_Cluster.clustersize < 1000)
                    // // && (Iter_Cluster.clustersize > 0.2 * expected_points)
                    // && ((Iter_Cluster.Right.y - Iter_Cluster.Left.y)*(Iter_Cluster.Right.y - Iter_Cluster.Left.y) + (Iter_Cluster.Right.x - Iter_Cluster.Left.x)*(Iter_Cluster.Right.x - Iter_Cluster.Left.x) < MaxWidth*MaxWidth)
                    // && ((Iter_Cluster.Back.y - Iter_Cluster.Front.y)*(Iter_Cluster.Back.y - Iter_Cluster.Front.y) + (Iter_Cluster.Back.x - Iter_Cluster.Front.x)*(Iter_Cluster.Back.x - Iter_Cluster.Front.x) < MaxLen*MaxLen)
                    && (Iter_Cluster.clustersize > MinPoints)
                ){
                    // RCLCPP_INFO(this->get_logger(), "%f %d", expected_points, Iter_Cluster.clustersize);
                    Cluster.size++;
                    sdc_msgs::msg::Coordinate CurrentCluster;
                    CurrentCluster.size = Iter_Cluster.clustersize;
                    CurrentCluster.reconsize = 0;
                    CurrentCluster.x = Iter_Cluster.Avg.x;
                    CurrentCluster.y = Iter_Cluster.Avg.y;
                    CurrentCluster.z = Iter_Cluster.Avg.z;
                    CurrentCluster.left = {Iter_Cluster.Left.x, Iter_Cluster.Left.y, Iter_Cluster.Left.z};
                    CurrentCluster.right = {Iter_Cluster.Right.x, Iter_Cluster.Right.y, Iter_Cluster.Right.z};
                    CurrentCluster.front = {Iter_Cluster.Front.x, Iter_Cluster.Front.y, Iter_Cluster.Front.z};
                    CurrentCluster.back = {Iter_Cluster.Back.x, Iter_Cluster.Back.y, Iter_Cluster.Back.z};
                    CurrentCluster.top = {Iter_Cluster.maxheight.x, Iter_Cluster.maxheight.y, Iter_Cluster.maxheight.z};
                    CurrentCluster.bottom = {Iter_Cluster.minheight.x, Iter_Cluster.minheight.y, Iter_Cluster.minheight.z};
                    RCLCPP_INFO(this->get_logger(), "Cone %d %f %f %f", Iter_Cluster.clustersize, Iter_Cluster.Avg.x, Iter_Cluster.Avg.y, Iter_Cluster.Avg.z);
                    Cluster.cone_coordinates.push_back(CurrentCluster);
                    cluster_pc->push_back(Iter_Cluster.Avg);
                }
            }
            cluster_pc->push_back(pcl::PointXYZI(0.f));

            // type conversion and publishing
            pcl::PCLPointCloud2 pcl_pc2;
            pcl::toPCLPointCloud2(*cluster_pc, pcl_pc2);
            sensor_msgs::msg::PointCloud2 cluster_pc_msg;
            pcl_conversions::fromPCL(pcl_pc2, cluster_pc_msg);
            cluster_pc_msg.header.frame_id = lidarframe;
            cluster_pc_msg.header.stamp = this->now();
            Clusters_pc->publish(cluster_pc_msg);
        }
        else{
            RCLCPP_INFO(this->get_logger(), "No object found");
        }
        Clusters->publish(Cluster);
        RCLCPP_INFO(this->get_logger(), "Publishing Cone Coordinates");
    }

private:
    rclcpp::Publisher<sdc_msgs::msg::CoordinateList>::SharedPtr Clusters;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr Clusters_pc;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr Surrounding_pc;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub;
    rclcpp::Subscription<sdc_msgs::msg::Arrofarr>::SharedPtr subground;
};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Clustering>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
