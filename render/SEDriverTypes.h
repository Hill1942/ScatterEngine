#ifndef SE_DRIVER_TYPES_H
#define SE_DRIVER_TYPES_H

namespace se
{
	namespace render
	{
		enum SE_DRIVER_TYPE
		{
			SE_DT_NULL,
			SE_DT_SOFTWARE,
			SE_DT_DIRECTX9,
			SE_DT_DIRECTX11,
			SE_DT_DIRECTX12,
			SE_DT_OPENGL_LEGACY,
			SE_DT_OPENGL
		};
	}
}

#endif