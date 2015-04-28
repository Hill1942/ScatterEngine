#ifndef SCATTER_H
#define SCATTER_H

namespace se
{
	SEDevice* CreateSEDevice(
		render::SE_DRIVER_TYPE driverType, 
		int clientWidth,
		int clientHeight,
		bool fullscreen);
}

#endif