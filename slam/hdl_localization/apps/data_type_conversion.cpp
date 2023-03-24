#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
// PCL specific includes
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>
ros::Publisher pub;

struct RSPOINTXYZI{
  PCL_ADD_POINT4D;
  
  uint8_t intensity;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
} EIGEN_ALIGN16;
POINT_CLOUD_REGISTER_POINT_STRUCT(RSPOINTXYZI,(float,x,x)(float,y,y)(float,z,z)(uint8_t,intensity, intensity))


void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& input)
{
  // ... do data processing
    
        pcl::PointCloud<pcl::PointXYZI> cloud_init0;
        pcl::PointCloud<RSPOINTXYZI> my_cloud;
        pcl::PointXYZI point_xyzi;
        
        pcl::fromROSMsg(*input, my_cloud);


        for(unsigned int i =0;i < my_cloud.points.size(); i++)
        {

        point_xyzi.x = my_cloud.points[i].x;
        point_xyzi.y = my_cloud.points[i].y;
        point_xyzi.z = my_cloud.points[i].z;
        point_xyzi.intensity = float(my_cloud.points[i].intensity);
        cloud_init0.push_back(point_xyzi);
        }
    
        cloud_init0.header=my_cloud.header;
        cloud_init0.header.frame_id=my_cloud.header.frame_id;
        

  sensor_msgs::PointCloud2 output;
  pcl::toROSMsg(cloud_init0, output);
  // Publish the data
  pub.publish (output);
}

int
main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("os32_top/point_cloud", 1, cloud_cb);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("os32_top/point_cloud_conversion", 1);

  // Spin
  ros::spin ();
}

