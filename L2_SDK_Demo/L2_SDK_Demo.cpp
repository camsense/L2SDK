// L2_SDK_Demo.cpp: 定义应用程序的入口点。
//

#include "L2_SDK_Demo.h"
#include <thread>
#include<fstream>
#include<stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "PointCloud.hpp"
#include "lcm_std_msgs/lcm_utils.h"
#include "lcm_std_msgs/Point32.hpp"
#include "lcm_std_msgs/Int8.hpp"
#include "lcm/lcm-cpp.hpp"

#ifdef __linux__
#include <unistd.h>
#else
#include <windows.h>
#endif

using namespace std;


static void delay(UINT32 ms) {
#ifdef __linux__
	if(ms != 0){usleep(ms * 1000);}
#else 
	Sleep(ms);
#endif
}


//数据保存
static void savedata(std::vector<stOutputPoint>& data)
{
    for (auto it : data)
    {
        std::string file = "addr" + std::to_string(it.uaddr) + ".csv";
        std::ofstream in(file, std::ios::app);
        std::vector<POINTDATA> vcPoint;
        vcPoint.swap(it.Point);
		UINT64 time = it.u64LocTimeStampS;
        for (auto p : vcPoint)
        {
            int quality = p.quality;
            int row = p.row;
            in << it.u64LocTimeStampS << "," << it.u64DeviceTimeStampS << "," << it.u64ExposureTimeStamp <<","<<p.bflag << "," << p.bfiter << ","
                << p.x << "," << p.y << "," << quality << ","<< row << endl;
        }
        in.close();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

//获取升级进度
static float g_fPercentage = 0x00;
static bool g_bEnable = false;
void showProcess(float temp ) {
    while (!g_bEnable)
    {
        float temp = 0;
        apiGetUpgradeProgress(temp);
        if (g_fPercentage != temp)
        {
            std::cout << "升级进度:" << temp <<"%" << endl;
            g_fPercentage = temp;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
   
}



//切换成点云模式
void switch_mode(lcm::LCM* lcm){
    EchoTest handler;
    lcm->subscribe("SartCloudPoint", &EchoTest::L2SartCallback, &handler) ;
    lcm->subscribe("SartImg", &EchoTest::L2SartCallback, &handler) ;
    while(0 == lcm->handle());   
}

int main()
{
	cout << "camsense L2 sdk" << endl;
    cout << "-------------------------------------" << endl;
    cout << " 1、启动设备 " << endl;
    cout << " 2、升级设备 " << endl;
    cout << " 3、切换图像模式,并输出图像 " << endl;
    cout << "-------------------------------------" << endl;
    cout << " 请输入: " << endl;
    int type = 1;
    std::cin >> type;

	std::string strVersion = apiGetVersion();
	cout << "sdk version: "<< strVersion << endl;

    cout << "请输入设备串口号: " << endl;
	int port = 0;
	std::cin >> port;

	std::string strPort = "/dev/ttyUSB" + std::to_string(port);

    EchoTest handler;
    lcm::LCM lcm(handler.getLcmUrl(255));
    if(!lcm.good())
    {
        return 1;
    }
    handler.m_lcm = lcm;
    handler.setPort(strPort);

	if (!apiSDKInit(strPort.c_str(), 921600))
	{
		cout << "camsense L2 sdk init fail!" << endl;
		apiSDKUninit();
		return 0;
	}
	cout << "camsense L2 sdk init fanish!" << endl;

    if (type == 2) {
        g_bEnable = false;
        std::string strPath;
        int addr = 0x00;
        std::cout << "请输入升级文件路径+文件名:" << endl;
        std::cin >> strPath;
        std::cout << "请输入升级设备的地址:" << endl;
        std::cin >> addr;
        float temp = 0;
        std::thread thProcess(showProcess, temp);
        thProcess.detach();

        int iRes = apiUpgradeBin(strPath.c_str(), addr);
        if (iRes < 0) {
            std::cout << "升级失败：" << iRes << endl;
        }
        else{
            std::cout << "升级成功！" << endl;
        }

        g_bEnable = true;

        while (1)
        {
            delay(10);
        }
    }
    else if(type == 1){
        handler.setMode(1);
        handler.run(type);
    }
    else if (3 == type){
        handler.setMode(3);
        handler.run(type);
    }

    lcm.subscribe("SartCloudPoint", &EchoTest::L2SartCallback, &handler) ;
    lcm.subscribe("SartImg", &EchoTest::L2SartCallback, &handler) ;
    while(0 == lcm.handle());   
    
	cout << "exit out!" << endl;
	delay(10);
	apiSDKUninit();
   
	return 0;
}

int EchoTest::point_mode()
{
       //----------------------------------------------
    //如果当前为图像模式,需要处理
    apiStopScan();
    delay(1400);
    //----------------------------------------------

    int nAddr = apiGetDeviceAddr();
    if (nAddr == 0) {
        cout << "apiGetDeviceAddr fail!" << endl;
        apiSDKUninit();
        return 0;
    }
    cout << "max addr:" << nAddr << endl;

    for (int i = 1; i <= nAddr; i++) {
        std::string file = "addr" + std::to_string(i) + ".csv";
        std::ofstream in(file);
        in << "loctime,devicetime,Exptime,是否有效,是否滤波,x,y,亮度信息,row" << endl;
        in.close();
    }

    std::vector<DeviceInfo> info;
    if (!apiGetDeviceInfo(info))
    {
        cout << "get device info failed" << endl;
        return 0;
    }

    for (auto it : info) {
        printf("add:%d, factoryInfo:%s,firmwareVersion:%s, productName:%s, SN:%s\r\n",
            it.addr, it.factoryInfo.c_str(), it.firmwareVersion.c_str(), it.productName.c_str(), it.deviceSN.c_str());
    }

    cout << "get device info  fanish!" << endl;
    delay(15);

    if (!apiStartScan()) {
        cout << "device scan failed!" << endl;
        apiSDKUninit();
        return 0;
    }
    cout << "device scan fanish!" << endl;

    while (!m_bExit) {
        std::vector<stOutputPoint> data;
        apiGetPointData(data);
        if (data.size() > 0) {
            savedata(data);
            for(int i = 0;i < data.size();i++)
            {   
                lcm_sensor_msgs::PointCloud point_cloud_msg;
                struct timespec tp;
                clock_gettime(CLOCK_REALTIME, &tp);
                point_cloud_msg.header.frame_id = "map";
                point_cloud_msg.header.stamp.sec = tp.tv_sec;
                point_cloud_msg.header.stamp.nsec = tp.tv_nsec;
                for(int j = 0;j < data[i].Point.size();j++)
                {
                    if(data[i].Point[j].bflag){
                        lcm_geometry_msgs::Point32 point;
                        point.x = data[i].Point[j].x / 1000.0;
                        point.y = data[i].Point[j].y / 1000.0;
                            point.z = 0;
                        point_cloud_msg.points.push_back(point);
                    }
                }

                if (data[i].uaddr == m_iCurAddr){
                    point_cloud_msg.n_points = point_cloud_msg.points.size();
                    m_lcm.publish("PointCloud", &point_cloud_msg);
                }
            }
         
            float fps = 0;
            apiGetDeviceFps(fps);
            std::cout << "fps: " << fps << endl;
        }
        else {
            ErrorCode error;
            apiGetErrorCode(error);
            if (error != IDLE)
            {
                cout << "error: " << error << endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::this_thread::yield();
    };
    return 0;
}

int EchoTest::img_mode()
{
    apiStopScan();
    delay(1400);
    //delay(2400);
    int nAddr = apiGetDeviceAddr();
    if (nAddr == 0) {
        cout << "apiGetDeviceAddr fail!" << endl;
        apiSDKUninit();
        return 0;
    }
    cout << "max addr:" << nAddr << endl;

    std::vector<DeviceInfo> info;
    if (!apiGetDeviceInfo(info))
    {
        cout << "get device info failed" << endl;
        return 0;
    }

    for (auto it : info) {
        printf("add:%d, factoryInfo:%s,firmwareVersion:%s, productName:%s, SN:%s\r\n",
            it.addr, it.factoryInfo.c_str(), it.firmwareVersion.c_str(), it.productName.c_str(), it.deviceSN.c_str());
    }

    cout << "get device info  fanish!" << endl;
    delay(15);
    int addr = m_iCurAddr;
    if (addr > nAddr || addr <= 0) 
    {
        /* code */
        printf("input  addr failed!\n");
        return 0;
    }
    
    if (!apiSwitchImgMode(addr))
    {
        printf("apiSwitchImgMode failed!\n");
        apiSDKUninit();
        return 0;
    }
        
    int Index = 0;
    while (!m_bExit)
    {
        std::string strType = info.at(addr - 1).productName;
        std::string strFileName = "addr"+std::to_string(addr) + "_"+  std::to_string(Index) + ".bmp";
        stImgData imgData;
        if(apiGetImgData(imgData)){
            printf("get img data: %d.bmp\n",Index);
            saveImgData(imgData.data, strFileName.c_str(), strType);

             //调试下开启图像输出发送到可视化界面
            int cols = 160, rows = 160;
            if (strType != "L2C2")
            {
                cols = 160;
                rows = 128;
            }

            lcm_sensor_msgs::Image rgb_lcm_msg;
            struct timespec tp;
            clock_gettime(CLOCK_REALTIME, &tp);
            rgb_lcm_msg.header.stamp.sec = tp.tv_sec;
            rgb_lcm_msg.header.stamp.nsec = tp.tv_nsec;
            rgb_lcm_msg.height = rows;
            rgb_lcm_msg.width = cols;
            rgb_lcm_msg.encoding = "8UC1";
            rgb_lcm_msg.is_bigendian = 0;

            rgb_lcm_msg.step = cols;
            rgb_lcm_msg.n_data = cols * rows;
            std::vector< uint8_t > vcData;
            vcData.resize(cols * rows);
            memcpy(vcData.data(), imgData.data, cols * rows);
            rgb_lcm_msg.data.swap(vcData);
            m_lcm.publish("Image", &rgb_lcm_msg);
            Index++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::this_thread::yield();
    };
    return 0;
}
