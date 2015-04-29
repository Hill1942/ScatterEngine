#ifndef SE_D3D11DRIVER_H
#define SE_D3D11DRIVER_H

#include "SENullDriver.h"

namespace se
{
	namespace render
	{
		class SED3D11Driver: public SENullDriver
		{
		public:
			SED3D11Driver();
			virtual ~SED3D11Driver();

			void drawPixel() override;

			void draw2DLine() override;
			void draw3DLine() override;

			void draw2DImage() override;
			void draw2DRectangle() override;
		};
	}
}
#endif
