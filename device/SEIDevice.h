#ifndef SE_SCATTER_DEVICE_H
#define SE_SCATTER_DEVICE_H

#include "../render/SEIDriver.h"
#include "SEDeviceCreationParameter.h"

namespace se
{
	class SEIDevice
	{
	public:
		SEIDevice(const SEDeviceCreationParameter& param);
		virtual ~SEIDevice(void);

		void run();
		void drop();

	protected:
		render::SE_DRIVER_TYPE m_driverType;
		int  m_clientWidth;
		int  m_clientHeight;
		bool m_bFullscreen;

		render::SEIDriver* m_renderDirver;
	};
}

#endif
