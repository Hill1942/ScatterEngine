#include "SEDevice.h"




namespace se
{
	SEDevice::SEDevice(const SEDeviceCreationParameter& param)
	{
		this->m_driverType   = param.driverType;
		this->m_clientWidth  = param.clientWidth;
		this->m_clientHeight = param.clientHeight;
		this->m_bFullscreen   = param.fullscreen;
	}


	SEDevice::~SEDevice(void)
	{
	}
}
