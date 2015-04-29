#ifndef SCATTER_H
#define SCATTER_H

#include <string>

#include "se_config.h"
#ifdef SE_WINDOWS_PLATFORM
#include <windows.h>
#endif

#ifdef SE_APPLE_PLATFORM
#endif

#ifdef SE_LINUX_PLATFORM
#endif

#ifdef SE_ANDROID_PLATFORM
#endif

#include "render/SEDriverTypes.h"
#include "device/SEDeviceCreationParameter.h"
#include "device/SEDevice.h"
#include "device/SEDeviceWin32.h"

namespace se
{
	SEDevice* CreateSEDevice(
		render::SE_DRIVER_TYPE driverType, 
		int clientWidth,
		int clientHeight,
		bool fullscreen);
}

#endif