//*****************************************************************************************
// ���ڹ���������Ķ���

//*****************************************************************************************

#pragma once
#include "stdafx.h"
#include "resource.h"


#ifndef D3D9_H
#define D3D9_H
#include<d3d9.h.>
#include <d3dx9.h>
#endif

//�����궨��
#define WINDOW_WIDTH	1000						//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	720							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�

#define MAX_LOADSTRING 100

#define MAX_XMODELNUM  20				//������Դ����
#define MAX_NPCNUM		20				//���NPC����


//�ͷź�
#ifndef HR
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }         //�Զ���һ��HR�꣬����ִ�д���ķ���
#endif

#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }       //�Զ���һ��SAFE_RELEASE()��,����ָ����Դ���ͷ�
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //�Զ���һ��SAFE_RELEASE()��,����COM��Դ���ͷ�
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif   
