#pragma once

#include "resource.h"

//-----------------------------------����Ϸ״̬�꡿---------------------------------------
//	����������Ϸ����״̬
//------------------------------------------------------------------------------------------------
#define GAME_RUNNING_STATE_GUI 1		//��Ϸ��ʼGUI����
#define GAME_RUNNING_STATE_INIT 2		//��Ϸ���ؽ���
#define GAME_RUNNING_STATE_UNDERWAY 3	//��Ϸ������
#define GAME_RUNNING_STATE_GAMEOVER 4	//��Ϸ����

class MyGame
{
public:
	HRESULT				Direct3D_Init(HWND hwnd,HINSTANCE hInstance);	//d3d��ʼ������
	void				Direct3D_Render( HWND hwnd,float fLastTime);	//d3d��Ⱦ����
	HRESULT				Objects_Init();									//d3d��Դ��ʼ��


	//��Դ��ʼ������
	bool				GUI_Init();										//GUI��Դ��ʼ��
protected:

private:

};