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
    void getFps(float& fps);
    void getPointData(std::vector<stOutputPoint>& data);
    void getErrorCode(ErrorCode& error);
    bool getInfo(std::vector<DeviceInfo>& info);
    

protected:
	CL2DeviceHandle();
    CBase m_pBase;

private:
    std::string m_strSdkVersion = "V1.0.3";

};


#endif
