#pragma once

#include "resource.h"

//-----------------------------------【游戏状态宏】---------------------------------------
//	描述：运游戏运行状态
//------------------------------------------------------------------------------------------------
#define GAME_RUNNING_STATE_GUI 1		//游戏开始GUI界面
#define GAME_RUNNING_STATE_INIT 2		//游戏加载界面
#define GAME_RUNNING_STATE_UNDERWAY 3	//游戏进行中
#define GAME_RUNNING_STATE_GAMEOVER 4	//游戏结束

class MyGame
{
public:
	HRESULT				Direct3D_Init(HWND hwnd,HINSTANCE hInstance);	//d3d初始化程序
	void				Direct3D_Render( HWND hwnd,float fLastTime);	//d3d渲染函数
	HRESULT				Objects_Init();									//d3d资源初始化


	//资源初始化函数
	bool				GUI_Init();										//GUI资源初始化
protected:

private:

};