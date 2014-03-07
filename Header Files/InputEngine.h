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



 

#endif