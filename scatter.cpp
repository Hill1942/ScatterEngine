#include "se_config.h"

#include "render/SEDriverTypes.h"
#include "device/SEDeviceCreationParameter.h"
#include "device/SEDevice.h"
#include "device/SEDeviceWin32.h"
#include "scatter.h"

namespace se
{
	SEDevice* CreateSEDevice(
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

		SEDevice* pDevice = nullptr;
#ifdef SE_WINDOWS_PLATFORM
		pDevice = new SEDeviceWin32(param);
#endif

		return pDevice;
	}


}