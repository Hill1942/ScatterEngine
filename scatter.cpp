#include "render/SEDriverTypes.h"
#include "device/SEDeviceCreationParameter.h"
#include "device/SEDevice.h"
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

		SEDevice* pDevice = 0;
		pDevice = new SEDevice(param);
	}
}