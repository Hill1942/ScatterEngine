#ifndef SE_D3D11DRIVER_H
#define SE_D3D11DRIVER_H

#include <cassert>
#include "d3d11_1.h"
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

			void beginScene();
			void endScene();

			void drawPixel() override;

			void draw2DLine() override;
			void draw3DLine() override;

			void draw2DImage() override;
			void draw2DRectangle() override;

			void onResize() override;
			void drop() override;

		private:
			void InitD3D11Driver();

		private:
			ID3D11Device*           m_d3dDevice;
			ID3D11DeviceContext*    m_d3dImmediateContext;
			IDXGISwapChain*         m_SwapChain;
			ID3D11Texture2D*        m_depthStencilBuffer;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11DepthStencilView* m_depthStencilView;
			D3D11_VIEWPORT          m_screenViewport;
			UINT                    m_4xMsaaQuality;
			bool                    m_bEnable4xMsaa;

			int                     m_clientWidth;
			int                     m_clientHeight;
			HWND                    m_hMainWnd;
		};
	}
}
#endif
