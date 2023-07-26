// L2_SDK_Demo.cpp: 定义应用程序的入口点。
//

#include "L2_SDK_Demo.h"
#include "l2_sdk.h"
#include <thread>
#include<fstream>

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
		UINT64 time = it.u64TimeStampS;
        for (auto p : vcPoint)
        {
            in << time <<","<<p.bflag << "," << p.x << "," << p.y << endl;
        }
        in.close();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

int main()
{
	cout << "camsense L2 sdk" << endl;
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

	int nAddr = apiGetDeviceAddr();

	if(nAddr  == 0){
		cout << "apiGetDeviceAddr fail!" << endl;
		apiSDKUninit();
		return 0;
	}
		cout << "max addr:" << nAddr << endl;

	for(int i = 1; i <= nAddr; i++){
		std::string file = "addr" + std::to_string(i) + ".csv";
        std::ofstream in(file);
		in << "time,是否有效,x,y"<<endl;
		in.close();
	}
	    
	std::vector<DeviceInfo> info;
	if (!apiGetDeviceInfo(info))
	{
		cout << "get device info failed"  << endl;
		return 0;
	}
	for(auto it:info){
		printf("add:%d, factoryInfo:%s,firmwareVersion:%s, productName:%s, id:%s\r\n",
		it.addr, it.factoryInfo.c_str(), it.firmwareVersion.c_str(),it.productName.c_str(), it.deviceID.c_str());
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
	while (nTestCount++ < 10000){
		std::vector<stOutputPoint> data;
		apiGetPointData(data);
		if(data.size() >0 ){
			savedata(data);
			float fps  = 0;
			apiGetDeviceFps(fps);
			std::cout << "fps: " << fps <<endl;
		}
		else{
			ErrorCode error;
			apiGetErrorCode(error);
			if (error != IDLE )
			{
				cout << "error: " << error << endl;
			}
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::yield();
	};

	apiStopScan();
	delay(2000);
	apiStartScan();
	nTestCount = 0;
	while (nTestCount++ <= 10000){
		std::vector<stOutputPoint> data;
		apiGetPointData(data);
		if(data.size() >0 ){
			savedata(data);
			float fps  = 0;
			apiGetDeviceFps(fps);
			std::cout << "fps: " << fps <<endl;
		}
		else{
			ErrorCode error;
			apiGetErrorCode(error);
			if (error != IDLE )
			{
				cout << "error: " << error << endl;
			}
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::yield();
	};
	

	cout << "exit out!" << endl;
	delay(10);
	apiSDKUninit();
   
	return 0;
}
