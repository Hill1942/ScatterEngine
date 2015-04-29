#ifndef SE_DRIVER_H
#define SE_DRIVER_H


namespace se
{
	namespace render
	{
		class SEIDriver
		{
		public:
			SEIDriver() {}
			virtual ~SEIDriver() {}

			virtual void drawPixel() = 0;

			virtual void draw2DLine() = 0;
			virtual void draw3DLine() = 0;

			virtual void draw2DImage() = 0;
			virtual void draw2DRectangle() = 0;
		};
	}
}
#endif
