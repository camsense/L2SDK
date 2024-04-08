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
										0x00 - 表示失败
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

    /*************************************************
    @Function: 				apiUpgradeBin()
    @Description: 			升级固件
    @Input:					path - 文件路径+文件名
                            addr - 设备地址
    @Output:  				nothing
    @Return:
                    0 - 升级成功
                    -1 - 输入的地址错误
                    -2 - 串口发送命令失败
                    -3 - 读取bin文件大小失败
                    -4 - 读取bin文件数据失败
                    -5 - 升级开始步骤失败
                    -6 - 升级数据开始步骤失败
                    -7 - 升级数据包步骤失败
                    -8 - 升级数据结束步骤失败
                    -9 - 升级校验重启失败
    @Others:
    *************************************************/
    int apiUpgradeBin(const char* path, const UINT8 addr)
    {
        return CL2DeviceHandle::Instance().upgradeBin(path, addr);
    }

    /*************************************************
    @Function: 				apiGetUpgradeProgress()
    @Description: 			获取升级进度
    @Input:					nothing
    @Output:  				percentage - 升级进度百分比
    @Return: 				nothing
    @Others:
    *************************************************/
    void apiGetUpgradeProgress(float& percentage)
    {
        CL2DeviceHandle::Instance().getUpgradeProgress(percentage);
    }

    /*************************************************
    @Function: 				apiSendTimeSamp()
    @Description: 			发送时间戳同步
    @Input:					ms - 时间戳， 单位毫秒
    @Output:  				nothing
    @Return: 				false - 失败  true - 成功
    @Others:
    *************************************************/
    bool apiSendTimeStamp(const unsigned int ms)
    {
        return CL2DeviceHandle::Instance().sendTimeStamp(ms);
    }

	/*************************************************
    @Function: 				apiSwitchImgMode()
    @Description: 			切换某设备图像模式
    @Input:					addr - 设备地址
    @Output:  				nothing
    @Return: 				false - 失败  true - 成功
    @Others:
    *************************************************/
    bool apiSwitchImgMode(const UINT8 addr)
    {
        return CL2DeviceHandle::Instance().sendImgMode(addr);
    }

	/*************************************************
    @Function: 				apiGetImgData()
    @Description: 		获取图像数据
    @Input:					img - 图像数据
    @Output:  				nothing
    @Return: 				false - 失败  true - 成功
    @Others:
    *************************************************/
    bool apiGetImgData(stImgData &img)
    {
        return CL2DeviceHandle::Instance().getImgData(img);
    }

  /*************************************************
	@Function: 				apiAllStartScan()
	@Description: 			整机模式
	@Input:					nothing
	@Output:  				nothing
	@Return: 				false表示失败, true表示成功
	@Others:
	*************************************************/
    bool apiAllStartScan()
    {
        return CL2DeviceHandle::Instance().allStratScanDevice();
    }

#ifdef __cplusplus
};
#endif
