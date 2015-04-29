#include "SED3D11Driver.h"

namespace se
{
	namespace render
	{
		SED3D11Driver::SED3D11Driver()
		{
			
		}

		SED3D11Driver::~SED3D11Driver()
		{
			
		}

		void  SED3D11Driver::beginScene()
		{
			
		}
		void  SED3D11Driver::endScene()
		{
			
		}

		void drawPixel()
		{

		}

		void SED3D11Driver::draw2DLine()
		{

		}
		void SED3D11Driver::draw3DLine()
		{

		}

		void SED3D11Driver::draw2DImage()
		{

		}
		void SED3D11Driver::draw2DRectangle()
		{

		}

		void SED3D11Driver::InitD3D11Driver()
		{
			UINT createDeviceFlages = 0;
#if defined(DEBUG) || defined(_DEBUG)
			createDeviceFlages |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			D3D_FEATURE_LEVEL featureLevel;

			if (FAILED(D3D11CreateDevice(
				0,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				createDeviceFlages,
				0,
				0,
				D3D11_SDK_VERSION,
				&m_d3dDevice,
				&featureLevel,
				&m_d3dImmediateContext)))
			{
				MessageBox(0, L"D3D11CreateDevice Failed", 0, 0);
				return;
			}

			if (FAILED(m_d3dDevice->CheckMultisampleQualityLevels(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				4,
				&m_4xMsaaQuality)))
			{
				MessageBox(0, L"Check MultisampleQuality Failed", 0, 0);
				return;
			}

			assert(m_4xMsaaQuality > 0);

			DXGI_SWAP_CHAIN_DESC swap_chain_desc;
			swap_chain_desc.BufferDesc.Width  = m_clientWidth;
			swap_chain_desc.BufferDesc.Height = m_clientHeight;
			swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			if (m_bEnable4xMsaa)
			{
				swap_chain_desc.SampleDesc.Count = 4;
				swap_chain_desc.SampleDesc.Quality = m_4xMsaaQuality - 1;
			}
			else
			{
				swap_chain_desc.SampleDesc.Count = 0;
				swap_chain_desc.SampleDesc.Quality = 0;
			}
			swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_desc.BufferCount = 1;
			swap_chain_desc.OutputWindow = m_hMainWnd;
			swap_chain_desc.Windowed    = true;
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swap_chain_desc.Flags = 0;

			IDXGIDevice* dxgiDevice;
			if (FAILED(m_d3dDevice->QueryInterface(
				__uuidof(IDXGIDevice), 
				(void**)&dxgiDevice)))
			{
				MessageBox(0, L"Query IDXGIDevice failed", 0, 0);
				return;
			}

			IDXGIAdapter* dxgiAdapter;
			if (FAILED(dxgiDevice->GetParent(
				__uuidof(IDXGIAdapter), 
				(void**)&dxgiAdapter)))
			{
				MessageBox(0, L"Get IDXGIAdapter failed", 0, 0);
				return;
			}

			IDXGIFactory* dxgiFactory;
			if (FAILED(dxgiAdapter->GetParent(
				__uuidof(IDXGIFactory), 
				(void**)&dxgiFactory)))
			{
				MessageBox(0, L"Get IDXGIFactory failed", 0, 0);
				return;
			}

			if (FAILED(dxgiFactory->CreateSwapChain(
				m_d3dDevice, 
				&swap_chain_desc,
				&m_SwapChain)))
			{
				MessageBox(0, L"Create Swap Chain failed", 0, 0);
				return;
			}

			ReleaseCOM(dxgiDevice);
			ReleaseCOM(dxgiAdapter);
			ReleaseCOM(dxgiFactory);

			onResize();
		}


	}
}