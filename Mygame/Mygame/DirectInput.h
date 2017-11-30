#pragma once

#include<dinput.h>
#include "D3DUnit.h"


//库文件

#pragma comment(lib, "Dinput8.lib")



class DInput
{
public:
	DInput(void);
	~DInput(void);
public:
	HRESULT Init( HWND hwnd, HINSTANCE hinst,DWORD MousedwFlags,DWORD KeybordFlags);//初始化
	void GetInput();//获取输入，断线重连
	bool IsKeyDown(int Ikey);//键盘是否有按键Ikey
	bool IsMouseDown(int Ibutton);//鼠标是否有按键Ibutton
	float MouseDx();//返回鼠标X移动量
	float MouseDy();//返回鼠标Y移动量
	float MouseDz();//返回鼠标Y移动量

private:
	LPDIRECTINPUT8 D_PDirectInput8;	
	LPDIRECTINPUTDEVICE8 D_PMouseDevice;
	DIMOUSESTATE D_MouseState;
	LPDIRECTINPUTDEVICE8 D_PKeyboardDevice;
	char D_KeyState[256];
};

