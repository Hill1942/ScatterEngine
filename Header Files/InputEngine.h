#ifndef INPUT_ENGINE
#define INPUT_ENGINE

int  DInput_Init();
void DInput_Shutdown();
int  DInput_Init_Joystick(int minX = -256, int maxX = 256, int minY = - 256, int maxY = 256, int deadBand = 10);
int  DInput_Init_Mouse();
int  DInput_Init_Keyboard();
int  DInput_Read_Joystick();
int  DInput_Read_Mouse();
int  DInput_Read_Keyboard();
void DInput_Release_Joystick();
void DInput_Release_Mouse();
void DInput_Release_Keyboard();


extern HWND                 mainWindowHandle;
extern HINSTANCE            mainInstance;

extern LPDIRECTINPUT8       lpDInput;
extern LPDIRECTINPUTDEVICE8 lpDInputKeyboard;
extern LPDIRECTINPUTDEVICE8 lpDInputMouse;
extern LPDIRECTINPUTDEVICE8 lpDInputJoystick;
extern GUID                 joystickGUID;
extern char                 joystickName[80];

extern UCHAR                keyboardState[256];
extern DIMOUSESTATE         mouseState;
extern DIJOYSTATE           joystickState;
extern int                  joystickFound;
 

#endif