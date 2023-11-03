// L2_SDK_Demo.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <thread>
#include<fstream>
#include<stdio.h>
#include <string.h>
#include<unistd.h>
#include "l2_sdk.h"
 #include "lcm_std_msgs/lcm_utils.h"
#include "lcm_std_msgs/Point32.hpp"
#include "lcm_std_msgs/Int8.hpp"
#include "lcm/lcm-cpp.hpp"

// TODO: 在此处引用程序需要的其他标头。

#pragma pack(push,1)
struct BMPFILEHEADER
{
	uint16_t   bfType = 19778;
	uint32_t   bfSize = 21558;
	uint16_t   bfReserved1 = 0;
	uint16_t   bfReserved2 = 0;
	uint32_t  bfOffBits = 1078;
};
struct BITMAPINFOHEADER
{
	uint32_t     biSize = 40;            		//本结构所占用字节数 40字节
	int             	biWidth = 160;          	 //位图的宽度，以像素为单位
	int             	biHeight = 160;          	//位图的高度，以像素为单位
	uint16_t   	biPlanes = 1;          			//目标设备的级别，必须为1
	uint16_t   	biBitCount = 8;        		//每个像素所需的位数，必须是1（双色）、
																		//4（16色）、8（256色）或24（真彩色）之一
	uint32_t    biCompression = 0;     //位图压缩类型，必须是 0（BI_RGB不压缩）、
																		//1（BI_RLE8压缩类型）
																		//2（BI_RLE压缩类型）之一
	uint32_t    biSizeImage = 0;       		//位图的大小，以字节为单位
	int             	biXPelsPerMeter = 0;   //位图水平分辨率，每米像素数
	int             	biYPelsPerMeter  = 0;   //位图垂直分辨率，每米像素数
	uint32_t    biClrUsed = 0;         			//位图实际使用的颜色表中的颜色数
	uint32_t    biClrImportant = 0;    	//位图显示过程中重要的颜色数 
};
struct tagRGBPallete
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char alpha;
}RGBPallete;
#pragma pack(pop)


#ifdef __unix
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif


void saveImgData(unsigned char data[], const char* newFileName, std::string strType)
{
	BMPFILEHEADER bmpfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	if ("L2C2" == strType){
		bitmapinfoheader.biHeight = 160;
		bitmapinfoheader.biWidth = 160;
	}else{
		bitmapinfoheader.biHeight = 128;
		bitmapinfoheader.biWidth = 160;	
	}
	bmpfileheader.bfSize = bitmapinfoheader.biHeight  * bitmapinfoheader.biWidth + 1078;

	FILE* fp2 = NULL; // 保存文件的文件指针
	fopen_s(&fp2, newFileName, "wb"); // 二进制写入方式打开文件
	// 写入文件头
	fwrite(&bmpfileheader, sizeof(bmpfileheader), 1, fp2);
	// 写入信息头
	fwrite(&bitmapinfoheader, sizeof(bitmapinfoheader), 1, fp2);
	tagRGBPallete  rgb[256];
	memset(rgb, 0, sizeof(tagRGBPallete)*256);
	for (size_t i = 0; i < 256; i++)
	{
		rgb[i].r = i;
		rgb[i].g = i;
		rgb[i].b = i;
		rgb[i].alpha = 0;
	}
	
	fwrite(rgb, sizeof(tagRGBPallete)*256, 1, fp2);

	// 写入数据
	unsigned char* pixel_line = (unsigned char*)(data + (bitmapinfoheader.biHeight - 1) *bitmapinfoheader.biWidth );
	for (size_t i = 0; i < bitmapinfoheader.biHeight; i++)
	{
		fwrite(pixel_line, bitmapinfoheader.biWidth, 1, fp2);
		pixel_line -= bitmapinfoheader.biWidth;
	}
	
	// 关闭写入的文件
	fclose(fp2);
}

//点云
#define POINT_MODE 1
#define IMG_MODE 3

class EchoTest {
public:
	void L2SartCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
							const lcm_std_msgs:: Int8 * msgs)
	{
		L2SartCallback(*msgs);
	}

	void L2SartCallback(const lcm_std_msgs:: Int8 &   cmd){
		uint8_t  addr = (cmd.data & 0xF0) >> 4;
		uint8_t  mode = (cmd.data & 0x0F);
		printf("mode: %d\n", mode);
		printf("addr: %d\n", addr);
		switch (mode)
		{
		case 1:
			if(m_mode != POINT_MODE ||  m_iCurAddr != addr ){
				m_iCurAddr = addr;
				m_bExit = true;
				apiSDKUninit();
				apiSDKInit(m_strPort.c_str(), 921600);
				if (m_imgThread.joinable())
				{
					m_imgThread.join();
				}
				if (m_pointThread.joinable())
				{
					m_pointThread.join();
				}
				usleep(20 * 1000);
				m_bExit = false;
				m_pointThread = std::thread(&EchoTest::point_mode,  this);
			}
			m_mode = POINT_MODE;
			break;
		case 3:
			if(m_mode != IMG_MODE ||  m_iCurAddr != addr ){
				m_iCurAddr = addr;
				apiSDKUninit();
				apiSDKInit(m_strPort.c_str(), 921600);
				m_bExit = true;
				usleep(20 * 1000);
				if (m_imgThread.joinable())
				{
					m_imgThread.join();
				}
				if (m_pointThread.joinable())
				{
					m_pointThread.join();
				}
				m_bExit = false;
				m_imgThread = std::thread(&EchoTest::img_mode,  this);
			}
			m_mode = IMG_MODE;
			break;
		default:
			break;
		}
	}

	inline std::string getLcmUrl(int64_t ttl) {
  		assert(ttl >= 0 && ttl <= 255);
 	 	return "udpm://239.255.76.67:7667?ttl=" + std::to_string(ttl);
	}	
	inline void setPort(const std::string strPort){ m_strPort =  strPort;}
	inline void setMode(const int iMode){ m_mode =  iMode;}

	void run(int type){
			if (POINT_MODE == type){
				m_pointThread = std::thread(&EchoTest::point_mode,  this);
			}
			else if(IMG_MODE == type){
				m_imgThread = std::thread(&EchoTest::img_mode,  this);
			}
	}

	lcm::LCM m_lcm;

private:
	int point_mode();
	int img_mode();
	bool m_bExit = false;
	std::thread m_pointThread;
	std::thread m_imgThread;
	std::string m_strPort = "";
	int m_mode = POINT_MODE;					//1 -  正常模式  3  - 图像模式
	int m_iCurAddr = 0x01;
	
};

