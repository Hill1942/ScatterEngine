#ifndef SE_D3D11DRIVER_H
#define SE_D3D11DRIVER_H

#include "SENullDriver.h"
#include <string>
#include <d3d11.h>

namespace se
{
	namespace render
	{
		class SED3D11Driver: public SENullDriver
		{
		public:
			SED3D11Driver();
			virtual ~SED3D11Driver();

			bool initDriver(HWND hwnd);

			void drawPixel() override;

			void draw2DLine() override;
			void draw3DLine() override;

			void draw2DImage() override;
			void draw2DRectangle() override;

		private:
			HINSTANCE m_hAppInstancce;
			HWND      m_hMainWnd;
			bool      m_bAppPaused;
			bool      m_bMinimized;
			bool      m_bMaximized;
			bool      m_Resizing;
			UINT      m_4xMsaaQuality;

			ID3D11Device*           m_d3dDevice;
			ID3D11DeviceContext*    m_d3dImmediateContext;
			D3D_DRIVER_TYPE         m_d3dDriverType;
			IDXGISwapChain*         m_SwapChain;
			ID3D11Texture2D*        m_DepthStencilBuffer;
			ID3D11RenderTargetView* m_RenderTargetView;
			ID3D11DepthStencilView* m_DepthStencilView;
			D3D11_VIEWPORT          m_ScreenViewport;


			std::wstring m_MainWndCaption;
			int  m_ClientWidth;
			int  m_ClientHeight;
			bool m_Enable4xMsaa;
		};
	}
}
#endif
