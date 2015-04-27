#ifndef SE_D3D11DRIVER_H
#define SE_D3D11DRIVER_H


namespace se
{
	namespace render
	{
		class SED3D11Driver: public SENullDriver
		{
			SED3D11Driver();
			virtual ~SED3D11Driver();

			virtual void drawPixel();

			virtual void draw2DLine();
			virtual void draw3DLine();

			virtual void draw2DImage();
			virtual void draw2DRectangle();
		};
	}
}
#endif
