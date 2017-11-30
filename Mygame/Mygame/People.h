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

//�������
#define PEOPLE_HERO 0

enum People_State
{
	People_State_daiji	=	0,	//���� ����
	People_State_zhandou=	1,	//ս��
	People_State_died	=	2	//����
};
//���ﶯ��
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
 	People_Action_shanbi	=	55,	//����
 	People_Action_dazuo1	=	60,	//����
 	People_Action_dazuo2	=	61,
 	People_Action_dazuo3	=	62,
 	People_Action_daoxia	=	65,//������
 	People_Action_daoxia2	=	70,	//����
// 
 	People_Action_tiaoyue1	=	75,	//����
 	People_Action_tiaoyue2	=	76,//����
 	People_Action_tiaoyue3	=	77 ,//���
// 
 	People_Action_died		=	80	//����
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

	//�������������꡿
	void SetPosition(D3DXVECTOR3 *position)	
	{
		if (position!=NULL)
		{
			m_position=*position;
		}
		else 
			m_position=D3DXVECTOR3(0.0F,0.0F,0.0F);
	}		
	//���������﷽��
	void SetDirection(D3DXVECTOR3 *direction)		//�������﷽��
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
	//����ȡ�������꡿
	D3DXVECTOR3 GetPosition(){ return m_position;}						//��ȡ����λ��
	//����ȡ���﷽��
	D3DXVECTOR3 GetDirection()											//��ȡ���﷽��
	{
		D3DXVec3Normalize(&m_direction,&m_direction);		//�����������
		return m_direction;
	}			
	//����ȡ����Ƕȡ�
	float GetAngel(){return m_Volangle;}						//��ȡ����Ƕ�
	//����ȡ�����ٶȡ�
	float GetSpeed(){return m_speed;}							//��ȡ�����ٶ�

	//�������ƶ�  ǰ�� ���ˡ�
	void MoveLook(float ftime);		
	void Rotation(float fang);
	void MoveLR(float ftime);

	//������ ��ȡ ���ﶯ��״̬��
	void setActionState (People_Action act){ m_Action = act; }
	int GetActionState (){return m_Action;}
	//�����������������
	void CaculateMatrix(D3DXMATRIX * mat);		

	//����������Ѫ������������Ѫ����λ�á�
	void InitHPMP(const D3DXVECTOR3* vec1 , wchar_t * TextureFile , wchar_t * HPFile );

	//����������Ѫ������������Ѫ����λ�á�
	void HPNPrender();
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	int				m_State;	//�������
	wchar_t	*		m_name;		//����
	int				m_Level;	//�ȼ�
	int				m_HP;		//HP
	int				MAX_HP;		//HP����

	int				m_MP;
	int				MAX_MP;		//MP����

	//��Ѫ����λ�����꡿
	D3DXVECTOR3					m_HPVec;
	LPDIRECT3DVERTEXBUFFER9		m_pHPVBuffer;			// Ѫ�����������㻺��
	LPDIRECT3DVERTEXBUFFER9		m_phpVertexBuf;			//Ѫ�����㻺��
	LPDIRECT3DTEXTURE9			m_pHPTexture;		//Ѫ������������
	LPDIRECT3DTEXTURE9			m_phpTextureBuf;		//Ѫ������

	float			m_speed;	//�ƶ��ٶ�

	D3DXVECTOR3		m_position;		//����λ��
	D3DXVECTOR3		m_direction;	//���ﳯ��
	float			m_angle;		//ÿ����ת��
	float			m_Volangle;		//���ﳯ���

	//�ĸ��͹���������ص�ȫ�ֱ���


	People_Action	m_Action;		//���ﶯ��״̬

};

