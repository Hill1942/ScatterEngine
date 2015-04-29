#ifndef SE_DRIVER_H
#define SE_DRIVER_H


namespace se
{
	namespace render
	{
		#define ReleaseCOM(x) { if (x) { x->Release(); x = 0;} }

		class SEIDriver
		{
		public:
			SEIDriver() {}
			virtual ~SEIDriver() {}
			virtual void drop() = 0;

			virtual void beginScene() = 0;
			virtual void endScene() = 0;
			virtual void onResize() = 0;

			virtual void drawPixel() = 0;

			virtual void draw2DLine() = 0;
			virtual void draw3DLine() = 0;

			virtual void draw2DImage() = 0;
			virtual void draw2DRectangle() = 0;
		};
	}
}
#endif
