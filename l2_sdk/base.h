#ifndef __BASE_H
#define __BASE_H
#include"data_struct.h"
#include"HC_serial.h"
#include <condition_variable>
#include <atomic>

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

#define HEAD                0x55
#define ADDR                0x0a
#define DEVICE              0x0b
#define START               0x0c
#define STOP                0x0d

#define MAX_LEN 968 * 10
enum DistPkgEmun {
    eTimeHead0 = 0,
    eTimeHead1,
    eTimeData,
    eTimeSum,
    eDataPkg,
    eDataEnd,
};
enum CmdEmun{eInit, eGetAddr, eGetInfo, eScanStop};

#pragma pack(1)
typedef struct _Point2D{
#if 0
    int x;   
    unsigned short y;  
#else
    short x : 14;                       //x坐标，单位0.1mm，有符号
    unsigned short flag : 1;            //有效标志位
    unsigned short fiter : 1;           //过滤标志位
    unsigned short y : 12;              //y坐标，单位0.1mm，无符号
    unsigned short reserve : 4;         //保留位
    unsigned char quality : 8;          //亮度信息
    UINT8 row : 8;                      //行
#endif
} Point2D;

typedef struct _PckData {
#if 0
    unsigned short usHD;
	unsigned char ucAD;
	unsigned char ucDN;
	Point2D data[160];
    unsigned short usVP;
    unsigned short checksum;
#else
    unsigned short usHead;      //包头
    unsigned char ucAddr;       //地址
    unsigned char param_h;      //参数H
    unsigned int u64Ms;         //时间戳
    Point2D data[160];          //点云数据
    unsigned char param_v;      //参数V
    unsigned char param_e;      //参数e
    unsigned short usCheckSum;  //校验和
#endif
} PKGDATA;

//时间同步
typedef struct {
    unsigned short usHead : 16;          //包头
    unsigned int usMs : 32;              //毫秒时间戳
    unsigned short checksum : 16;        //包校验和
}TIMEPKG;

//设备信息pack
typedef struct _DeviceInfoPkg {
    unsigned short usHead;				//帧头
    unsigned char ucAddr;				//设备地址
    unsigned short usLen;				//长度
    unsigned char ucManuInfo[2];		//厂家信息
    unsigned char ucProductInfo[4];		//产品信息
    unsigned char ucFirmwareVersion[4];	//固件版本号V1.0.1；
    unsigned char ucSN[32];				//固件SN
    unsigned char ucReserve[4];			//预留
    unsigned char ucSum;				//校验和
}stDeviceInfoPkg;



#pragma pack()

#define MAGIC_GD_L2			    0x324C4447// GDL2
#define UPGRDE_HEAD_PKG0        0x55 
#define UPGRDE_HEAD_PKG1        0x0E
#define TIME_HEAD_PKG0                0x55 
#define TIME_HEAD_PKG1                0x0F
#define POINT_HEAD_PKG0             0x55 
#define POINT_HEAD_PKG1             0xAA

#define  MSG_START          0
#define  MSG_TYPE             1
#define  MSG_TIME             2
#define  MSG_POINT          3

#define		MAX_PKT_LEN		1024*4    

//cmd
enum {
    UPDATE_CMD_START = 0,           //开始升级
    UPDATE_CMD_DATA_START,          //开始升级数据
    UPDATE_CMD_DATA,                //升级数据
    UPDATE_CMD_DATA_END,            //升级数据结束
    UPDATE_CMD_REBOOT,              //重启
};

//head
typedef struct {
    unsigned short head;// 0x0E55
    UINT8 id : 2;
    UINT8 err : 3;
    UINT8 update_cmd : 3;
    UINT8 seq_k;
} update_head;

//发送以及接受命令包结构
typedef struct {
    update_head pkg_head;               //包头
    unsigned int filter;                //升级匹配标准
    unsigned int descriptor;            //升级描述标准
    unsigned short fw_checksum;         //固件校验和
    unsigned short pck_checksum;        //包校验和
}UPGRDE_FARMWARE_PKG;

//数据发送命令包结构
typedef struct {
    update_head pkg_head;               //包头
    unsigned short valid_len;           //数据长度
    unsigned char data[2048];           //数据
    unsigned short pck_checksum;        //包校验和
}UPGRDE_DATA_PKG;


class CBase{
public:
    CBase(void);
    ~CBase(void);

    //初始化
    bool init(const char *chPort,  int iBaute);    
     //释放         
    bool uninit();         
     //配置设备                                                            
    bool configDevices();                                                    
    //获取设备地址cmd
    bool  getDeviceAddrCmd();       
    //获取设备信息cmd                                      
    bool  getDeviceInfoCmd();        
     //开始扫描cmd                                       
    bool  StartScanCmd();         
    //停止扫描cmd                                                
    bool  StopScanCmd();                                                       
    //获取帧率
    void getFps(float& fps);       
      //获取设备个数                                         
    UINT8 getDeviceNum();        
    //获取点云                                         
    int getPointData(std::vector<stOutputPoint> & point, int len);
    //获取错误码
    ErrorCode getErrorCode();              
    //获取设备信息                                 
    bool getDeviceInfo(std::vector<DeviceInfo>& info);                  
    inline void setEnable(bool res){                        
            m_thread_enable = res;
    }
    //升级bin文件
    int upgradeBin(const char* path, const UINT8 addr);   
    //设置升级进度                  
    void setProgress(float percentage) {                                    
        std::unique_lock<std::mutex> uclk(m_mutProgress);
        m_fProgress = percentage;
    }
    //获取升级进度
    void getProgress(float& percentage) {                                   
        std::unique_lock<std::mutex> uclk(m_mutProgress);
        percentage = m_fProgress;
    }
    //设置时间戳同步
    bool sendTimeStamp(const unsigned int ms);          

    //切换图像模式
    bool sendImgCmdMode(const UINT8 uaddr );   
    bool getImgData(stImgData& img) ;              

protected:
    HC_serial m_serial;
    std::queue<stOutputPoint> m_quePointData;
    std::mutex mtxPointData;
    std::queue<unsigned char> m_queData;                //正常模式下的数据
    stDeviceInfoPkg m_arrDeviceInfo[3];

    void writeDataBuffer(unsigned char data);
    int readDataBuffer(unsigned char *data, int len);
    void clearDataBuffer(void);
    
    void writePointBuffer(stOutputPoint& data);
    void writeErrorCode(ErrorCode error);
    void clearPointBuffer(void);
    void clearErrorCoed(void);
    UINT64 getCurrentTimestampUs();

    void writeCmdBuffer(unsigned char data);
    int readCmdBuffer(unsigned char *data, int len);

    void eraseBuff(std::vector<unsigned char>& lstG,int iLen);

    bool reviceStop(std::vector<unsigned char>& lstData);
    bool reviceAddr(std::vector<unsigned char>& lstData);
    bool reviceDeviceInfo(std::vector<unsigned char>& lstData);

    inline void delay(UINT32 ms) {
        #ifdef __linux__
          while (ms > 0){
              usleep( 1000);
              ms--;
          }
          #else
          Sleep(ms);
          #endif
    }


private:
    bool CheckSum(unsigned char *buff, int len);                     //校验和
    void ThreadCmd();
    void ThreadParse();
    bool sendData(unsigned char data[], const int len);

    //读取数据线程
    void readDataThread();
    //解析数据线程
    void parseDataThread();
    //解析时间戳
    bool parseTime(std::vector<unsigned char> &lstData);   
      //解析点云                 
    bool parsePointData(std::vector<unsigned char> &lstData);      
    
    void ThreadUpgradeRun();                                        //启动升级数据接收线程
    void readUpgradeThead();                                        //读取升级应答数据线程
    void parseUpgradeThead();                                       //解析升级应答指令线程
    bool parseUpgradeData(std::vector<unsigned char> &lstData);     // 解析应答指令函数
    void UpgradeUninit();                                                   //退出升级


     //----------------升级--------------------
     //获取bin文件大小
    bool getBinSize(const char* path, unsigned int& size);       
    //获取bin文件数据                                       
    bool getBinData(const char* path, unsigned char* data, unsigned int size);   
     //----------------升级指令--------------------
    //升级开始
    bool upgradeStart(const UINT8 addr);            
     //升级数据开始                                                            
    bool upgradeDataStart(const UINT8 addr);                 
    //升级数据 包                                                  
    bool upgradeData(const UINT8 addr, unsigned char data[], const unsigned short len, UINT8 uIndex); 
    //升级结束          
    bool upgradeDataEnd(const UINT8 addr, unsigned char data[], const unsigned short len, UINT8 uIndex) ;//启动升级数据接收线程

    //校验重启
    bool upgradeReset(const UINT8 addr, const unsigned short FW_checksum);                                  
    //发送升级数据
    bool sendUpgradeData(unsigned char data[], const unsigned short len);

    //解析图像数据
    bool parseImgData();  
    int paraseImgEx(unsigned char* buff);

    std::thread m_threadCmd ;
    std::thread m_threadParse;

    //设备地址   
    UINT8 m_uDeviceAddr = 0x00;   
     //帧率                                              
    float m_fps = 0.0;                                                        

    std::mutex mtx_data;
    std::mutex mtx_error;
    std::queue<ErrorCode> m_errorCode;

    double startTimes = 0.0;
    std::vector<DeviceInfo>m_vcDeviceInfo;
    bool m_bStart = false;                                                      //是否启动
    std::mutex m_mtx;
    bool m_bInit = false;

    std::atomic<bool>      m_thread_enable;
    
    std::vector<unsigned char>m_lstBuff;
    std::vector<unsigned char>m_lstTemp;
    std::mutex m_mtxBuff;

    //升级
    std::thread m_threadUpgradRead;
    std::thread m_threadUpgradParse;
    bool m_isResponseCommand = false;               //是否收到应答指令
    UPGRDE_FARMWARE_PKG m_pUPGRDE_FARMWARE_PKG;     //应答数据结构
    std::vector<unsigned char> m_lstUpgrdeBuff;
    std::vector<unsigned char> m_lstUpgrdeTemp;
    std::mutex m_mutUpgrdeBuff;
    float m_fProgress = 0.0;
    std::mutex m_mutProgress;
    UINT64 m_u64DevicesTime = 0;
    int m_iCount = 0;
    std::mutex m_mutImg;
    stImgData m_img_data;
    bool m_isGetImg = false;
    int m_iRow = 160;
    int m_iCol = 160;
    bool m_bFindHead = true;
    bool m_bDone = true;
};
#endif