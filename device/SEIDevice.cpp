#include "SEIDevice.h"




namespace se
{
	SEIDevice::SEIDevice(const SEDeviceCreationParameter& param)
	{
		this->m_driverType   = param.driverType;
		this->m_clientWidth  = param.clientWidth;
		this->m_clientHeight = param.clientHeight;
		this->m_bFullscreen   = param.fullscreen;
	}


	SEIDevice::~SEIDevice(void)
	{
	}

}
