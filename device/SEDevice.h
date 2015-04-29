#ifndef SE_SCATTER_DEVICE_H
#define SE_SCATTER_DEVICE_H

#include "SEDeviceCreationParameter.h"

namespace se
{
	class SEDevice
	{
	public:
		SEDevice(const SEDeviceCreationParameter& param);
		~SEDevice(void);
	};
}

#endif
