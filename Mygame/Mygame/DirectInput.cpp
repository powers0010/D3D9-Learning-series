#include "StdAfx.h"
#include "DirectInput.h"


DInput::DInput(void)
{
	D_PDirectInput8=NULL;	
	D_PMouseDevice=NULL;
	ZeroMemory(&D_MouseState,sizeof(D_MouseState));
	D_PKeyboardDevice=NULL;
	ZeroMemory(D_KeyState,sizeof(char)*256);
}

DInput::~DInput(void)
{
	if (D_PMouseDevice!=NULL)
	{
		D_PMouseDevice->Unacquire();
	}
	if (D_PKeyboardDevice!=NULL)
	{
		D_PKeyboardDevice->Unacquire();
	}
	D_PDirectInput8->Release();
	D_PMouseDevice->Release();
	D_PKeyboardDevice->Release();
}

HRESULT DInput::Init( HWND hwnd, HINSTANCE hinst,DWORD MousedwFlags,DWORD KeybordFlags)
{
	HRESULT hr;
	HR(DirectInput8Create(hinst,0x0800,IID_IDirectInput8,(void**)&D_PDirectInput8,NULL));	//创建DirectInput8对象
	//创建鼠标设备对象
	HR(D_PDirectInput8->CreateDevice(GUID_SysMouse,&D_PMouseDevice,NULL));					
	HR(D_PMouseDevice->SetDataFormat(&c_dfDIMouse));
	HR(D_PMouseDevice->SetCooperativeLevel(hwnd,MousedwFlags));
	HR(D_PMouseDevice->Acquire());
	HR(D_PMouseDevice->Poll());
	//创建键盘设备对象
	HR(D_PDirectInput8->CreateDevice(GUID_SysKeyboard,&D_PKeyboardDevice,NULL));			
	HR(D_PKeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(D_PKeyboardDevice->SetCooperativeLevel(hwnd,KeybordFlags));
	HR(D_PKeyboardDevice->Acquire());
	HR(D_PKeyboardDevice->Poll());

	return S_OK;
}
void DInput::GetInput()
{
	HRESULT hr;
	//获取鼠标信息
	hr=D_PMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),(void**)&D_MouseState);
 	if (FAILED(hr))
 	{
 		D_PMouseDevice->Acquire();
 		D_PMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),(void**)&D_MouseState);
 	}

	//获取键盘信息
 	hr=D_PKeyboardDevice->GetDeviceState(sizeof(char)*256,(void**)&D_KeyState);
  	if (FAILED(hr))
  	{
  		D_PKeyboardDevice->Acquire();
		D_PMouseDevice->GetDeviceState(sizeof(char)*256,(void**)&D_KeyState);
  	}
}
bool DInput::IsKeyDown(int Ikey)
{
	if (D_KeyState[Ikey]&0x80)
		return true;
	else
		return false;
}
bool DInput::IsMouseDown(int Ibutton)
{
	if (D_MouseState.rgbButtons[Ibutton]&0x80)
		return true;
	else
		return false;
}
float DInput::MouseDx()
{
	return (float)D_MouseState.lX;
}
float DInput::MouseDy()
{
	return (float)D_MouseState.lY;
}
float DInput::MouseDz()
{
	return (float)D_MouseState.lZ;
}