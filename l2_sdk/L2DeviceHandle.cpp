#include "L2DeviceHandle.h"
CL2DeviceHandle::CL2DeviceHandle(){

}

CL2DeviceHandle::~CL2DeviceHandle(void){
    //m_pBase.uninit();
}

CL2DeviceHandle& CL2DeviceHandle::Instance(){
	static CL2DeviceHandle Driver;
	return Driver;
}

char* CL2DeviceHandle::getSDKVersion(){
    return (char*) m_strSdkVersion.c_str();
}

UINT8 CL2DeviceHandle::getNumberOfDevice()
{
    m_pBase.configDevices();
    return m_pBase.getDeviceNum();
}

bool CL2DeviceHandle::starScanDevice()
{
    return m_pBase.StartScanCmd();
}

bool CL2DeviceHandle::stopScanDevice()
{
    m_pBase.setEnable(false);
    return m_pBase.StopScanCmd();
}

bool CL2DeviceHandle::allStratScanDevice()
{
    return m_pBase.AllStartScanCmd();
}

void CL2DeviceHandle::getFps(float& fps)
{
    m_pBase.getFps(fps);
}

void CL2DeviceHandle::getPointData(std::vector<stOutputPoint>& data)
{
    m_pBase.getPointData(data, m_pBase.getDeviceNum());
}

void CL2DeviceHandle::getErrorCode(ErrorCode& error)
{
    error = m_pBase.getErrorCode();
}

bool CL2DeviceHandle::getInfo(std::vector<DeviceInfo>& info)
{
    return m_pBase.getDeviceInfo(info);
}

int CL2DeviceHandle::upgradeBin(const char* path, const UINT8 addr)
{
    return m_pBase.upgradeBin(path, addr);
}

void CL2DeviceHandle::getUpgradeProgress(float& percentage)
{
    m_pBase.getProgress(percentage);
}

bool CL2DeviceHandle::sendTimeStamp(const unsigned int ms)
{
   return m_pBase.sendTimeStamp(ms);
}

bool CL2DeviceHandle::sendImgMode(const UINT8 addr)
{
    m_pBase.StopScanCmd();
    return m_pBase.sendImgCmdMode(addr);
}

bool CL2DeviceHandle::getImgData(stImgData &img)
{
    return m_pBase.getImgData(img);
}

bool CL2DeviceHandle::init(const char *chPort,  int iBaud)
{
    return m_pBase.init(chPort, iBaud);
}

bool CL2DeviceHandle::uinit()
{
    return m_pBase.uninit();
}

