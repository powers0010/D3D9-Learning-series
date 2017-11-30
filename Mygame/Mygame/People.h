#pragma once

#include "D3DUnit.h"
#include "stdafx.h"
#include "AllocateHierarchyClass.h"

struct HPDVERTEX
{
	FLOAT _x, _y, _z;
	FLOAT _rhw;
	FLOAT _u, _v ;
	HPDVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT rhw,FLOAT u, FLOAT v)
		: _x(x), _y(y), _z(z),_rhw(rhw), _u(u), _v(v) {}
};
#define D3DFVF_HPVERTEX  (D3DFVF_XYZRHW | D3DFVF_TEX1)

//人物分类
#define PEOPLE_HERO 0

enum People_State
{
	People_State_daiji	=	0,	//待机 正常
	People_State_zhandou=	1,	//战斗
	People_State_died	=	2	//死亡
};
//人物动作
enum People_Action
{
	People_Action_stand		=	0,
	People_Action_stand1	=	1,
 	People_Action_walk		=	10,
	People_Action_walkLeft	=	11,
	People_Action_walkRight	=	12,
 	People_Action_run		=	20,

 	People_Action_back		=	30,
	People_Action_back2		=	31,
 	People_Action_attack1	=	40,
 	People_Action_attack2	=	41,
 	People_Action_beattack	=	50,
 	People_Action_shanbi	=	55,	//闪避
 	People_Action_dazuo1	=	60,	//打坐
 	People_Action_dazuo2	=	61,
 	People_Action_dazuo3	=	62,
 	People_Action_daoxia	=	65,//被击倒
 	People_Action_daoxia2	=	70,	//爬起
// 
 	People_Action_tiaoyue1	=	75,	//起跳
 	People_Action_tiaoyue2	=	76,//空中
 	People_Action_tiaoyue3	=	77 ,//落地
// 
 	People_Action_died		=	80	//死亡
};


class People
{
public:
	People(LPDIRECT3DDEVICE9 pd3dDevice,int state,wchar_t* name):m_State(state),m_name(name),m_Level(0),
		m_HP(100),MAX_HP(100),m_MP(100),MAX_MP(100),m_speed(0.1f),m_Action(People_Action_stand),m_direction(D3DXVECTOR3(0.0f,0.0f,1.0f)),
		m_angle(0),m_Volangle(0)
	{
		m_pd3dDevice=pd3dDevice;
	}
	~People(void);

	//【设置人物坐标】
	void SetPosition(D3DXVECTOR3 *position)	
	{
		if (position!=NULL)
		{
			m_position=*position;
		}
		else 
			m_position=D3DXVECTOR3(0.0F,0.0F,0.0F);
	}		
	//【设置人物方向】
	void SetDirection(D3DXVECTOR3 *direction)		//设置人物方向
	{
		if (direction!=NULL)
		{
			m_direction=*direction;
		} 
		else
		{
			m_direction=D3DXVECTOR3(0.0f,0.0f,1.0f);
		}
		
	}
	//【获取人物坐标】
	D3DXVECTOR3 GetPosition(){ return m_position;}						//获取人物位置
	//【获取人物方向】
	D3DXVECTOR3 GetDirection()											//获取人物方向
	{
		D3DXVec3Normalize(&m_direction,&m_direction);		//方向向量规格化
		return m_direction;
	}			
	//【获取人物角度】
	float GetAngel(){return m_Volangle;}						//获取人物角度
	//【获取人物速度】
	float GetSpeed(){return m_speed;}							//获取人物速度

	//【人物移动  前进 后退】
	void MoveLook(float ftime);		
	void Rotation(float fang);
	void MoveLR(float ftime);

	//【设置 获取 人物动作状态】
	void setActionState (People_Action act){ m_Action = act; }
	int GetActionState (){return m_Action;}
	//【计算人物世界矩阵】
	void CaculateMatrix(D3DXMATRIX * mat);		

	//【加载人物血蓝条纹理，设置血蓝条位置】
	void InitHPMP(const D3DXVECTOR3* vec1 , wchar_t * TextureFile , wchar_t * HPFile );

	//【加载人物血蓝条纹理，设置血蓝条位置】
	void HPNPrender();
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	int				m_State;	//人物分类
	wchar_t	*		m_name;		//名称
	int				m_Level;	//等级
	int				m_HP;		//HP
	int				MAX_HP;		//HP上限

	int				m_MP;
	int				MAX_MP;		//MP上限

	//【血蓝条位置坐标】
	D3DXVECTOR3					m_HPVec;
	LPDIRECT3DVERTEXBUFFER9		m_pHPVBuffer;			// 血蓝条背景顶点缓存
	LPDIRECT3DVERTEXBUFFER9		m_phpVertexBuf;			//血条顶点缓存
	LPDIRECT3DTEXTURE9			m_pHPTexture;		//血蓝条背景纹理
	LPDIRECT3DTEXTURE9			m_phpTextureBuf;		//血条纹理

	float			m_speed;	//移动速度

	D3DXVECTOR3		m_position;		//人物位置
	D3DXVECTOR3		m_direction;	//人物朝向
	float			m_angle;		//每次旋转角
	float			m_Volangle;		//人物朝向角

	//四个和骨骼动画相关的全局变量


	People_Action	m_Action;		//人物动作状态

};

