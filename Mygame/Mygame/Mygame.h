#pragma once

#include "resource.h"

//-----------------------------------����Ϸ״̬�꡿---------------------------------------
//	����������Ϸ����״̬
//------------------------------------------------------------------------------------------------
#define GAME_RUNNING_STATE_GUI 1		//��Ϸ��ʼGUI����
#define GAME_RUNNING_STATE_INIT 2		//��Ϸ���ؽ���
#define GAME_RUNNING_STATE_UNDERWAY 3	//��Ϸ������
#define GAME_RUNNING_STATE_GAMEOVER 4	//��Ϸ����

//��Ϸ״̬����
static int	g_GameRunningState=GAME_RUNNING_STATE_GUI;	//��Ϸ״̬
