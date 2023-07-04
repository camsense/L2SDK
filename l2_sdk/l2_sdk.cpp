// l2_sdk.cpp: 目标的源文件。
//

#include "l2_sdk.h"
#include "L2DeviceHandle.h"


#ifdef __cplusplus
extern "C" {
#endif

	/*************************************************
	@Function: 				apiSDKInit()
	@Description: 			L2 SDK初始化，初始化成功才能使用其它API.
	@Input:					chPort - 串口号   iBaud - 串口波特率 (仅支持921600)
	@Output:  				nothing
	@Return: 				false表示失败, true表示成功
	@Others:
	*************************************************/
	bool apiSDKInit(const char* chPort, int iBaud) {

		return CL2DeviceHandle::Instance().init(chPort, iBaud);
	}


	/*************************************************
	@Function: 				apiSDKUninit()
	@Description: 			L2 SDK释放
	@Input:					nothing
	@Output:  				nothing
	@Return: 				false表示失败, true表示成功
	@Others:
	*************************************************/
	bool apiSDKUninit() {
		return  CL2DeviceHandle::Instance().uinit();
	}

	/*************************************************
	@Function: 				apiGetVersion()
	@Description: 			获取SDK版本号
	@Input:					nothing
	@Output:  				nothing
	@Return: 				SDK version
	@Others:
	*************************************************/
	char* apiGetVersion() {
		return CL2DeviceHandle::Instance().getSDKVersion();
	}

	/*************************************************
	@Function: 				apiGetDeviceInfo()
	@Description: 			获取所有设备信息
	@Input:					nothing
	@Output:  				Info
							typedef struct _deviceInfo{
								UINT8 addr;                                      //设备地址
								std::string factoryInfo;                         //厂家信息
								std::string productName;                         //产品名称    
								std::string firmwareVersion;                     //固件版本号 
								std::string deviceID;                            //设备ID 号 
							}DeviceInfo;
	
	@Return: 				false表示失败, true表示成功
	@Others:
	*************************************************/
	bool apiGetDeviceInfo(std::vector<DeviceInfo>& Info) {
		
		return CL2DeviceHandle::Instance().getInfo(Info);
	}

	/*************************************************
	@Function: 				apiGetDeviceAddr()
	@Description: 			获取设备的地址
	@Input:					nothing
	@Output:  				nothing
	@Return: 				设备地址：	0x01表示设备地址只有一个设备，
										0x02 表示2个设备级联地址分别为0x01\0x02,
										0x03表示3个设备级联，地址分别是0x01\0x02\0x03
	@Others:				最多级联三个设备
	*************************************************/
	UINT8 apiGetDeviceAddr() {
		return CL2DeviceHandle::Instance().getNumberOfDevice();
	}

	/*************************************************
	@Function: 				apiStartScan()
	@Description: 			开始扫描
	@Input:					nothing
	@Output:  				nothing
	@Return: 				false表示失败, true表示成功
	@Others:
	*************************************************/
	bool apiStartScan() {
		return CL2DeviceHandle::Instance().starScanDevice();
	}

	/*************************************************
    @Function: 				apiStartScan()
    @Description: 			停止扫描
    @Input:					nothing
    @Output:  				nothing
    @Return: 				false表示失败, true表示成功
    @Others:
    *************************************************/
	bool apiStopScan() {
		return CL2DeviceHandle::Instance().stopScanDevice();
	}

	/*************************************************
	@Function: 				apiGetDeviceFps()
	@Description: 			获取fps
	@Input:					nothing
	@Output:  				fps - 帧率
	@Return: 				nothing
	@Others:
	*************************************************/
	void apiGetDeviceFps(float& fps) {
		CL2DeviceHandle::Instance().getFps(fps);
	}

	/*************************************************
	@Function: 				apiGetPointData()
	@Description: 			获取每帧点云数据
	@Input:					nothing
	@Output:  				point - 每帧点云数据
	@Return: 				nothing
	@Others:
	*************************************************/
	void apiGetPointData(std::vector<stOutputPoint>& point) {
		CL2DeviceHandle::Instance().getPointData(point);
	}

	/*************************************************
	@Function: 				apiGetErrorCode()
	@Description: 			获取错误码
	@Input:					nothing
	@Output:  				code - 错误码
	@Return: 				nothing
	@Others:
	*************************************************/
	void apiGetErrorCode(ErrorCode& code) {
		CL2DeviceHandle::Instance().getErrorCode(code);
	}

#ifdef __cplusplus
};
#endif
