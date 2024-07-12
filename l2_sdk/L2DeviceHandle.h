#ifndef     __L2DEVICEHANDLE_H
#define     __L2DEVICEHANDLE_H

#include "data_struct.h"
#include "HC_serial.h"
#include "base.h"


class CL2DeviceHandle: public HC_serial{
public:
    static CL2DeviceHandle& Instance();
	virtual ~CL2DeviceHandle(void);

    bool init(const char *chPort,  int iBaud);                              //设备初始化
    bool uinit();
    char* getSDKVersion();                                                  //获取SDK 版本号
    UINT8 getNumberOfDevice();                                              //获取设备数量
    bool starScanDevice();                                                  //开始扫描
    bool stopScanDevice();                                                  //停止扫描
      bool allStratScanDevice();                                           //整机模式
    void getFps(float& fps);
    void getPointData(std::vector<stOutputPoint>& data);                    //获取点云
    void getErrorCode(ErrorCode& error);                                    //获取错误码
    bool getInfo(std::vector<DeviceInfo>& info);                            //获取设备信息
    
    //升级
    int upgradeBin(const char* path, const UINT8 addr);
    void getUpgradeProgress(float& percentage);

    bool sendTimeStamp(const unsigned int ms);
    bool sendImgMode(const UINT8 addr);
    bool getImgData(stImgData& img);
protected:
	CL2DeviceHandle();
    CBase m_pBase;

private:
    std::string m_strSdkVersion = "V1.0.11";

};


#endif
