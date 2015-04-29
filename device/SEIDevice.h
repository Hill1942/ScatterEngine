#ifndef SE_SCATTER_DEVICE_H
#define SE_SCATTER_DEVICE_H

#include "../render/SERenderTimer.h"
#include "../render/SEIDriver.h"
#include "SEDeviceCreationParameter.h"

namespace se
{
	class SEIDevice
	{
	public:
		SEIDevice(const SEDeviceCreationParameter& param);
		virtual ~SEIDevice(void);

		virtual void run() = 0;
		virtual void drop() = 0;

	protected:
		
		int  m_clientWidth;
		int  m_clientHeight;
		bool m_bFullscreen;

		bool m_bPaused;
		bool m_bMinimized;
		bool m_bMaximized;


		render::SERenderTimer  m_renderTimer;
		render::SE_DRIVER_TYPE m_driverType;
		render::SEIDriver*     m_renderDirver;
	};
}

#endif
