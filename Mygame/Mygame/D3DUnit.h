//*****************************************************************************************
// 用于公共辅助宏的定义

//*****************************************************************************************

#pragma once
#include "stdafx.h"
#include "resource.h"


#ifndef D3D9_H
#define D3D9_H
#include<d3d9.h.>
#include <d3dx9.h>
#endif

//参数宏定义
#define WINDOW_WIDTH	1000						//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	720							//为窗口高度定义的宏，以方便在此处修改窗口高度

#define MAX_LOADSTRING 100

#define MAX_XMODELNUM  20				//场景资源数量
#define MAX_NPCNUM		20				//最大NPC数量


//释放宏
#ifndef HR
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }         //自定义一个HR宏，方便执行错误的返回
#endif

#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }       //自定义一个SAFE_RELEASE()宏,便于指针资源的释放
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //自定义一个SAFE_RELEASE()宏,便于COM资源的释放
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif   
