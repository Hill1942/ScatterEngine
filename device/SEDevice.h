#ifndef SE_SCATTER_DEVICE_H
#define SE_SCATTER_DEVICE_H

#include "../render/SEIDriver.h"
#include "SEDeviceCreationParameter.h"

namespace se
{
	class SEDevice
	{
	public:
		SEDevice(const SEDeviceCreationParameter& param);
		virtual ~SEDevice(void);

	protected:
		render::SE_DRIVER_TYPE m_driverType;
		int  m_clientWidth;
		int  m_clientHeight;
		bool m_bFullscreen;

		render::SEIDriver* m_renderDirver;
	};
}

#endif
