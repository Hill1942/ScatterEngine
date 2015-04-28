#ifndef SE_DEVICE_CREATEION_PARAM_H
#define SE_DEVICE_CREATEION_PARAM_H

#include "../render/SEDriverTypes.h"

namespace se
{
	struct SEDeviceCreationParameter
	{
		SEDeviceCreationParameter()
			:driverType(render::SE_DT_NULL),
			 clientWidth(800),
			 clientHeight(600),
			 fullscreen(false)
		{
			
		}

		render::SE_DRIVER_TYPE driverType;
		int clientWidth;
		int clientHeight;
		bool fullscreen;
	};
}
#endif
