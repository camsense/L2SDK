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
        printf("port is null, errorcode:%d\r\n", ERROR_INIT_FAIL);
        writeErrorCode(ERROR_INIT_FAIL);
        return false;
    }
    printf("chPort: %s, iBaute:%d\n", chPort, iBaute);
    if( m_serial.openDevice(chPort, iBaute) != 1){
        printf(" open device is failed! errorcode:%d\n", ERROR_INIT_FAIL);
        writeErrorCode(ERROR_INIT_FAIL);
        return false;
    }

    std::vector<DeviceInfo>().swap(m_vcDeviceInfo);

    m_thread_enable = true;
    delay(1);

#if 0
    std::vector<unsigned char>().swap(m_lstBuff);
    if (m_threadCmd.joinable())
        m_threadCmd.join();
    if (m_threadParse.joinable())
        m_threadParse.join();
    m_threadCmd = std::thread(&CBase::ThreadCmd, this);
    m_threadParse = std::thread(&CBase::ThreadParse, this);


    if (!StopScanCmd())
    {
        printf(" stop device failed!\n" );
        printf(" errorcode:%d\n",ERROR_STOPSCAN_CMD_FAIL );
        writeErrorCode(ERROR_STOPSCAN_CMD_FAIL);
        return false;
    }
     delay(15);
    if (!getDeviceAddrCmd())
    {
        printf(" get addr cmd failed! errorcode:%d\r\n",ERROR_ADDR_CMD_FAIL );
        writeErrorCode(ERROR_ADDR_CMD_FAIL);
        return false;
    }
    delay(15);
    if (!getDeviceInfoCmd())
    {
        printf(" get device info failed! errorcode:%d\n",ERROR_INFO_CMD_FAIL );
        writeErrorCode(ERROR_INFO_CMD_FAIL);
        return false;
    }
    m_bInit = true;
    delay(1000);
    for(int i = 0; i < m_uDeviceAddr; i++){
        DeviceInfo info;
        stDeviceInfoPkg stDeviceInfoTemp = m_arrDeviceInfo[i];
        //addr
        info.addr = stDeviceInfoTemp.ucAddr;

        char buf[128] = {0};
        //ID 
        for (int j = 0; j < 15; j++)
        {
            sprintf(&buf[j], "%c", stDeviceInfoTemp.ucSN[j]);
        }
        info.deviceSN = buf;
        
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
    memset(m_arrDeviceInfo, 0, sizeof(m_arrDeviceInfo));
    
    m_thread_enable = false;
    delay(5);
#endif
    m_bInit = true;
    return true;
}

bool CBase::uninit()
{
    std::vector<DeviceInfo>().swap(m_vcDeviceInfo);
    memset(m_arrDeviceInfo, 0, sizeof(m_arrDeviceInfo));
    m_thread_enable = false;

    if (m_threadCmd.joinable())
			m_threadCmd.join();
    if (m_threadParse.joinable())
			m_threadParse.join();
    
    clearPointBuffer();
    clearErrorCoed();
    StopScanCmd();
    m_serial.closeDevice();
    std::vector<unsigned char>().swap(m_lstBuff);
    if(m_bInit){
        return true;
    }
    return false;
}


bool CBase::configDevices()
{
    if (!m_bInit){
        return false;
    }
    m_thread_enable = true;
    delay(1);

#ifdef __linux__
    std::vector<unsigned char>().swap(m_lstBuff);
    m_threadCmd = std::thread(&CBase::ThreadCmd, this);
    m_threadParse = std::thread(&CBase::ThreadParse, this);

    if (!StopScanCmd())
    {
        printf(" stop device failed!\n");
        printf(" errorcode:%d\n", ERROR_STOPSCAN_CMD_FAIL);
        writeErrorCode(ERROR_STOPSCAN_CMD_FAIL);
        return false;
    }
    //StopScanCmd();
    m_serial.flushReceiver();
    delay(200);
    if (!getDeviceAddrCmd())
    {
        printf(" get addr cmd failed! errorcode:%d\r\n", ERROR_ADDR_CMD_FAIL);
        writeErrorCode(ERROR_ADDR_CMD_FAIL);
        return false;
    }
    delay(100);
    if (!getDeviceInfoCmd())
    {
        printf(" get device info failed! errorcode:%d\n", ERROR_INFO_CMD_FAIL);
        writeErrorCode(ERROR_INFO_CMD_FAIL);
        return false;
    }

    delay(150);
    for (int i = 0; i < m_uDeviceAddr; i++) {
        DeviceInfo info;
        stDeviceInfoPkg stDeviceInfoTemp = m_arrDeviceInfo[i];
        //addr
        info.addr = stDeviceInfoTemp.ucAddr;

        char buf[128] = { 0 };
        //ID 
        for (int j = 0; j < 15; j++)
        {
            sprintf(&buf[j], "%c", stDeviceInfoTemp.ucSN[j]);
        }
        info.deviceSN = buf;

        //
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%c%c", stDeviceInfoTemp.ucManuInfo[0], stDeviceInfoTemp.ucManuInfo[1]);
        info.factoryInfo = buf;

        //productName
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%c%c%c%c", stDeviceInfoTemp.ucProductInfo[0], stDeviceInfoTemp.ucProductInfo[1],
         stDeviceInfoTemp.ucProductInfo[2] , stDeviceInfoTemp.ucProductInfo[3]);
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
    memset(m_arrDeviceInfo, 0, sizeof(m_arrDeviceInfo));

    m_thread_enable = false;

    if (m_threadCmd.joinable())
    {
        m_threadCmd.join();
    }

    if (m_threadParse.joinable())
    {
        m_threadParse.join();
    }
    delay(5);
#else
    int nCount = 0;
    while (nCount < 5)
    {
        nCount++;
        m_serial.flushReceiver();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        unsigned char  cData[4] = { 0x55, 0x0D, 0x00, 0x62 };
        if (m_serial.writeData2(cData, sizeof(cData)) < 1)
        {
            Sleep(10);
            continue;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        double dBigen = m_serial.GetTimeStamp() / 1.0e6;
        std::vector<unsigned char> veTemp;
        while (true)
        {
            unsigned char  cBuff = 0;
            if (m_serial.readData(&cBuff, 1) <= 0)
            {
                if (veTemp.size() >= 4)
                {
                    unsigned char ucSum = (veTemp[veTemp.size() - 4] + veTemp[veTemp.size() - 3] + veTemp[veTemp.size() - 2]);
                    //校验 
                    if (ucSum != veTemp[veTemp.size() - 1])
                    {
                        break;
                    }
                    if (veTemp[veTemp.size() - 4] == 0x55 && veTemp[veTemp.size() - 3] == 0x0D)
                    {
                        break;
                    }

                }
                break;
            }
            else {
                veTemp.push_back(cBuff);
            }

            double dEnd = m_serial.GetTimeStamp() / 1.0e6;
            if (dEnd - dBigen > 500)
            {
                printf("read  Stop cmd timeout!");
                break;
            }
            continue;
        }
        continue;;
    }

    delay(200);
    unsigned char  cData[4] = { 0x55, 0x0A,0x00, 0x5F };
    if (m_serial.writeData2(cData, sizeof(cData)) < 1)
    {
        printf("write DeviceAddrCmd failed!");
        return false;
    }

    int nLen = 4;
    unsigned char  cBuff[4] = { 0 };
    if (m_serial.readData(cBuff, nLen, 30) != nLen)
    {
        printf("read  Device Addr failed!");
        return false;
    }
    unsigned char ucSum = (cBuff[0] + cBuff[1] + cBuff[2]);
    //У�� 
    if (ucSum != cBuff[3])
    {
        printf("CheckSum failed!");
        return false;
    }

    if (cBuff[0] == 0x55 && cBuff[1] == 0x0A)
    {
        int addr = cBuff[2];
        printf("get Device Add: %d \r\n", addr);
        m_uDeviceAddr = addr;


        if (m_uDeviceAddr == 0x00)
        {
            //printf("无设备地址，请先获取设备地址！");
            return false;
        }

        delay(15);

        unsigned char  cData[4] = { 0x55, 0x0B,0x00,0x60 };
        if (m_serial.writeData2(cData, sizeof(cData)) < 1)
        {
            printf("write getDeviceInfoCmd failed!");
            return false;
        }

        for (int i = 0; i < m_uDeviceAddr; i++) {
            stDeviceInfoPkg stDeviceInfoTemp;
            memset(&stDeviceInfoTemp, 0, sizeof(stDeviceInfoTemp));
            int nLen = sizeof(stDeviceInfoTemp);
            unsigned char  cBuff[255] = { 0 };
            if (m_serial.readData(cBuff, nLen) != nLen)
            {
                printf("read  getDeviceInfoCmd failed!");
                return false;
            }

            unsigned char ucSum = 0;
            //У�� 
            for (int n = 0; n < nLen - 1; n++)
            {
                ucSum += cBuff[n];
            }

            memcpy(&stDeviceInfoTemp, cBuff, nLen);
            if (ucSum != stDeviceInfoTemp.ucSum)
            {
                printf("getDeviceInfoCmd CheckSum failed!");
                return false;
            }


            DeviceInfo info;
            //addr
            info.addr = stDeviceInfoTemp.ucAddr;

            char buf[128] = { 0 };
            //ID 
            for (int j = 0; j < 15; j++)
            {
                sprintf(&buf[j], "%c", stDeviceInfoTemp.ucSN[j]);
            }
            info.deviceSN = buf;

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
    printf("get Device Addr failed!");
    return false;
#endif
 
    return true;
}

bool CBase::getDeviceAddrCmd()
{
    unsigned char  cData[4] = { 0x55, 0x0A,0x00, 0x5F };
    #if 0
    if (m_serial.writeData2(cData, sizeof(cData)) != sizeof(cData))
    {
        printf("write DeviceAddrCmd failed!\n");
        writeErrorCode(ERROR_ADDR_CMD_FAIL);
        return false;
    }
    delay(100);
    #else
    return sendData(cData, sizeof(cData));
    #endif
    return  true;
   
}

bool CBase::getDeviceInfoCmd()
{
     unsigned char  cData[4] = { 0x55, 0x0B,0x00,0x60 };
     #if 0
    if (m_serial.writeData2(cData, sizeof(cData)) < sizeof(cData))
    {
        printf("write getDeviceInfoCmd failed! \r\n");
        writeErrorCode(ERROR_INFO_CMD_FAIL);
        return false;
    }
    #else
    return sendData(cData, sizeof(cData));
    #endif
    return true;
}

bool CBase::StartScanCmd()
{
    delay(15);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    m_serial.flushReceiver();
    unsigned char  cData[4] = { 0x55, 0x0C, 0x00, 0x61 };
    if (m_serial.writeData2(cData, sizeof(cData)) < 1) 
    {
        printf("Start ScanCmd failed!");
        return false;
    }

    m_thread_enable = false;
    //delay(100);
    
    m_bStart = true;
    m_thread_enable = true;
    m_serial.flushReceiver();

    m_thread_enable = true;

    std::thread readThead(&CBase::readDataThread, this);
    readThead.detach();
    std::thread parseThead(&CBase::parseDataThread, this);
    parseThead.detach();

    return true;
}

bool CBase::StopScanCmd()
{
    m_bFindHead = true;
    m_bDone = true;
    clearPointBuffer();
    clearErrorCoed();
    unsigned char  cData[] = { 0x55, 0x0D, 0x00, 0x62 }; 
    //m_serial.flushReceiver();
    #if 0
    if(m_serial.writeData2(cData, sizeof(cData)) != sizeof(cData)){
        writeErrorCode(ERROR_STOPSCAN_CMD_FAIL);
        return false;
    }
    delay(200);
    #else
    return sendData(cData, sizeof(cData));
    #endif
    return true;
}

void CBase::getFps(float &fps)
{
    std::unique_lock<std::mutex> uclk(mtx_data);
    fps = m_fps;
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
    return len;
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

/*************************************************
@Function: 				getBinSize()
@Description: 			获取升级文件bin的大小.
@Input:					path - 路径+文件名
@Output:  				size - 文件大小
@Return: 				false表示失败, true表示成功
@Others:
*************************************************/
bool CBase::getBinSize(const char* path, unsigned int& size)
{
    FILE* fp = fopen(path, "rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        return true;
    }

    return false;
}

/*************************************************
@Function: 				getBinData()
@Description: 			获取升级文件bin的数据.
@Input:					path - 路径+文件名
                        size - 文件大小
@Output:  				data - 升级文件的数据
@Return: 				false表示失败, true表示成功
@Others:
*************************************************/
bool CBase::getBinData(const char* path, unsigned char* data, unsigned int size)
{
    FILE* fp = fopen(path, "rb");
    if (fp)
    {
        fread(data, sizeof(unsigned char), size, fp);
        fclose(fp);
        return true;
    }

    return false;
}

bool CBase::upgradeStart(const UINT8 addr)
{
    m_isResponseCommand = false;
    UPGRDE_FARMWARE_PKG pPkg;
    memset(&pPkg, 0, sizeof(pPkg));
    pPkg.pkg_head.head = 0x0E55;
    pPkg.pkg_head.id = addr;
    pPkg.pkg_head.err = 0;
    pPkg.pkg_head.update_cmd = UPDATE_CMD_START;
    pPkg.pkg_head.seq_k = 0;

    pPkg.filter = MAGIC_GD_L2;
    pPkg.descriptor = 0;
    pPkg.fw_checksum = 0;

    int size = sizeof(pPkg);
    unsigned char ucBuff[1024] = { 0 };
    memcpy(ucBuff, &pPkg, size);

    unsigned short uSum = 0;
    for (int i = 0; i < size - 2; i++)
    {
        uSum += ucBuff[i];
    }
    ucBuff[size - 2] = uSum;
    ucBuff[size - 1] = uSum >> 8;

    if (m_serial.writeData2(ucBuff, size) < 1)
    {
        std::cout << "upgradeStart cmd write failed!\n";
        return false;
    }
    delay(100);
    //等待回复
    double timeStart = m_serial.GetTimeStamp() / 1.0e6;
    while (true)
    {
        if (m_isResponseCommand)
        {
            return true;
        }

        double timeEnd = m_serial.GetTimeStamp() / 1.0e6;
        if (timeEnd - timeStart >= 1000.0)
        {
            std::cout << "no revice reply cmd data\n";
            return false;
        }
        delay(5);
    }
}

bool CBase::upgradeDataStart(const UINT8 addr)
{
    m_isResponseCommand = false;
    UPGRDE_FARMWARE_PKG pPkg;
    memset(&pPkg, 0, sizeof(pPkg));
    pPkg.pkg_head.head = 0x0E55;
    pPkg.pkg_head.id = addr;
    pPkg.pkg_head.err = 0;
    pPkg.pkg_head.update_cmd = UPDATE_CMD_DATA_START;
    pPkg.pkg_head.seq_k = 0;

    pPkg.filter = MAGIC_GD_L2;
    pPkg.descriptor = 0;
    pPkg.fw_checksum = 0;

    int size = sizeof(pPkg);
    unsigned char ucBuff[1024] = { 0 };
    memcpy(ucBuff, &pPkg, size);

    unsigned short uSum = 0;
    for (int i = 0; i < size - 2; i++)
    {
        uSum += ucBuff[i];
    }
    ucBuff[size - 2] = uSum;
    ucBuff[size - 1] = uSum >> 8;

    return  sendUpgradeData(ucBuff, size);

#if 0
    if (m_serial.writeData2(ucBuff, size) < 1)
    {
        printf("upgradeStart cmd write failed!\n");
        return false;
    }
    delay(10);
    double timeStart = m_serial.GetTimeStamp() / 1.0e6;
    while (true)
    {
        if (m_isResponseCommand)
        {
            return true;
        }

        double timeEnd = m_serial.GetTimeStamp() / 1.0e6;
        if (timeEnd - timeStart >= 1000.0)
        {
            std::cout << "no revice reply cmd data\n";
            return false;
        }
        delay(5);
    }

    return false;
#endif
}

bool CBase::upgradeData(const UINT8 addr, unsigned char data[], const unsigned short len, UINT8 uIndex)
{
    m_isResponseCommand = false;
    if (len > 2048)
    {
        printf("updata len out\n");
        return false;
    }
    unsigned char ucBuff[1024*4] = { 0 };
    int iHeadSize = sizeof(update_head);
    update_head pkg_head;
    memset(&pkg_head, 0, iHeadSize);
    pkg_head.head = 0x0E55;
    pkg_head.id = addr;
    pkg_head.err = 0;
    pkg_head.update_cmd = UPDATE_CMD_DATA;
    pkg_head.seq_k = uIndex;
    memcpy(ucBuff, &pkg_head, iHeadSize);
    memcpy(&ucBuff[iHeadSize], &len, sizeof(unsigned short));
    memcpy(&ucBuff[iHeadSize + 2], data, len);

#if 0

    UPGRDE_DATA_PKG pPkg;
    memset(&pPkg, 0, sizeof(pPkg));
    pPkg.pkg_head.head = 0x0E55;
    pPkg.pkg_head.id = addr;
    pPkg.pkg_head.err = 0;
    pPkg.pkg_head.update_cmd = UPDATE_CMD_DATA;
    pPkg.pkg_head.seq_k = uIndex;

    pPkg.valid_len = len;
    memcpy(pPkg.data, data, len);

    int size = sizeof(pPkg);
 
    memcpy(ucBuff, &pPkg, size);
#endif
    unsigned short uSum = 0;
    int size = len + iHeadSize + 4;
    for (int i = 0; i < size - 2; i++)
    {
        uSum += ucBuff[i];
    }
    ucBuff[size - 2] = uSum;
    ucBuff[size - 1] = uSum >> 8;
    return sendUpgradeData(ucBuff, size);
}

bool CBase::upgradeDataEnd(const UINT8 addr, unsigned char data[], const unsigned short len, UINT8 uIndex)
{
    m_isResponseCommand = false;
    unsigned char ucBuff[1024 * 4] = { 0 };
    int iHeadSize = sizeof(update_head);
    update_head pkg_head;
    memset(&pkg_head, 0, iHeadSize);
    pkg_head.head = 0x0E55;
    pkg_head.id = addr;
    pkg_head.err = 0;
    pkg_head.update_cmd = UPDATE_CMD_DATA_END;
    pkg_head.seq_k = uIndex;
    memcpy(ucBuff, &pkg_head, iHeadSize);
    memcpy(&ucBuff[iHeadSize], &len, sizeof(unsigned short));
    memcpy(&ucBuff[iHeadSize + 2], data, len);
    unsigned short uSum = 0;
    int size = len + iHeadSize + 4;
    for (int i = 0; i < size - 2; i++)
    {
        uSum += ucBuff[i];
    }
    ucBuff[size - 2] = uSum;
    ucBuff[size - 1] = uSum >> 8;
    return sendUpgradeData(ucBuff, size);
#if 0

    UPGRDE_DATA_PKG pPkg;
    memset(&pPkg, 0, sizeof(pPkg));
    pPkg.pkg_head.head = 0x0E55;
    pPkg.pkg_head.id = addr;
    pPkg.pkg_head.err = 0;
    pPkg.pkg_head.update_cmd = UPDATE_CMD_DATA_END;
    pPkg.pkg_head.seq_k = uIndex;

    pPkg.valid_len = len;
    memcpy(pPkg.data, data, len);

    int size = sizeof(pPkg);
    unsigned char ucBuff[2048] = { 0 };
    memcpy(ucBuff, &pPkg, size);

    unsigned short uSum = 0;
    for (int i = 0; i < size - 2; i++)
    {
        uSum += ucBuff[i];
    }
    ucBuff[size - 2] = uSum;
    ucBuff[size - 1] = uSum >> 8;
#endif
}

bool CBase::upgradeReset(const UINT8 addr, const unsigned short FW_checksum)
{
    m_isResponseCommand = false;
    UPGRDE_FARMWARE_PKG pPkg;
    memset(&pPkg, 0, sizeof(pPkg));
    pPkg.pkg_head.head = 0x0E55;
    pPkg.pkg_head.id = addr;
    pPkg.pkg_head.err = 0;
    pPkg.pkg_head.update_cmd = UPDATE_CMD_REBOOT;
    pPkg.pkg_head.seq_k = 0;

    pPkg.filter = MAGIC_GD_L2;
    pPkg.descriptor = 0;
    pPkg.fw_checksum = m_pUPGRDE_FARMWARE_PKG.fw_checksum;

    int size = sizeof(pPkg);
    unsigned char ucBuff[1024] = { 0 };
    memcpy(ucBuff, &pPkg, size);

    unsigned short uSum = 0;
    for (int i = 0; i < size - 2; i++)
    {
        uSum += ucBuff[i];
    }
    ucBuff[size - 2] = uSum;
    ucBuff[size - 1] = uSum >> 8;

    return sendUpgradeData(ucBuff, size);
}

bool CBase::sendUpgradeData(unsigned char data[], const unsigned short len)
{
    int iCount = 0;
    while (iCount++ < 3)    //重发三次机制
    {
        if (m_serial.writeData2(data, len) < 1)
        {
            printf("upgradeStart cmd write failed!\n");
            delay(10);
            continue;
        }
        double timeStart = m_serial.GetTimeStamp() / 1.0e6;

        while (true)
        {
            if (m_isResponseCommand)
            {
                return true;
            }

            double timeEnd = m_serial.GetTimeStamp() / 1.0e6;
            if (timeEnd - timeStart >= 1000.0)
            {
                break;
            }
            delay(5);
        }
    }
    return false;

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
}

void CBase::clearPointBuffer(void)
{
    std::unique_lock<std::mutex> ulck(mtx_error);
    std::queue<stOutputPoint>().swap(m_quePointData);
#if 0
    while (!m_quePointData.empty())
    {
        m_quePointData.pop();
    };
#endif
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


void CBase::eraseBuff(std::vector<unsigned char> &lstG, int iLen)
{
    if (lstG.size() >= iLen)
		lstG.erase(lstG.begin(), lstG.begin() + iLen);
	else
		lstG.clear();

    {
         std::vector<UCHAR> tmp = lstG;
         lstG.swap(tmp);
    }
}

bool CBase::reviceStop(std::vector<unsigned char> &lstData)
{
    if(lstData.size() < 4){
        return false;
    } 
    unsigned char uSum = 0x00;
    uSum = lstData[0] + lstData[1] + lstData[2];
    
    return (uSum == lstData[3]) ? true : false; ;
}

bool CBase::reviceAddr(std::vector<unsigned char> &lstData)
{
     if(lstData.size() < 4){
        return false;
    } 
    unsigned char uSum = 0x00;
    uSum = lstData[0] + lstData[1] + lstData[2];
    if(uSum == lstData[3]){
          m_uDeviceAddr = lstData[2];
          printf("add:%d\n",m_uDeviceAddr );
          return true;
    }

    return  false; 
}

bool CBase::reviceDeviceInfo(std::vector<unsigned char> &lstData)
{
    stDeviceInfoPkg stDeviceInfoTemp;
    memset(&stDeviceInfoTemp, 0, sizeof(stDeviceInfoTemp));
    int nLen = sizeof(stDeviceInfoTemp);
    if(lstData.size()  < nLen) {
        return false;
    }

    memcpy(&stDeviceInfoTemp, &lstData[0], nLen);

    unsigned char ucSum = 0;
    for (int n = 0; n < nLen - 1; n++)
    {
        ucSum += lstData[n];
    }

    eraseBuff(lstData, nLen);

    if (ucSum != stDeviceInfoTemp.ucSum)
    {
        writeErrorCode(ERROR_INFO_CMD_FAIL);
        printf("getDeviceInfoCmd CheckSum failed!\r\n");
        return false;
    }

        if(stDeviceInfoTemp.ucAddr > 0 && stDeviceInfoTemp.ucAddr <4){
        m_arrDeviceInfo[stDeviceInfoTemp.ucAddr- 1] = stDeviceInfoTemp;
    }

    return true;
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

void CBase::ThreadCmd()
{
    m_thread_enable = true;
    //unsigned char unBuff [256]= {0x00};
    unsigned char unBuff[1024] = { 0 };
    while (m_thread_enable)
    {
        memset(unBuff, 0 , sizeof(unBuff));
        //int iRC = m_serial.readData(unBuff, sizeof(unBuff), 30);
        int iRC = m_serial.readData(unBuff, sizeof(unBuff), 30);
        if(iRC < 1){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
		    std::this_thread::yield();
            continue;
        }else{
            std::lock_guard<std::mutex> lock(m_mtxBuff);
            std::vector<unsigned char> vcTmp(std::begin(unBuff), std::begin(unBuff) + iRC);
            m_lstTemp.insert(m_lstTemp.end(), vcTmp.begin(), vcTmp.end());
        }
    };
    
}

void CBase::ThreadParse()
{
    std::vector<UCHAR>().swap(m_lstBuff);

    while (m_thread_enable)
    {
            std::lock_guard<std::mutex> lock(m_mtxBuff);
            if(m_lstTemp.size()>0)
            {
                std::vector<UCHAR>  lstTemp;
                lstTemp.swap(m_lstTemp);
                m_lstBuff.insert(m_lstBuff.end(), lstTemp.begin(), lstTemp.end());
            }   

            int cmd_step = eInit;
            unsigned char tmep = 0x00;
            int index = -1;
            int nCount = m_lstBuff.size();
            for(int i = 0; i < nCount -1;i++)
            {
                if(m_lstBuff[i] == HEAD && m_lstBuff[i + 1] == ADDR ){
                    cmd_step = eGetAddr;
                    index = i;
                    break;
                }else if(m_lstBuff[i] == HEAD && m_lstBuff[i + 1] == DEVICE ){
                    cmd_step = eGetInfo;
                    index = i;
                    break;
                }else if(m_lstBuff[i] == HEAD && m_lstBuff[i + 1] == STOP ){
                    cmd_step = eScanStop;
                    index = i;
                    break;
                }
            }

            if(index >= 0 )
            {
                    eraseBuff(m_lstBuff,index);
                    switch (cmd_step)
                    {
                    case eScanStop:
                        if (reviceStop(m_lstBuff))
                        {
                            /* code */
                            m_isResponseCommand = true;
                        }
                         eraseBuff(m_lstBuff,4);
                        break;
                    case eGetAddr:
                        if ( reviceAddr(m_lstBuff))
                        {
                            /* code */
                             m_isResponseCommand = true;
                        }
                        
                        eraseBuff(m_lstBuff,4);
                        break;
                    case eGetInfo:
                        if (reviceDeviceInfo(m_lstBuff))
                        {
                            /* code */
                            m_isResponseCommand = true;
                        }
                        break;
                    }
            }else
            {
                continue; 
            }
    };
}

//发送cmd数据
bool CBase::sendData(unsigned char data[], const int len)
{
     int iCount = 0;
    while (iCount++ < 3)    //重发三次机制
    {
        if (m_serial.writeData2(data, len) < 1)
        {
            printf(" cmd write failed!\n");
            delay(10);
            continue;
        }
        double timeStart = m_serial.GetTimeStamp() / 1.0e6;

        while (true)
        {
            if (m_isResponseCommand)
            {
                return true;
            }

            double timeEnd = m_serial.GetTimeStamp() / 1.0e6;
            if (timeEnd - timeStart >= 200.0)
            {
                break;
            }
            delay(5);
        }
    }
    return false;
}

//读取点云数据线程
void CBase::readDataThread()
{
    //delay(50);
    std::vector<unsigned char>().swap(m_lstTemp);
    while (m_thread_enable)
    {
#define DATA_LEN   1024
        unsigned char data[DATA_LEN] = { 0 };
        int iLen = m_serial.readData(data, DATA_LEN, 100);
        if (iLen <= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::this_thread::yield();
        }
        else {
            std::lock_guard<std::mutex> lck(m_mtxBuff);
            std::vector<unsigned char> vcTmp(std::begin(data), std::begin(data) + iLen);
            m_lstTemp.insert(m_lstTemp.end(), vcTmp.begin(), vcTmp.end());
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    };
}

//解析点云数据线程
void CBase::parseDataThread()
{
    std::vector<unsigned char>().swap(m_lstBuff);
    startTimes = m_serial.GetTimeStamp() / 1.0e6;
    while (m_thread_enable)
    {
        std::lock_guard<std::mutex> lck(m_mtxBuff);
        if (m_lstTemp.size() > 0) {
            std::vector<unsigned char> vcTemp;
            m_lstTemp.swap(vcTemp);
            m_lstBuff.insert(m_lstBuff.end(), vcTemp.begin(), vcTemp.end());
        }

        if (m_lstBuff.size() < 2) {
            continue;
        }
        int iSize = m_lstBuff.size();
        int iIndex = -1;
        int iType = 0;
        int iTimeSize = sizeof(TIMEPKG);
        int iPointSize = sizeof(PKGDATA);
        for (int i = 0; i < iSize - 1; i++)
        {
            if (TIME_HEAD_PKG0 == m_lstBuff[i] && TIME_HEAD_PKG1 == m_lstBuff[i + 1])
            {
                iIndex = i;
                iType = 0;
                if (iSize < iTimeSize)
                {
                    iIndex = -1;
                }
                break;
            }
            else if (POINT_HEAD_PKG0 == m_lstBuff[i] && POINT_HEAD_PKG1 == m_lstBuff[i + 1])
            {
                iIndex = i;

                if (iSize < iPointSize)
                {
                    iIndex = -1;
                }
                iType = 1;
                break;
            }
        }
        if (iIndex == -1)
        {
            continue;
        }
        if(m_uDeviceAddr == 3){
            //std::this_thread::sleep_for(std::chrono::milliseconds(7));
            std::this_thread::sleep_for(std::chrono::microseconds(7600));
            std::this_thread::yield();
        }else  if(m_uDeviceAddr == 2){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::this_thread::yield();
        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
            std::this_thread::yield();
        }
        eraseBuff(m_lstBuff, iIndex);
        switch (iType)
        {
        case 0:           //时间戳解析
            parseTime(m_lstBuff);
            eraseBuff(m_lstBuff, iTimeSize);
            break;
        case 1:           //点云解析
            parsePointData(m_lstBuff);
            eraseBuff(m_lstBuff, iPointSize);
            break;
        default:
            break;
        }
    }

}

//解析时间戳同步
bool CBase::parseTime(std::vector<unsigned char> &lstData)
{
    TIMEPKG temp;
    if (lstData.size() < sizeof(temp)) {
        return false;
    }
    if (!CheckSum(lstData.data(), sizeof(temp)))
    {
        return false;
    }
    memcpy(&temp, lstData.data(), sizeof(temp));
    m_u64DevicesTime = temp.usMs;
    std::unique_lock<std::mutex> lck(mtx_data);
    double endTime = m_serial.GetTimeStamp() / 1.0e6;
    m_iCount++;
    if (endTime - startTimes >= 1000.0)
    {
        m_fps = m_iCount;
        m_iCount = 0;
        startTimes = endTime;
        //printf("m_fps: %.2f\n", m_fps);
    }

    return true;
}

//解析点云
bool CBase::parsePointData(std::vector<unsigned char> &lstData)
{
    PKGDATA pPoint;
    if (lstData.size() < sizeof(pPoint)) {
        return false;
    }
    if (!CheckSum(lstData.data(), sizeof(pPoint)))
    {
        return false;
    }
    memcpy(&pPoint, lstData.data(), sizeof(pPoint));

    auto ts = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    UINT64 u64LocTime = ts / 1000;
    UINT64 uDeviceTime = m_u64DevicesTime;
    std::vector<POINTDATA> vcPoint;
    for (int i = 0; i < 160; i++)
    {
        POINTDATA pTemp;
        float x = (float)pPoint.data[i].x * 0.1;
        float y = (float)pPoint.data[i].y * 0.1;

        pTemp.bflag = pPoint.data[i].flag;
        pTemp.x = x;
        pTemp.y = y;
        pTemp.bfiter = pPoint.data[i].fiter;
        pTemp.quality = pPoint.data[i].quality;
        pTemp.row = pPoint.data[i].row;
        vcPoint.push_back(pTemp);
    }
    stOutputPoint outPoint;
    outPoint.u64LocTimeStampS = u64LocTime;
    outPoint.u64DeviceTimeStampS = uDeviceTime;
    outPoint.u64ExposureTimeStamp = pPoint.u64Ms;
    outPoint.uaddr = pPoint.ucAddr;
    outPoint.Point.swap(vcPoint);
    writePointBuffer(outPoint);

    return true;
}

//解析点云数据
bool CBase::parseImgData()
{
    int nRow = m_iRow;
    int nCOL = m_iCol;
    int nSize = nRow * nCOL;
    int nFps = 0;
    unsigned char data[nSize] = {0};
     startTimes = m_serial.GetTimeStamp() / 1.0e6;
    while (m_thread_enable)
    {
        int bRes = 0;
        bRes = paraseImgEx(data);
        if (bRes > 0) //是否读取数据
        {
             std::lock_guard<std::mutex> lckImg(m_mutImg);
            {
                memcpy(m_img_data.data,  data,  nRow * nCOL);
                m_isGetImg = true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::this_thread::yield();
    }

    m_thread_enable = false;
    //t.join();
    //readTh.join();
    printf("threadImage_ exited \r\n");
    return false;
}

//图片写入到磁盘（参数1：保存的文件名 参数2：要写的数据   参数3：数据长度）
int CBase::paraseImgEx(unsigned char *buff)
{
   int nRow = 128, nStartRow = 0, nCol = 160, nStartCol = 0;
    int nStep = 0;

    //寻找第一帧
    m_bFindHead = false;
    m_bDone = false;
    unsigned char uCur = 0xff;
    unsigned char buffer[1024] = { 0 };

    int nFailedCount = 0;
    int  nErrorCount = 0;

    while (!m_bFindHead)    //寻找第一帧
    {
        if ( m_serial.readData(&uCur, 1, 10) <= 0){
            nStep = 0;
            if (nErrorCount++ > nRow * nCol){
                nErrorCount = 0;
                return -1;
            }
            delay(1);
            continue;
        }
        else {
            nErrorCount = 0;
        }

        switch (nStep){
        case 0:
            if (0x55 == uCur){
                nStep = 1;
                buffer[0] = uCur;
            }  break;
        case 1:
            if (0xAF == uCur){
                buffer[1] = uCur;
                nStep = 2;
                m_serial.readData(&buffer[2], 14, 1000);  //起始行
                if (CheckSum(buffer, 16)){
                    memcpy(&nStartRow, &buffer[2], 2);
                    memcpy(&nRow, &buffer[4], 2);
                    memcpy(&nStartCol, &buffer[6], 2);
                    memcpy(&nCol, &buffer[8], 2);
                    m_bFindHead = true;
                }else {
                    nStep = 0;
                }
            }else {
                nStep = 0;
            }
            break;
        default:
            break;
        }

        if (nFailedCount++ > (nCol * nRow + 14)) {
            return -2;
        }
    }

    unsigned char* revBuff = new unsigned char[nRow * nCol];
    int nIndex = 0;

    int nTestIndex = 0;
    int imgSize = nRow * nCol;
    while (!m_bDone)    {
        int nCount = nCol;
        memset(buffer, 0, sizeof(buffer));
        int iLen = m_serial.readData(buffer, nCount, 100);
        if (iLen <= 0){
            delete[] revBuff;
            return -2;
        }else {
            if (nTestIndex +  iLen >= imgSize){
                memcpy(revBuff + nTestIndex, buffer, imgSize -  nTestIndex);
                nTestIndex  = imgSize;
            }else{
                memcpy(revBuff + nTestIndex, buffer, iLen);
                nTestIndex  += iLen;
            }
        }
        if (nTestIndex == imgSize){
            nTestIndex = 0;
            m_bDone = true;
            double clock = 0.0;
            memcpy(buff, revBuff, nCol * nRow * sizeof(unsigned char));
            delete[] revBuff;
            return 1;
        }
    }
    delete[] revBuff;
    return -1;
}

int CBase::upgradeBin(const char* path, const UINT8 addr)
{
    m_thread_enable = false;
    if (addr == 0 || addr > 3) {
        return -1;      //输入的地址错误
    }
    StopScanCmd();
    delay(10);
    if (m_threadCmd.joinable())
    {
        m_threadCmd.join();
    }
    if (m_threadParse.joinable())
    {
        m_threadParse.join();
    }
    setProgress(1);

    m_uDeviceAddr = 0x00;
    m_thread_enable = true;
    m_threadCmd = std::thread(&CBase::ThreadCmd, this);
    m_threadParse = std::thread(&CBase::ThreadParse, this);
   
    if (!getDeviceAddrCmd())
    {
        printf("send get ID cmd failed\n");
        return -2;            //串口发送命令失败
    }                       
    delay(20);

    setProgress(2);

    bool bUpgrade = true;
    int iAddr = m_uDeviceAddr;
    if (iAddr == 0x00){
        bUpgrade = false;
    }
    else {
        bUpgrade = true;
    }
    m_thread_enable = false;

    if (m_threadCmd.joinable())
    {
        m_threadCmd.join();
    }
    if (m_threadParse.joinable())
    {
        m_threadParse.join();
    }
    delay(10);
  
    ThreadUpgradeRun();
    setProgress(3);
    delay(30);
    unsigned int iSize = 0;
    if (!getBinSize(path, iSize)){
        printf("upgrade failed，read bin size failed！\n");
        UpgradeUninit();
        return -3;      //读取升级文件大小是失败
    }

    unsigned char* Buff = new unsigned char[iSize];
    if (!getBinData(path, Buff, iSize)){
        printf("upgrade failed，read bin data failed！\n");
        UpgradeUninit();
        return -4;       //读取bin文件数据失败
    }
    setProgress(5);

    if (bUpgrade && !upgradeStart(addr)){
        UpgradeUninit();
        printf("upgradeStart failed\n");
        if (Buff){
            delete Buff;
            Buff = nullptr;
        }
        return -5;              //升级开始步骤失败
    }
    delay(20);
    setProgress(10);
    if (!upgradeDataStart(0x00)){
        bUpgrade = false;
        UpgradeUninit();
        printf("upgradeDataStart  failed！\n");
        if (Buff){
            delete Buff;
            Buff = nullptr;    
        }
        return -6;               //升级数据开始步骤失败
    }
    delay(20);
    setProgress(20);

#define BIN_OFFSET 256
    //发送数据
    int iCount = iSize / BIN_OFFSET;
    int iOffset = 0;
    int iIndex = 0;
    while (iOffset < iSize){
        unsigned short usWriteSize = (iSize - iOffset) > BIN_OFFSET ? BIN_OFFSET : (iSize - iOffset);
        if (usWriteSize == BIN_OFFSET){
            if (upgradeData(0x00, &Buff[iOffset], usWriteSize, iIndex)){
                iOffset += usWriteSize;
                setProgress(20 + (float)iOffset / (float)iSize * 70);
                iIndex++;
            }
            else{
                UpgradeUninit();
                printf("upgradeData  failed！\n");
                if (Buff){
                    delete Buff;
                    Buff = nullptr;
                }
                bUpgrade = false;
                return -7;           //升级数据包步骤失败
            }
            delay(20);
        }
        else {

            if (upgradeDataEnd(0x00, &Buff[iOffset], usWriteSize, iIndex)){
                iOffset += usWriteSize;
                setProgress(20 + (float)iOffset / (float)iSize * 70);
            }
            else{
                UpgradeUninit();
                printf("upgradeData upgradeDataEnd failed！\n");
                if (Buff) {
                    delete Buff;
                    Buff = nullptr;
                }
                bUpgrade = false;
                return -8;          //升级数据结束步骤失败
            }
        }

    }

    delay(20);
    setProgress(95);
    unsigned short FW_checksum = 0;
    if (!upgradeReset(0x00, FW_checksum)){
        UpgradeUninit();
        bUpgrade = false;
        printf("upgradeReset  failed！\n");
        if (Buff){
            delete Buff;
            Buff = nullptr;
        }
        bUpgrade = true;

        return -9;                  // 升级校验重启失败
    }
    delay(20);

    if (Buff){
        delete Buff;
        Buff = nullptr;
    }
    bUpgrade = true;
    UpgradeUninit();
    setProgress(100);

    return true;
}

bool CBase::sendTimeStamp(const unsigned int ms)
{
    unsigned char buf[8] = { 0 };
    buf[0] = 0x55;
    buf[1] = 0x0F;
    memcpy(&buf[2], &ms, sizeof(unsigned int));
    unsigned short uSum = 0;
    for (int i = 0; i < 6; i++)
    {
        uSum += buf[i];
    }
    buf[6] = uSum;
    buf[7] = uSum >> 8;
    if (m_serial.writeData2(buf, 8) < 1)
    {
        return false;
    }

    return true;
}

//级联出图
bool CBase::sendImgCmdMode(const UINT8 uaddr)
{
     if (uaddr == 0 || uaddr > m_uDeviceAddr) {
        std::cout << "addr  error !\n";
        return false;      //输入的地址错误
    }
    m_bFindHead = true;
    m_bDone = true;
    m_thread_enable = false;
    m_serial.flushReceiver();

    delay(3200);
    
    unsigned char buf[4] = { 0 };
    buf[0] = 0x55;
    buf[1] = 0x10;
    buf[2] =  uaddr;
    unsigned short uSum = 0;
    for (int i = 0; i < 3; i++){
        uSum += buf[i];
    }
    buf[3] = uSum;
    if (m_serial.writeData2(buf, 4) < 1){
        return false;
    }

    m_thread_enable = true;
    //std::thread readImgThead(&CBase::readDataThread, this);
    //readImgThead.detach();
    std::thread parseImgThead(&CBase::parseImgData, this);
    parseImgThead.detach();

    return true;
}

bool CBase::getImgData(stImgData &img)
{
    if (m_isGetImg)
    {
        std::lock_guard<std::mutex> lck(m_mutImg);
        memcpy(img.data, m_img_data.data, m_iRow*m_iCol);
        memset(m_img_data.data, 0, sizeof(stImgData));
        m_isGetImg  = false;
        return true;
    }
    return false;
}

//启动升级
void CBase::ThreadUpgradeRun()
{
    if (m_threadUpgradRead.joinable())
    {
        m_threadUpgradRead.joinable();
    }
    if (m_threadUpgradParse.joinable())
    {
        m_threadUpgradParse.joinable();
    }

    m_thread_enable = true;
    m_threadUpgradRead = std::thread(&CBase::readUpgradeThead, this);
    m_threadUpgradParse = std::thread(&CBase::parseUpgradeThead, this);
}

//读取升级数据线程
void CBase::readUpgradeThead()
{
    std::vector<unsigned char>().swap(m_lstUpgrdeTemp);
    while (m_thread_enable)
    {
        unsigned char data[1024] = { 0 };
        int iLen = m_serial.readData(data, 1024, 100);
        if (iLen <= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::this_thread::yield();
        }
        else {
            std::lock_guard<std::mutex> lck(m_mutUpgrdeBuff);
            {
                std::vector<unsigned char> vcTmp(std::begin(data), std::begin(data) + iLen);
                m_lstUpgrdeTemp.insert(m_lstUpgrdeTemp.end(), vcTmp.begin(), vcTmp.end());
            }
           
        }
    };
}

//解析升级数据线程
void CBase::parseUpgradeThead()
{
    std::vector<unsigned char>().swap(m_lstUpgrdeBuff);
    while (m_thread_enable)
    {
        std::lock_guard<std::mutex> lck(m_mutUpgrdeBuff);
        if (m_lstUpgrdeTemp.size() > 0)
        {
            std::vector<unsigned char> vcTemp;
            m_lstUpgrdeTemp.swap(vcTemp);
            m_lstUpgrdeBuff.insert(m_lstUpgrdeBuff.end(), vcTemp.begin(), vcTemp.end());
        }
        int iSize = m_lstUpgrdeBuff.size();
        int iIndex = -1;
        for (int i = 0; i < iSize - 1; i++)
        {
            if (UPGRDE_HEAD_PKG0 == m_lstUpgrdeBuff[i] && UPGRDE_HEAD_PKG1 == m_lstUpgrdeBuff[i + 1])
            {
                if (iSize - i >= sizeof(UPGRDE_FARMWARE_PKG))
                {
                    iIndex = i;
                    break;
                }
                else {
                    iIndex = -1;
                    break;
                }

            }
        }

        if (iIndex == -1)
        {
            continue;
        }

        eraseBuff(m_lstUpgrdeBuff, iIndex);
        m_isResponseCommand = parseUpgradeData(m_lstUpgrdeBuff);
        eraseBuff(m_lstUpgrdeBuff, sizeof(UPGRDE_FARMWARE_PKG));

    };
}

//解析升级命令数据
bool CBase::parseUpgradeData(std::vector<unsigned char> &lstData)
{
    if (lstData.size() < sizeof(UPGRDE_FARMWARE_PKG))
    {
        return false;
    }

    if (UPGRDE_HEAD_PKG0 != lstData[0] || UPGRDE_HEAD_PKG1 != lstData[1])
    {
        return false;
    }

    int iSize = sizeof(UPGRDE_FARMWARE_PKG);
    unsigned short uSum = 0;
    for (int i = 0; i < iSize - 2; i++)
    {
        uSum += lstData[i];
    }

    memcpy(&m_pUPGRDE_FARMWARE_PKG, lstData.data(), sizeof(m_pUPGRDE_FARMWARE_PKG));
    if (uSum == m_pUPGRDE_FARMWARE_PKG.pck_checksum)
    {
        //printf("addr:%d, cmd:%d\n", m_pUPGRDE_FARMWARE_PKG.pkg_head.id, m_pUPGRDE_FARMWARE_PKG.pkg_head.update_cmd);
        printf("err:%d, seq_k: %d\n", m_pUPGRDE_FARMWARE_PKG.pkg_head.err, m_pUPGRDE_FARMWARE_PKG.pkg_head.seq_k);
        printf("filter:%d,fw_checksum: %d \n", m_pUPGRDE_FARMWARE_PKG.filter, m_pUPGRDE_FARMWARE_PKG.fw_checksum);

        if (m_pUPGRDE_FARMWARE_PKG.pkg_head.err > 0)
        {
            ErrorCode code;
            switch (m_pUPGRDE_FARMWARE_PKG.pkg_head.err)
            {
            case 1:
                code = ERROR_UPDATE_CHECKSUM;
                break;
            case 2:
                code = ERROR_UPDATE_FILTER;
                break;
            case 3:
                code = ERROR_UPDATE_SEQ;
                break;
            case 4:
                code = ERROR_UPDATE_FW_CHECKSUM;
                break;
            default:
                break;
            }
            writeErrorCode(code);
            return false;
        }

        return true;
    }
    return false;
}

//退出升级
void CBase::UpgradeUninit()
{
    m_thread_enable = false;

    if (m_threadUpgradRead.joinable())
    {
        m_threadUpgradRead.join();
    }

    if (m_threadUpgradParse.joinable())
    {
        m_threadUpgradParse.join();
    }
}
