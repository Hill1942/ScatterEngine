#ifndef SE_SCATTER_DEVICE_WIN32_H
#define SE_SCATTER_DEVICE_WIN32_H

#include "SEDeviceCreationParameter.h"

namespace se
{
	class SEDeviceWin32: public SEIDevice
	{
	public:
		SEDeviceWin32(const SEDeviceCreationParameter& param);
		virtual ~SEDeviceWin32(void);

		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		bool InitWindow();
		bool InitDriver();

	protected:
		HWND         m_hMainWnd;
		HINSTANCE    m_hAppInstance;
		bool         m_AppPaused;
		bool         m_Minimized;
		bool         m_Maximized;
		bool         m_Resizing;
		std::wstring m_strMainWndCaption;
	};
}

#endif
