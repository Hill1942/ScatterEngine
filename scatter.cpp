#include "scatter.h"

namespace se
{
	SEIDevice* CreateSEDevice(
		render::SE_DRIVER_TYPE driverType, 
		int clientWidth,
		int clientHeight,
		bool fullscreen)
	{
		SEDeviceCreationParameter param;
		param.driverType = driverType;
		param.clientWidth = clientWidth;
		param.clientHeight = clientHeight;
		param.fullscreen = fullscreen;

		SEIDevice* pDevice = nullptr;
#ifdef SE_WINDOWS_PLATFORM
		pDevice = new SEDeviceWin32(param);
#endif

		return pDevice;
	}


}