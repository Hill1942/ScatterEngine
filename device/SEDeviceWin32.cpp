#include <string>
#include <windows.h>

#include "../render/SED3D11Driver.h"

#include "SEIDevice.h"
#include "SEDeviceWin32.h"


namespace se
{
	SEDeviceWin32* g_pSEDeviceWin32 = 0;

	LRESULT CALLBACK MainWinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return g_pSEDeviceWin32->MsgProc(hwnd, msg, wParam, lParam);
	}

	SEDeviceWin32::SEDeviceWin32(const SEDeviceCreationParameter& param)
		:SEDevice(param)
	{
		g_pSEDeviceWin32 = this;

		if (!InitWindow())
		{
			MessageBox(0, L"Cannot init window!", L"Error", MB_OK);
			return;
		}

		if (!InitDriver())
		{
			MessageBox(0, L"Cannot init render driver!", L"Error", MB_OK);
			return;
		}


		
	}

	SEDeviceWin32::~SEDeviceWin32()
	{
		
	}

	bool SEDeviceWin32::InitWindow()
	{
		WNDCLASS wd;
		wd.style         = CS_HREDRAW | CS_VREDRAW;
		wd.lpfnWndProc   = MainWinProc;
		wd.cbClsExtra    = 0;
		wd.cbWndExtra    = 0;
		wd.hCursor       = LoadCursor(0, IDC_ARROW);
		wd.hIcon         = LoadIcon(0, IDC_ICON);
		wd.hInstance     = m_hAppInstance;
		wd.hbrBackground = NULL;
		wd.lpszMenuName  = 0;
		wd.lpszClassName = L"SEMainWindow";

		if( !RegisterClass(&wd) )
		{
			MessageBox(0, L"RegisterClass Failed.", 0, 0);
			return false;
		}

		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, this->m_clientWidth, this->m_clientHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width  = R.right - R.left;
		int height = R.bottom - R.top;

		m_hMainWnd = CreateWindow(
			L"SEMainWindow", 
			m_strMainWndCaption.c_str(), 
			WS_OVERLAPPEDWINDOW, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			width,
			height, 
			0,
			0,
			m_hAppInstance,
			0); 
		if( !m_hMainWnd )
		{
			MessageBox(0, L"CreateWindow Failed.", 0, 0);
			return false;
		}

		ShowWindow(m_hMainWnd, SW_SHOW);
		UpdateWindow(m_hMainWnd);

		return true;
	}

	bool SEDeviceWin32::InitDriver()
	{
		switch (m_driverType)
		{
		case render::SE_DT_DIRECTX11:
			{
				m_renderDirver = new render::SED3D11Driver();

				if (!m_renderDirver)
				{
					
				}

				break;
			}
		case render::SE_DT_DIRECTX12:
			{
				break;
			}
		case render::SE_DT_DIRECTX9:
			{
				break;
			}
		case render::SE_DT_OPENGL:
			{
				break;
			}
		case render::SE_DT_OPENGL_LEGACY:
			{
				break;
			}
		case render::SE_DT_SOFTWARE:
			{
				break;
			}
		case render::SE_DT_NULL:
			{
				break;
			}
		default:
			break;
		}
	}



}
