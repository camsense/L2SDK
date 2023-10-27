// l2_sdk.h: 目标的头文件。

/************************************************************
@Copyright (C), Camsense. Co., Ltd.
@FileName: l2_sdk.cpp
@Author:
@ Date: 2023-05-05
@Description:  L2 SDK api接口
@Version: V1.0.1
@History: 历史修改记录
<author> <time> <version > <desc>
yzb  2023/05/05  V1.0.1 build this moudle
***********************************************************/
#ifndef __L2_SDK_H_
#define __L2_SDK_H_

#include "data_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

    //SDK初始化
    bool apiSDKInit(const char* chPort, int iBaud);
    //释放
    bool apiSDKUninit();
    //获取SDK版本号
    char* apiGetVersion();
    //获取设备信息
    bool apiGetDeviceInfo(std::vector<DeviceInfo>& Info);
    //获取设备的地址
    UINT8 apiGetDeviceAddr();
    //开始扫描
    bool apiStartScan();
    //停止扫描
    bool apiStopScan();
    //获取FPS
    void apiGetDeviceFps(float& fps);
    //获取每帧点云数据
    void apiGetPointData(std::vector<stOutputPoint>& point);
    //获取错误码
    void apiGetErrorCode(ErrorCode& code);
    //升级固件bin
    int apiUpgradeBin(const char* path, const UINT8 addr);
    //获取固件升级进度
    void apiGetUpgradeProgress(float& percentage);
    //发送时间戳同步
    bool apiSendTimeStamp(const unsigned int ms);
    //切换图像模式
    bool apiSwitchImgMode(const UINT8 addr);
    //获取图像数据
    bool apiGetImgData(stImgData& img);

#ifdef __cplusplus
};
#endif

#endif // DEBUG



