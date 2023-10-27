// L2_SDK_Demo.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#define FILENAME "30mm.bmp"

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
#if 0
//读取数据的函数
unsigned char** readData(BMPFILEHEADER& bmpfileheader, BITMAPINFOHEADER& bitmapinfoheader)
{
	FILE* fp = NULL;  // C标准库的文件指针
	fopen_s(&fp, FILENAME, "rb"); // 二进制读取方式打开文件
	unsigned char** data = NULL;

	// 读取文件头
	fread(&bmpfileheader, sizeof(bmpfileheader), 1, fp);
	// 读取信息头
	fread(&bitmapinfoheader, sizeof(bitmapinfoheader), 1, fp);

	//读取调色板
	tagRGBPallete  rgb[256];
	memset(rgb, 0, sizeof(tagRGBPallete)*256);
	fread(rgb, sizeof(tagRGBPallete)*256, 1, fp);
	// 动态分配二维数组存储像素数据，注意先申请一个存放指针的数组，
	data = (unsigned char **)malloc(sizeof(unsigned char*) * bitmapinfoheader.biHeight );
	//其大小为sizeof(unsigned char*) * bitmapinfoheader.biHeight，这点很容易错
	//申请行指针
	for (int i = 0; i < bitmapinfoheader.biHeight; i++)
	{
		data[i] = (unsigned char *)malloc(bitmapinfoheader.biWidth  );
		// 读取像素数据
		for (int j = 0; j < bitmapinfoheader.biWidth ; j++)
		{
			fread(&data[i][j], 1, 1, fp);
		}
	}

	return data;
	// 关闭读取的文件
	fclose(fp);
}
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


