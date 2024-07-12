
/* *****************************************************************
 * @File_Name   :   data_struct.h
 * @Auth0r      :   yzb
 * @Email       :   zb.yin@camsense.cn
 * @Data        :   2023-05-29 
 * @Copyright   :   CAMSENSE. All rights reserved
 * @Description :   结构体数据文件
 * @Brief       :
 ******************************************************************/
#ifndef      __DATA_STRUCT_H
#define     __DATA_STRUCT_H
#include<string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <functional>
#include <list>
#include <set>
#include <chrono>
#include <ctime> 
#include <stdarg.h> 
#include <deque>
#include <queue>
#include <map>

typedef unsigned char                           UINT8;
typedef unsigned char                           UCHAR;
typedef unsigned short                          UINT16;
typedef unsigned int                            UINT;
typedef unsigned long                           ULONG;
typedef unsigned long long int                  UINT64;  
typedef unsigned int                            UINT32;


typedef struct _deviceInfo{
    UINT8 addr;                                             //设备地址
    std::string factoryInfo;                                //厂家信息
    std::string productName;                                //产品名称    
    std::string firmwareVersion;                            //固件版本号 
    std::string deviceSN;                                   //设备SN 号 
    void  DeviceInfo() {
        addr = 0;
        factoryInfo.clear();
        productName.clear();
        firmwareVersion.clear();
        deviceSN.clear();
    }
}DeviceInfo;

typedef struct _PointData{
    bool bflag;                                  //是否有效： true - 有效   false - 无效
    bool bfiter;                                 //是否被过滤:  true - 是     false - 否
    float x;                                     //x坐标
    float y;                                     //y坐标
    unsigned char quality;                       //亮度信息
    UINT8 row;                           //每个激光点所在的行
}POINTDATA;

typedef struct _OutputPoint
{
    UINT64 u64LocTimeStampS;                        //本地解析数据的时间戳
    UINT64 u64DeviceTimeStampS;                     //设备的时间戳
    UINT64 u64ExposureTimeStamp;                    //每帧开始曝光的时间戳
    UINT8 uaddr;                                    //设备地址
    std::vector<POINTDATA> Point;                   //一帧的点云数据
}stOutputPoint;


//错误码
 enum ErrorCode{
    IDLE = 0,
    ERROR_INIT_FAIL = -1000,                                 //init失败
    ERROR_OPEN_SERIAL_FAIL ,                            //open com failed
    ERROR_ADDR_CMD_FAIL,                                    //获取地址命令失败
    ERROR_INFO_CMD_FAIL,                                    //获取设备信息命令失败
    ERROR_STARTSCAN_CMD_FAIL,                    //开始扫描命令失败     
    ERROR_STOPSCAN_CMD_FAIL,                     //停止扫描命令失败
    ERROR_NOT_DATA,                                             //设备无数据
    ERROR_CHECKSUM_FAIL,                                //校验和失败

    ERROR_UPDATE_CHECKSUM,                              //升级数据包校验和错误
    ERROR_UPDATE_FILTER,                                //升级的固件不匹配
    ERROR_UPDATE_SEQ,                                   //升级顺序错误
    ERROR_UPDATE_FW_CHECKSUM,                            //固件校验和错误
    ERROR_READ_THREAD_EXIT,                                  //read线程退出
    ERROR_PARSE_THREAD_EXIT,                                //解析线程退出
};

//图片数据
typedef struct _img_data_struct
{
    /* data */
    unsigned char data[160*160];
}stImgData;


//------------------------------------------------test---------------------------------------------
#pragma once
 
/**
 * @file 
 * @author yzb
 * @brief 
 * @version 0.1
 * @date 2023-09-21
 *
 * @copyright Copyright (c) 2023
 *
 */
 
#include <condition_variable>
#include <mutex>
#include <vector>
#include <chrono>
#include <thread>
 
#endif