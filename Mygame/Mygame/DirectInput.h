#pragma once

#include<dinput.h>
#include "D3DUnit.h"


//���ļ�

#pragma comment(lib, "Dinput8.lib")



class DInput
{
public:
	DInput(void);
	~DInput(void);
public:
	HRESULT Init( HWND hwnd, HINSTANCE hinst,DWORD MousedwFlags,DWORD KeybordFlags);//��ʼ��
	void GetInput();//��ȡ���룬��������
	bool IsKeyDown(int Ikey);//�����Ƿ��а���Ikey
	bool IsMouseDown(int Ibutton);//����Ƿ��а���Ibutton
	float MouseDx();//�������X�ƶ���
	float MouseDy();//�������Y�ƶ���
	float MouseDz();//�������Y�ƶ���

private:
	LPDIRECTINPUT8 D_PDirectInput8;	
	LPDIRECTINPUTDEVICE8 D_PMouseDevice;
	DIMOUSESTATE D_MouseState;
	LPDIRECTINPUTDEVICE8 D_PKeyboardDevice;
	char D_KeyState[256];
};

