#include "RosPublisher.hpp"

using namespace std;

RosPublisher::RosPublisher(lcm::LCM* lcmer): lcm(lcmer)
{
    lcm->subscribe("scan", &RosPublisher::laserScanCallback, this) ;
    lcm->subscribe("PointCloud", &RosPublisher::PointCloudCallback, this) ;
    lcm->subscribe("Image", &RosPublisher::ImageCallback, this) ;

    publisher.scan_pub = nh.advertise<sensor_msgs::LaserScan>("scan", 500);
    publisher.image_pub =nh.advertise<sensor_msgs::Image>("image",1000);
	publisher.point_cloud_pub = nh.advertise<sensor_msgs::PointCloud>("point_cloud",1000);
}





std::string RosPublisher::getLcmUrl(int64_t ttl) {
  assert(ttl >= 0 && ttl <= 255);
  return "udpm://239.255.76.67:7667?ttl=" + std::to_string(ttl);
}

void RosPublisher::Time(const lcm_std_msgs::Time& t, ros::Time& r)
{
        uint64_t sec_part = static_cast<uint64_t>(t.sec);
        uint64_t nsec_part = static_cast<uint64_t>(t.nsec);
        sec_part += nsec_part / 1000000000UL;
        nsec_part = nsec_part % 1000000000UL;
        if(r.sec >= std::numeric_limits<uint32_t>::max())
        {
            sec_part = 0;
            nsec_part = 0;
            std::cout<< "time error: " << t.sec << ", " << t.nsec << std::endl;
        }
        r.sec = static_cast<uint32_t>(sec_part);
        r.nsec = static_cast<uint32_t>(nsec_part);
}

void RosPublisher::Header(const lcm_std_msgs::Header& h, std_msgs::Header& r)
{

    r.seq = h.seq;
    Time(h.stamp,r.stamp);
    r.frame_id = h.frame_id;
}


void RosPublisher::laserScanCallback(const lcm_sensor_msgs::LaserScan& scan)
{
    sensor_msgs::LaserScan laser_scan;
    
    Header(scan.header,laser_scan.header);
    laser_scan.angle_min = scan.angle_min;
    laser_scan.angle_max = scan.angle_max;
    laser_scan.angle_increment = scan.angle_increment;
    laser_scan.time_increment = scan.time_increment;
    laser_scan.scan_time = scan.scan_time;
    laser_scan.range_min = scan.range_min;
    laser_scan.range_max = scan.range_max;
    laser_scan.ranges = scan.ranges;
    laser_scan.intensities.resize(scan.intensities.size());
    for(size_t i = 0, iend = laser_scan.intensities.size(); i < iend; i++){
        laser_scan.intensities[i] = scan.intensities[i];
    }
    publisher.scan_pub.publish(laser_scan);
    	
}

void RosPublisher::PointCloudCallback(const lcm_sensor_msgs::PointCloud& lcm_point_cloud)
{
    sensor_msgs::PointCloud point_cloud;
    Header(lcm_point_cloud.header,point_cloud.header);
    point_cloud.points.resize(lcm_point_cloud.points.size());
    memcpy(point_cloud.points.data(),lcm_point_cloud.points.data(),lcm_point_cloud.points.size()*12);
     publisher.point_cloud_pub.publish(point_cloud);
}


void RosPublisher::ImageCallback(const lcm_sensor_msgs::Image& lcm_image)
{
    sensor_msgs::Image image;

    Header(lcm_image.header,image.header);
    image.height = lcm_image.height;
    image.width = lcm_image.width;
    image.encoding = lcm_image.encoding;
    image.is_bigendian = lcm_image.is_bigendian;
    image.step = lcm_image.step;
    image.data.resize(lcm_image.n_data);
    memcpy(image.data.data(),lcm_image.data.data(),lcm_image.n_data);
    publisher.image_pub.publish( image);
}

void RosPublisher::laserScanCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
                           const lcm_sensor_msgs::LaserScan * msgs)
{

    laserScanCallback(*msgs);
}

void RosPublisher::PointCloudCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
                           const  lcm_sensor_msgs::PointCloud * msgs)
{
    PointCloudCallback(*msgs);
 }


void RosPublisher::ImageCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
                           const lcm_sensor_msgs::Image * msgs)
{
    ImageCallback(*msgs);
}

