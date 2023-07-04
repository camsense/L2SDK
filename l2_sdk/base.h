#ifndef __BASE_H
#define __BASE_H
#include"data_struct.h"
#include"HC_serial.h"

#ifdef __linux__
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#endif
#if defined (_WIN32) || defined( _WIN64)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1):__FILE__)
#endif

#define __MY_DATE__ (print_curr_time())

#define LOG_WARNING (printf("HCSDK W:%s %s:%u:\t", __MY_DATE__, __FILENAME__, __LINE__), printf) 
#define LOG_INFO    (printf("HCSDK I:%s %s:%u:\t", __MY_DATE__, __FILENAME__, __LINE__), printf) 
#define LOG_ERROR   (printf("HCSDK E:%s %s:%u:\t", __MY_DATE__, __FILENAME__, __LINE__), printf) 

#define MAX_LEN 968 * 10
enum DistPkgEmun { eDist55 , eDistAA, eDistCount, eDistData, eDistSum};

#pragma pack(1)
typedef struct _Point2D{
    int x;   
    unsigned short y;   
} Point2D;

typedef struct _PckData {
    unsigned short usHD;
	unsigned char ucAD;
	unsigned char ucDN;
	Point2D data[160];
    unsigned short usVP;
    unsigned short checksum;
} PKGDATA;

//设备信息pack
typedef struct _DeviceInfoPkg {
    unsigned short usHead;				//帧头
    unsigned char ucAddr;				//设备地址
    unsigned short usLen;				//长度
    unsigned char ucManuInfo[2];		//厂家信息
    unsigned char ucProductInfo[4];		//产品信息
    unsigned char ucFirmwareVersion[4];	//固件版本号V1.0.1；
    unsigned char ucID[8];				//固件ID
    unsigned char ucReserve[4];			//预留
    unsigned char ucSum;				//校验和
}stDeviceInfoPkg;

#pragma pack()



class CBase{
public:
    CBase(void);
    ~CBase(void);

    bool init(const char *chPort,  int iBaute);             //初始化
    bool uninit();                                                                      //释放

    bool  getDeviceAddrCmd(int& addr);                      //获取设备地址cmd
    bool  getDeviceInfoCmd();                                           //获取设备信息cmd
    bool  StartScanCmd();                                                   // 开始扫描cmd
    bool  StopScanCmd();                                                    //停止扫描cmd   

    void getFps(float& fps);                                                        //获取帧率
    UINT8 getDeviceAddr();                                                      //获取设备地址
    UINT8 getDeviceNum();                                                       //获取设备个数
    int getPointData(std::vector<stOutputPoint> & point, int len);
    ErrorCode getErrorCode();                                                   //获取错误码
    bool getDeviceInfo(std::vector<DeviceInfo>& info);  //获取设备信息

protected:
    HC_serial m_serial;
    std::queue<stOutputPoint> m_quePointData;
    std::mutex mtxPointData;
    std::queue<unsigned char> m_queData;                //正常模式下的数据

    void writeDataBuffer(unsigned char data);
    int readDataBuffer(unsigned char *data, int len);
    void clearDataBuffer(void);
    
    void writePointBuffer(stOutputPoint& data);
    void writeErrorCode(ErrorCode error);
    void clearPointBuffer(void);
    void clearErrorCoed(void);
    UINT64 getCurrentTimestampUs();

#ifdef __linux__
    inline void delay(UINT32 ms) {
          while (ms >=1000){
              usleep(1000 * 1000);
              ms -= 1000;
          }
    }
#endif

private:
    void ThreadRun();                                                                   //运行采集数据并解析点云数据
    int ParsePackData(PKGDATA& Data);                                                   //解析数据
    bool CheckSum(unsigned char *buff, int len);                                        //校验和
    void ThreadRead();                                                                  //队列的方式采集数据
    void ThreadParsePkg();                                                              //从队列中读取数据并解析
    int ParasePkg(PKGDATA& data);                                                       //解析队列中的数据

 

    bool m_thread_enable = false;
    UINT8 m_uDeviceAddr = 0x00;                                                 //设备地址   
    float m_fps = 0.0;                                                                          //帧率      
    std::mutex mtx_data;
    std::mutex mtx_error;
    std::queue<ErrorCode> m_errorCode;
    double startTimes = 0.0;
    std::vector<DeviceInfo> m_vcDeviceInfo;
    bool m_bStart = false;                                       //是否启动
    std::mutex m_mtx;
 
};
#endif