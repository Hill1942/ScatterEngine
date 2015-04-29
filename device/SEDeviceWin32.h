#ifndef SE_SCATTER_DEVICE_WIN32_H
#define SE_SCATTER_DEVICE_WIN32_H

#include "SEDeviceCreationParameter.h"

namespace se
{
	class SEDeviceWin32: public SEDevice
	{
	public:
		SEDeviceWin32(const SEDeviceCreationParameter& param);
		~SEDeviceWin32(void);
	};
}

#endif
