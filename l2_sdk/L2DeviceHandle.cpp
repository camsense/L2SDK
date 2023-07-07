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

bool CL2DeviceHandle::init(const char *chPort,  int iBaud)
{
    return m_pBase.init(chPort, iBaud);
}

bool CL2DeviceHandle::uinit()
{
    return m_pBase.uninit();
}

