#ifndef _RosPublisher_HPP_
#define _RosPublisher_HPP_

#include <string>
#include <assert.h>
#include <ros/ros.h>
#include <lcm/lcm-cpp.hpp>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/Image.h>

#include <lcm_std_msgs/LaserScan.hpp>
#include <Image.hpp>
#include <PointCloud.hpp>

struct Publisher
{
  
    ros::Publisher scan_pub;
    ros::Publisher image_pub;
    ros::Publisher point_cloud_pub;
};

class RosPublisher
{
private:
    ros::NodeHandle nh;
    lcm::LCM* lcm; 
public:
    explicit RosPublisher(lcm::LCM* lcmer);
    static std::string getLcmUrl(int64_t ttl);
    void Time(const lcm_std_msgs::Time& t, ros::Time& r);
    void Header(const lcm_std_msgs::Header& h, std_msgs::Header& r);
    void laserScanCallback(const lcm_sensor_msgs::LaserScan& scan);
    void PointCloudCallback(const lcm_sensor_msgs::PointCloud& lcm_point_cloud);
    void ImageCallback(const lcm_sensor_msgs::Image& lcm_image);
    void laserScanCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
                           const lcm_sensor_msgs::LaserScan * msgs);
    void PointCloudCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
                           const  lcm_sensor_msgs::PointCloud * msgs);
    void ImageCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
                           const lcm_sensor_msgs::Image * msgs);

    Publisher publisher;
};

#endif
