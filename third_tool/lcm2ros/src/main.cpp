#include <iostream>
#include <string>
#include "RosPublisher.hpp"
#include "lcm_std_msgs/Int8.hpp"

using namespace std;

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "./lcm_to_ros [ip]\n";
        return -1;
    }

    ros::init(argc, argv, "lcm_to_ros");

    string addr = RosPublisher::getLcmUrl(255);
    cout << "listen to " << addr << endl;
    lcm::LCM lcm(addr);
    if(!lcm.good())
        return 1;

    lcm_std_msgs::Int8   start_mode;
    if(argc > 2 &&std::atoi(argv[2]) == 1){
            uint8_t addr = (std::atoi(argv[3])  << 4 ) & 0xF0;
            start_mode.data=1+addr;
            lcm.publish("SartCloudPoint", &start_mode);
            printf("SartCloudPoint: %d\n", start_mode.data );
    }
    else if(argc > 2 &&std::atoi(argv[2]) == 3){
            uint8_t addr =  (std::atoi(argv[3])  << 4 ) & 0xF0;
            start_mode.data=3 + addr;
            lcm.publish("SartImg", &start_mode);
            printf("SartImg: %d\n", start_mode.data );
    }


    RosPublisher handlerObject(&lcm);

    const int lcm_timeout = 100; // ms
    while((lcm.handleTimeout(lcm_timeout) >= 0) && (ros::ok()))
        ros::spinOnce();

    return 0;
}
