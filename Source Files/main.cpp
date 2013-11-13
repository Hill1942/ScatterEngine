#include <windows.h>


#define WINDOW_CLASS_NAME "WIN3DCLASS"
#define WINDOW_TITLE      "Scatter Engine"
#define WINDOW_WIDTH      640
#define WINDOW_HEIGHT     480

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0:1)


//Function Prototype
int Game_Init(void *param = NULL);
int Game_Shutdown(void* param = NULL);
int Game_Main(void* param = NULL);

//Global Variable
HWND main_window_handle = NULL;
HINSTANCE main_instance = NULL;
char buffer[256];

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_CREATE:
		return(0);
		break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return(0);
		break;

	case WM_DESTROY: 
		PostQuitMessage(0);
		return(0);
	    break;

	default:
		break;
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int nCmdShow)
{
	WNDCLASSEX wnd = {};
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpfnWndProc = WinProc;
	wnd.hInstance = hInstance;
	wnd.lpszClassName = WINDOW_CLASS_NAME;

	RegisterClassEx(&wnd);

	main_window_handle = CreateWindowEx(
		0, 
		WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		200, 100, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//SetWindowLong(, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	if (main_window_handle == NULL)
		return 0;
	main_instance = hInstance;

	ShowWindow(main_window_handle, nCmdShow);

	Game_Init();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Game_Main();
		}
	}

	Game_Shutdown();
	return 0;
}

int Game_Init(void *param)
{
	return 1;
}

int Game_Shutdown(void *param)
{
	return 1;
} 

int Game_Main(void *param)
{
	if (KEY_DOWN(VK_ESCAPE))
	{
		PostMessage(main_window_handle, WM_DESTROY, 0, 0);
    } 

	return 1;
} 