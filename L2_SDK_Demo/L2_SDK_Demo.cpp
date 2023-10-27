// L2_SDK_Demo.cpp: 定义应用程序的入口点。
//

#include "L2_SDK_Demo.h"
#include "l2_sdk.h"
#include <thread>
#include<fstream>
#include<stdio.h>
#include <string.h>
//#include <opencv2/opencv.hpp>

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

	printf("Please select COM:\n");
	int port = 0;
	std::cin >> port;

	std::string strPort = "//./COM" + std::to_string(port);

#if  __linux__
	strPort = "/dev/ttyUSB" + std::to_string(port);
#endif

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
        if (iRes < 0)
        {
            std::cout << "升级失败：" << iRes << endl;
        }
        else
        {
            std::cout << "升级成功！" << endl;
        }

        g_bEnable = true;

        while (1)
        {
            delay(10);
        }
    }
    else if(type == 1){
        //----------------------------------------------
        //如果当前为图像模式
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

        int nTestCount = 1;
        while (nTestCount) {
            std::vector<stOutputPoint> data;
            apiGetPointData(data);
            if (data.size() > 0) {
                savedata(data);
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
    }
    else if (3 == type)
    {
        apiStopScan();
        delay(1400);
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

        printf("Please select addr:\n");
	    int addr = 1;
        std::cin >> addr;
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
        while (1)
        {
            std::string strType = info.at(addr - 1).productName;
            std::string strFileName = std::to_string(Index) + ".bmp";
            stImgData imgData;
            if(apiGetImgData(imgData)){
                 printf("get img data: %d.bmp\n",Index);
                saveImgData(imgData.data, strFileName.c_str(), strType);

                 //cv::Mat imgTemp(160, 160, CV_8UC1);
                //memcpy(imgTemp.data,imgData.data , 160* 160 );
               // cv::imwrite(std::to_string(Index) +"_0cv.bmp" , imgTemp); 
               
                Index++;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::this_thread::yield();
        }
        
    }
    

	cout << "exit out!" << endl;
	delay(10);
	apiSDKUninit();
   
	return 0;
}
