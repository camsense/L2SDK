#include"base.h"

CBase::CBase(){
                     
}

CBase::~CBase(){
    uninit();
}

bool CBase::init(const char *chPort, int iBaute)
{
    m_thread_enable = false;
    m_uDeviceAddr = 0x00;                                                   
    m_fps = 0.0;  
    startTimes = 0.0;

    clearPointBuffer();
    clearErrorCoed();

    if(chPort == NULL){
        printf("port is null");
        writeErrorCode(ERROR_INIT_FAIL);
        return false;
    }

    if( m_serial.openDevice(chPort, iBaute) != 1){
        printf(" open device is failed!\n" );
        writeErrorCode(ERROR_INIT_FAIL);
        return false;
    }

    if (!StopScanCmd())
    {
        printf(" stop device  failed!\n" );
        writeErrorCode(ERROR_STOPSCAN_CMD_FAIL);
        return false;
    }

    #if  __linux__
        delay(5);
    #else
        Sleep(5);
    #endif


    int addr = 0x00;
    if (!getDeviceAddrCmd(addr))
    {
        writeErrorCode(ERROR_ADDR_CMD_FAIL);
        return false;
    }
    printf("addr: %d \r\n", addr);
    
   #if  __linux__
        delay(5);
    #else
        Sleep(5);
    #endif

    if (!getDeviceInfoCmd())
    {
        writeErrorCode(ERROR_INFO_CMD_FAIL);
        return false;
    }
    printf("init finish \r\n");
    return true;
}

bool CBase::uninit()
{
    std::vector<DeviceInfo> ().swap(m_vcDeviceInfo);
    m_thread_enable = false;
    clearPointBuffer();
    clearErrorCoed();
    StopScanCmd();
    m_serial.closeDevice();
    return false;
}

bool CBase::getDeviceAddrCmd(int &addr)
{
#if defined (_WIN32) || defined(_WIN64)
    Sleep(2);
#else 
    delay(10);
#endif
    m_serial.flushReceiver();
    unsigned char  cData[4] = { 0x55, 0x0A,0x00, 0x5F };
    if (m_serial.writeData2(cData, sizeof(cData)) < 1)
    {
        printf("write DeviceAddrCmd failed!");
        writeErrorCode(ERROR_ADDR_CMD_FAIL);
        return false;
    }

    int nLen = 4;
    unsigned char  cBuff[4] = { 0 };
    if (m_serial.readData(cBuff, nLen,50) != nLen)
    {
        writeErrorCode(ERROR_ADDR_CMD_FAIL);
        printf("read  Device Addr failed!");
        return false;
    }
    unsigned char ucSum = (cBuff[0] + cBuff[1] + cBuff[2]);
    
    if (ucSum != cBuff[3])
    {
        writeErrorCode(ERROR_ADDR_CMD_FAIL);
        printf("CheckSum failed!\n");
        return false;
    }

    if (cBuff[0] == 0x55 && cBuff[1] == 0x0A)
    {
        addr = cBuff[2];
        printf("get Device Add: %d \r\n", addr);
        m_uDeviceAddr = addr;
        return true;
    }
    printf("get Device Addr failed!");
    writeErrorCode(ERROR_ADDR_CMD_FAIL);
    return false;
}

bool CBase::getDeviceInfoCmd()
{
#if defined (_WIN32) || defined(_WIN64)
    Sleep(2);
#else 
    delay(10);
#endif
    std::vector<DeviceInfo> ().swap(m_vcDeviceInfo);

    if (m_uDeviceAddr == 0x00)
    {
        printf(" no find device,please get device addr \r\n");
        return false;
    }

    unsigned char  cData[4] = { 0x55, 0x0B,0x00,0x60 };
    if (m_serial.writeData2(cData, sizeof(cData)) < 1)
    {
        printf("write getDeviceInfoCmd failed! \r\n");
        writeErrorCode(ERROR_INFO_CMD_FAIL);
        return false;
    }

    for (int i = 0; i < m_uDeviceAddr; i++) {
        stDeviceInfoPkg stDeviceInfoTemp;
        memset(&stDeviceInfoTemp, 0, sizeof(stDeviceInfoTemp));
        int nLen = sizeof(stDeviceInfoTemp);
        unsigned char  cBuff[255] = { 0 };
        if (m_serial.readData(cBuff, nLen,10) != nLen)
        {
            printf("read  getDeviceInfoCmd failed! \r\n");
            writeErrorCode(ERROR_INFO_CMD_FAIL);
            return false;
        }
        
        if (cBuff[0] == 0x55 && cBuff[1] == 0x0b)
        {
            memcpy(&stDeviceInfoTemp, cBuff, nLen);
        }
      
        unsigned char ucSum = 0;
        for (int n = 0; n < nLen - 1; n++)
        {
            ucSum += cBuff[n];
        }

        if (ucSum != stDeviceInfoTemp.ucSum)
        {
            writeErrorCode(ERROR_INFO_CMD_FAIL);
            printf("getDeviceInfoCmd CheckSum failed!\r\n");
            return false;
        }

        
        DeviceInfo info;
        //addr
        info.addr = i + 1;

        char buf[128] = {0};
        //ID 
        for (int j = 0; j < 8; j++)
        {
            sprintf(buf + i, "%c", stDeviceInfoTemp.ucID[i]);
        }
        info.deviceID = buf;
        
         //
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%c%c", stDeviceInfoTemp.ucManuInfo[0], stDeviceInfoTemp.ucManuInfo[1]);
        info.factoryInfo = buf;

        //productName
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%c%c", stDeviceInfoTemp.ucProductInfo[0], stDeviceInfoTemp.ucProductInfo[1]);
        info.productName = buf;

        //version
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%c%c.%c.%c", stDeviceInfoTemp.ucFirmwareVersion[0], 
            stDeviceInfoTemp.ucFirmwareVersion[1],
            stDeviceInfoTemp.ucFirmwareVersion[2], 
            stDeviceInfoTemp.ucFirmwareVersion[3]);
        info.firmwareVersion = buf;
        m_vcDeviceInfo.push_back(info);
    }
    return true;
}

bool CBase::StartScanCmd()
{
#if defined (_WIN32) || defined(_WIN64)
    Sleep(2);
#else 
    delay(2);
#endif
    m_serial.flushReceiver();
    unsigned char  cData[4] = { 0x55, 0x0C, 0x00, 0x61 };
    if (m_serial.writeData2(cData, sizeof(cData)) < 1) 
    {
        printf("Start ScanCmd failed!");
        return false;
    }
#if defined (_WIN32) || defined(_WIN64)
   
    Sleep(1800);
#else 
    delay(1800);
#endif
    m_bStart = true;
    m_thread_enable = true;
    m_serial.flushReceiver();

#if 0
    //队列方式采集数据
    //std::thread thRead(&CBase::ThreadRead, this);
    //thRead.detach();
    //std::thread thParasePkg(&CBase::ThreadParsePkg, this);
    //thParasePkg.detach();
#else
    std::thread thRun(&CBase::ThreadRun, this);
    thRun.detach();
#endif

    
    return true;
}



bool CBase::StopScanCmd()
{
    m_thread_enable = false;
    int nCount = 0;
    clearPointBuffer();
    clearErrorCoed();

    while (nCount < 5)
    {
        nCount++;
        m_serial.flushReceiver();

#ifdef __linux__
        delay(5);
#else
        Sleep(5);
#endif
       
        printf("nCountT: %d\r\n", nCount);
        unsigned char  cData[4] = { 0x55, 0x0D, 0x00, 0x62 };
        if (m_serial.writeData2(cData, sizeof(cData)) < 1)
        {
            #if defined (_WIN32) || defined(_WIN64)
            Sleep(5);
            #else 
                delay(5);
            #endif
               printf("stop cmd failed \n");
            continue;
        }
        m_serial.flushReceiver();

#ifdef __linux__
        delay(100);
#else
        Sleep(100);
#endif
        int nLen = 2;
        double dBigen = m_serial.GetTimeStamp() / 1.0e6;
        std::vector<unsigned char> veTemp;
        while (true)
        {
            unsigned char  cBuff =  0 ;
            if (m_serial.readData(&cBuff, 1) <= 0)
            {
                if (veTemp.size() >= 4)
                {
                    unsigned char ucSum = (veTemp[veTemp.size() - 4] + veTemp[veTemp.size() - 3] + veTemp[veTemp.size() - 2]);
                    if (ucSum != veTemp[veTemp.size() - 1])
                    {
                        printf("CheckSum failed! 1\n");
                        //return false;
                        break;
                    }

                    if (veTemp[veTemp.size() - 4] == 0x55 && veTemp[veTemp.size() - 3] == 0x0D)
                    {
                        m_bStart = false;
                        return true;
                    }
                }
                break;
            }
            else {
                veTemp.push_back(cBuff);
                //printf("%02x,", cBuff);
            }

            double dEnd = m_serial.GetTimeStamp() / 1.0e6;
            if (dEnd - dBigen > 1000)
            {
                printf("read  Stop cmd timeout!\n");
                break;
            }
            continue;
        }
        continue;
    }

    return false;
}

void CBase::getFps(float &fps)
{
    std::unique_lock<std::mutex> uclk(mtx_data);
    fps = m_fps;
}

UINT8 CBase::getDeviceAddr()
{
    return UINT8();
}

//
UINT8 CBase::getDeviceNum()
{
    return m_uDeviceAddr;
}


int CBase::getPointData(std::vector<stOutputPoint>& point, int len)
{
    std::unique_lock<std::mutex> ulck(mtxPointData);
    {
        if (m_quePointData.size() == 0 || m_quePointData.size() <= len)
        {
            return 0;
        }
        for (int i = 0; i < len; i++)
        {
            point.push_back(m_quePointData.front());
            m_quePointData.pop();
        }
    }
}

ErrorCode CBase::getErrorCode()
{
    ErrorCode error = IDLE;
    std::unique_lock<std::mutex> ulck(mtx_error);
    {
        if (m_errorCode.size() == 0)
        {
            return error;
        }

        error = m_errorCode.front();
        m_errorCode.pop();
    }
    return error;
}

bool CBase::getDeviceInfo(std::vector<DeviceInfo>& info)
{
    if (m_vcDeviceInfo.size() != m_uDeviceAddr || m_vcDeviceInfo.size() <= 0)
    {
        return false;
    }
    info = m_vcDeviceInfo;
    return true;
}

void CBase::writePointBuffer(stOutputPoint& data)
{
    std::unique_lock<std::mutex> ulck(mtxPointData);
    {
        if (m_quePointData.size() == MAX_LEN)
        {
            m_quePointData.pop();
        }
        m_quePointData.push(data);
    }
}

void CBase::writeErrorCode(ErrorCode error)
{
    std::unique_lock<std::mutex> ulck(mtx_error);
    {
        if (m_errorCode.size() == MAX_LEN)
        {
            m_errorCode.pop();
        }
        m_errorCode.push(error);
    }
}

void CBase::writeDataBuffer(unsigned char data)
{
    std::unique_lock<std::mutex> ulck(m_mtx);
    if(m_queData.size() >= MAX_LEN)
    {
        m_queData.pop();
    }
    m_queData.push(data);
}

int CBase::readDataBuffer(unsigned char *data, int len)
{
    std::unique_lock<std::mutex> ulck(m_mtx);
    if (m_queData.size() == 0 || m_queData.size() < len)
    {
        return 0;
    }

    for(int i = 0; i < len; i++)
    {
        data[i] = m_queData.front();
        m_queData.pop();
    }
    return len;
}

void CBase::clearDataBuffer(void)
{
    std::unique_lock<std::mutex> ulck(m_mtx);
    std::queue<unsigned char> temp;
    temp.swap(m_queData);
    //while (!m_queData.empty())
    //{
     //   m_queData.pop();
    //};
}

void CBase::clearPointBuffer(void)
{
    std::unique_lock<std::mutex> ulck(mtx_error);
    while (!m_quePointData.empty())
    {
        m_quePointData.pop();
    };
}

void CBase::clearErrorCoed(void)
{
    std::unique_lock<std::mutex> ulck(mtx_error);
    while (!m_errorCode.empty())
    {
        m_errorCode.pop();
    };
}

UINT64 CBase::getCurrentTimestampUs()
{
    auto ts = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return ts;
}

void CBase::ThreadRun()
{
    m_thread_enable = true;
    int nFps = 0;
    while (m_thread_enable) {
        PKGDATA stTemp;
        int bRes = 0;
        bRes = ParsePackData(stTemp);

        std::unique_lock<std::mutex> lck(mtx_data);
        double endTimes = m_serial.GetTimeStamp() / 1.0e6;
        nFps++;
        if (endTimes - startTimes >= 1000.0)
        {
            m_fps = nFps;
            nFps = 0;
            startTimes = endTimes;
        }

        if (bRes > 0) 
        {
            std::vector<POINTDATA> vcPoint;
            for (int i = 0; i < stTemp.ucDN; i++)
            {
                POINTDATA pTemp;
                float x = (float)stTemp.data[i].x * 0.01;
                float y = (float)stTemp.data[i].y * 0.01;
                if (y <= 20.0 || y >= 600.0 || x >= 1100.0 || x <= -1100.0)
                {
                    pTemp.bflag = false;
                    pTemp.x = 0;
                    pTemp.y = 0;
                }
                else {
                    pTemp.bflag = true;
                    pTemp.x = x;
                    pTemp.y = y;
                } 
                vcPoint.push_back(pTemp);
            }
            stOutputPoint outPoint;
            //outPoint.u64TimeStampS = m_serial.GetTimeStamp()/ 1.0e6;
            outPoint.u64TimeStampS = getCurrentTimestampUs() / 1000;
            outPoint.uaddr = stTemp.ucAD;
            outPoint.Point.swap(vcPoint);
            writePointBuffer(outPoint);
        }
        //std::this_thread::sleep_for(std::chrono::microseconds(1));
        //Sleep(1);
    };
}

int CBase::ParsePackData(PKGDATA& Data)
{
#define MAX_COUNT  100
    unsigned char uCur = 0x00;
    int nCount = 0;
    int nStep = 0;
    unsigned char ucBuffer[1024 * 5] = { 0 };
    int nResCount = 0;

    while (m_thread_enable)
    {
        if (m_serial.readChar(&uCur, 5) != 1)
        {
            nStep = 0;
            nCount++;
            if (nCount > MAX_COUNT)
            {
                printf("read char failed!\r\n");
                writeErrorCode(ERROR_NOT_DATA);
                nCount = 0;
                return -1;
            }
            continue;
        }
        int nDataLen = 0;
        switch (nStep)
        {
        case eDist55:
            if (uCur == 0x55)
            {
                nStep = eDistAA;
                ucBuffer[0] = 0x55;
            }
            break;
        case eDistAA:
            if (uCur == 0xAA)
            {
                nStep = eDistCount;
                ucBuffer[1] = 0xaa;
            }
            else {
                nStep = eDistAA;
            }
            break;
        case eDistCount:
            ucBuffer[2] = uCur;
            m_serial.readChar(&uCur, 5);
            ucBuffer[3] = uCur;

            memcpy(&nResCount, &ucBuffer[3], 1);
            if (nResCount == 160)
            {
                nDataLen = nResCount * 6;
                nStep = eDistData;
            }
            else {
                nStep = eDist55;
            }
            break;
        default:
            break;
        }

        if (nStep == eDistData)
        {
            int nLen =  m_serial.readChars(&ucBuffer[4], nDataLen + 4, 1000);
            if ( nLen != nDataLen + 4)
            {
                nStep = eDist55;
                printf("read char  nDataLen + 4 failed!\r\n");
                memset(ucBuffer, 0, sizeof(ucBuffer));
                return -1;
            }
            if (!CheckSum(ucBuffer, nDataLen + 8))
            {
                //printf("dist CheckSum  failed!\r\n");
                nStep = eDist55;
                memset(ucBuffer, 0, sizeof(ucBuffer));
                 //writeErrorCode(ERROR_CHECKSUM_FAIL);
                return -1;
            }

            memcpy(&Data, ucBuffer, sizeof(PKGDATA));
            return 1;
        }
    }
    return 0;

}

/*******************************************************************
* �������� CheckSum
* ���ܣ���֤У���
* ������
*       buff - ���ݣ� len - ����
* ���أ�
*     true - У��ɹ�  false- У��ʧ��
* ʱ�䣺2023/03/01
* ���ߣ�yzb
* �޸ģ�
*******************************************************************/
bool CBase::CheckSum(unsigned char* buff, int len)
{
    if (sizeof(buff) > len)
    {
        return false;
    }
    unsigned short nSum = 0;
    unsigned short nCheck = (buff[len - 1] << 8);
    nCheck = nCheck + buff[len - 2];
    for (int i = 0; i < len - 2; ++i)
    {
        nSum += buff[i];
    }

    return (nCheck == nSum) ? true : false;
}

//read  thread
void CBase::ThreadRead()
{
    unsigned char  ucBuff[1024] = { 0 };
    int nErrorCount = 0;
    clearDataBuffer();
    while (m_thread_enable) {
        /* code */
        int nLen =  m_serial.readChar(ucBuff, 10);
        if ( nLen  <= 0){
            nErrorCount++;
            if(nErrorCount >= sizeof(PKGDATA)){
                    printf("read char failed!\r\n");
                    nErrorCount = 0;
                    memset(ucBuff, 0, sizeof(ucBuff));
                    writeErrorCode(ERROR_NOT_DATA);
            }
            continue;
        }

		for (int i = 0; i < nLen; i++){
			writeDataBuffer(ucBuff[i]);
		}
        memset(ucBuff, 0, sizeof(ucBuff));
        
    };
    
}

void CBase::ThreadParsePkg()
{
    double startTimes =  m_serial.GetTimeStamp()/ 1.0e6;
    double endTimes =  m_serial.GetTimeStamp()/ 1.0e6;
    PKGDATA data;
    int nFps = 0;
#ifdef __linux__
    delay(50);
#else
    Sleep(50);
#endif
     while (m_thread_enable)
     {
        std::unique_lock<std::mutex> lck(mtx_data);
        endTimes =  m_serial.GetTimeStamp()/ 1.0e6;
        memset(&data, 0 , sizeof(data));
        int nRes =  ParasePkg(data);
         nFps++;
        if (endTimes - startTimes >= 1000.0)
        {
            if (m_uDeviceAddr != 0x00)
            {
                m_fps = (float)nFps / (float)m_uDeviceAddr;
            }
            nFps = 0;
            //printf("fps: %.2f\r\n", m_fps);
            startTimes = endTimes;
        }
        if(nRes > 0){
             std::vector<POINTDATA> vcPoint;
            for (int i = 0; i < data.ucDN; i++)
            {
                POINTDATA pTemp;
                float x = (float)data.data[i].x * 0.01;
                float y = (float)data.data[i].y * 0.01;
                if (y <= 20.0 || y >= 600.0 || x >= 600.0 || x <= -600.0)
                {
                    pTemp.bflag = false;
                    pTemp.x = 0;
                    pTemp.y = 0;
                }
                else {
                    pTemp.bflag = true;
                    pTemp.x = x;
                    pTemp.y = y;
                } 
                vcPoint.push_back(pTemp);
            }
            stOutputPoint outPoint;
            outPoint.uaddr = data.ucAD;
            outPoint.Point.swap(vcPoint);
            writePointBuffer(outPoint);
            //printf("add: %d\n", data.ucAD);
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
     };
        
}

int CBase::ParasePkg(PKGDATA &data)
{
     unsigned char uCur = 0x00;
    int nCount = 0;
    int nStep = 0;
    unsigned char ucBuffer[1024 * 5] = { 0 };
    int nResCount = 0;
    bool bFindHead = false;
    int nIndex  = 0;

    int nDataLen = 0;
    int nFPS = 0;
    while (m_thread_enable)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(2)); 
        if (!bFindHead && readDataBuffer(&uCur, 1) != 1)
        {
            bFindHead = false;
            nStep = 0;
            nCount++;
           
            if (nCount > 921600)
            {
                printf(" read time !\r\n");
                writeErrorCode(ERROR_NOT_DATA);
                nCount = 0;
                return -1;
            }
            continue;
        }
        nCount = 0;
    
        switch (nStep)
        {
        case eDist55:
            
            if (uCur == 0x55)
            {
                nStep = eDistAA;
                ucBuffer[0] = 0x55;
            }
            break;
        case eDistAA:
            if (uCur == 0xAA)
            {
                nStep = eDistCount;
                ucBuffer[1] = 0xaa;
            }
            else {
                nStep = eDistAA;
            }
            break;
        case eDistCount:
            ucBuffer[2] = uCur;
            readDataBuffer(&uCur, 1);
  
            ucBuffer[3] = uCur;

            memcpy(&nResCount, &ucBuffer[3], 1);
            if (nResCount == 160)
            {
                nDataLen = nResCount * 6;
                nStep = eDistData;
                bFindHead = true;
                nIndex = 4;
            }
            else {
                nStep = eDist55;
                nDataLen = 0;
            }
            break;
        default:
            break;
        }

        if (nStep == eDistData)
        {
            //delay(5);
            int nLen =  readDataBuffer(&ucBuffer[nIndex], nDataLen + 4);
            if ( nLen != nDataLen + 4 )
            {
                nIndex  += nLen;
                continue ;
            }

            if (!CheckSum(ucBuffer, nDataLen + 8))
            {
                //printf("dist CheckSum  failed!\r\n");
                nStep = eDist55;
                memset(ucBuffer, 0, sizeof(ucBuffer));
                //writeErrorCode(ERROR_CHECKSUM_FAIL);
                nStep = eDist55;
                nDataLen = 0;
                nIndex = 0;
                bFindHead = false;
                return -1;
            }
            else{
                memcpy(&data, ucBuffer, sizeof(PKGDATA));
                //printf("  add: %d, ucDN:%d,\r\n", data.ucAD, data.ucDN);
            }

            nStep = eDist55;
            nDataLen = 0;
            nIndex = 0;
            bFindHead = false;
            return 1;
        }
     };
     return 0;
}
