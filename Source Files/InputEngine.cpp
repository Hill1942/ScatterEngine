#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstring>
#include <dinput.h>

#include "InputEngine.h"

#pragma comment(lib, "dinput8.lib")

extern HWND          mainWindowHandle;
extern HINSTANCE     windowInstance;


LPDIRECTINPUT8       lpDInput = NULL;
LPDIRECTINPUTDEVICE8 lpDInputKeyboard = NULL;
LPDIRECTINPUTDEVICE8 lpDInputMouse = NULL;
LPDIRECTINPUTDEVICE8 lpDInputJoystick = NULL;

GUID                 joystickGUID;
char                 joystickName[80];

UCHAR                keyboardState[256];
DIMOUSESTATE         mouseState;
DIJOYSTATE           joystickState;
int                  joystickFound = 0;




BOOL CALLBACK DInput_Enum_Joysticks(LPCDIDEVICEINSTANCE lpddi, LPVOID pGUID)
{
	*(GUID*)pGUID = lpddi->guidInstance;
	strcpy(joystickName, (char*)lpddi->tszProductName);
	return DIENUM_STOP;
}

int DInput_Init()
{
	if (FAILED(DirectInput8Create(windowInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDInput, NULL)))
		return 0;

	return 1;
}

void DInput_Shutdown()
{
	if (lpDInput)
		lpDInput->Release();
}

int DInput_Init_Joystick(int minX, int maxX, int minY, int maxY, int deadZone)
{
	lpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DInput_Enum_Joysticks, &joystickGUID, DIEDFL_ATTACHEDONLY);
	LPDIRECTINPUTDEVICE lpdiJoyTemp = NULL;

	if (lpDInput->CreateDevice(joystickGUID, &lpDInputJoystick, NULL) != DI_OK)
		return 0;
	
	if (lpDInputJoystick->SetCooperativeLevel(mainWindowHandle, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND) != DI_OK)
		return 0;

	if (lpDInputJoystick->SetDataFormat(&c_dfDIJoystick) != DI_OK)
		return 0;

	DIPROPRANGE joystickAxisRange;
	joystickAxisRange.lMin              = minX;
	joystickAxisRange.lMax              = maxX;
	joystickAxisRange.diph.dwSize       = sizeof(DIPROPRANGE);
	joystickAxisRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	joystickAxisRange.diph.dwObj        = DIJOFS_X;
	joystickAxisRange.diph.dwHow        = DIPH_BYOFFSET;
	lpDInputJoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);

	joystickAxisRange.lMin              = minY;
	joystickAxisRange.lMax              = maxY;
	joystickAxisRange.diph.dwSize       = sizeof(DIPROPRANGE);
	joystickAxisRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	joystickAxisRange.diph.dwObj        = DIJOFS_Y;
	joystickAxisRange.diph.dwHow        = DIPH_BYOFFSET;
	lpDInputJoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);


	DIPROPDWORD deadBand;
	deadZone                  *= 100;
	deadBand.diph.dwSize       = sizeof(DIPROPDWORD);
	deadBand.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	deadBand.diph.dwObj        = DIJOFS_X;
	deadBand.diph.dwHow        = DIPH_BYOFFSET;
	deadBand.dwData            = deadZone;
	lpDInputJoystick->SetProperty(DIPROP_DEADZONE, &deadBand.diph);

	deadBand.diph.dwSize       = sizeof(DIPROPDWORD);
	deadBand.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	deadBand.diph.dwObj        = DIJOFS_Y;
	deadBand.diph.dwHow        = DIPH_BYOFFSET;
	deadBand.dwData            = deadZone;
	lpDInputJoystick->SetProperty(DIPROP_DEADZONE, &deadBand.diph);


	if (lpDInputJoystick->Acquire() != DI_OK)
		return 0;

	joystickFound = 1;
	return 1;
}

int DInput_Init_Mouse()
{
	if (FAILED(lpDInput->CreateDevice(GUID_SysMouse, &lpDInputMouse, NULL)))
		return 0;

	if (FAILED(lpDInputMouse->SetCooperativeLevel(mainWindowHandle, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND)))
		return 0;

	if (FAILED(lpDInputMouse->SetDataFormat(&c_dfDIMouse)))
		return 0;

	if (FAILED(lpDInputMouse->Acquire()))
		return 0;

	return 1;
}

int DInput_Init_Keyboard()
{
	if (FAILED(lpDInput->CreateDevice(GUID_SysKeyboard, &lpDInputKeyboard, NULL)))
		return 0;

	if (FAILED(lpDInputKeyboard->SetCooperativeLevel(mainWindowHandle, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND)))
		return 0;

	if (FAILED(lpDInputKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return 0;

	if (FAILED(lpDInputKeyboard->Acquire()))
		return 0;

	return 1;
}

int DInput_Read_Joystick()
{
	if (!joystickFound)
		return 0;

	if (lpDInputJoystick)
	{
		if (FAILED(lpDInputJoystick->Poll()))
			return 0;
		if (FAILED(lpDInputJoystick->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joystickState)))
			return 0;
	}
	else
	{
		memset(&joystickState, 0, sizeof(DIJOYSTATE));
		return 0;
	}
	
	return 1;
}

int DInput_Read_Mouse()
{
	if (lpDInputMouse)
	{
		if (FAILED(lpDInputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState)))
			return 0;
	}
	else
	{
		memset(&mouseState, 0, sizeof(DIMOUSESTATE));
		return 0;
	}
	return 1;
}

int DInput_Read_Keyboard()
{
	if (lpDInputKeyboard)
	{
		if (FAILED(lpDInputKeyboard->GetDeviceState(256, (LPVOID)keyboardState)))
			return 0;
	}
	else
	{
		memset(keyboardState, 0, sizeof(keyboardState));
		return 0;
	}

	return 1;
}

void DInput_Release_Keyboard()
{
	if (lpDInputKeyboard)
	{
		lpDInputKeyboard->Unacquire();
		lpDInputKeyboard->Release();
	}
}

void DInput_Release_Mouse()
{
	if (lpDInputMouse)
	{
		lpDInputMouse->Unacquire();
		lpDInputMouse->Release();
	}
}

void DInput_Release_Joystick()
{
	if (lpDInputJoystick)
	{
		lpDInputJoystick->Unacquire();
		lpDInputJoystick->Release();
	}
}
