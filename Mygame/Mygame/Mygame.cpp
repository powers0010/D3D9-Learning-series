// Mygame.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "Mygame.h"
#include "D3DUnit.h"
#include "DirectInput.h"
#include "D3DGUIClass.h"
#include "Terrain.h"
#include "People.h"
#include "Camera.h"
#include "SkyBoxClass.h"
#include "XFileModelClass.h"

#include <tchar.h>
#include <time.h> 
#include <MMSystem.h>



//-----------------------------------【库文件包含部分】---------------------------------------
//	描述：包含程序所依赖的库文件
//------------------------------------------------------------------------------------------------
#pragma comment(lib, "winmm.lib") //timegettime使用


//-----------------------------------【全局变量声明部分】-------------------------------------
//	描述：全局变量的声明
//------------------------------------------------------------------------------------------------
//-----【 窗口全局变量】
HINSTANCE					hInst;								// 当前实例		
HWND						g_Hwnd;
TCHAR						szTitle[MAX_LOADSTRING];			// 标题栏文本
TCHAR						szWindowClass[MAX_LOADSTRING];		// 主窗口类名
D3DLIGHT9					g_Light;   //全局光照
//------【资源类 对象】
LPDIRECT3DDEVICE9			g_pd3dDevice = NULL;			//Direct3D设备对象
DInput*						g_pDirectInput=NULL;			//DirectInput对象
D3DGUIClass*				g_gui=NULL;						//GUI对象指针
CameraClass*				g_pCamera=NULL;					//相机类
TerrainClass*				g_pTerrain=NULL;				//地形类
SkyBoxClass*				g_pSkyBox=NULL;					//天空盒
LPD3DXFONT					g_pTextFPS =NULL;				//字体COM接口
wchar_t						g_strFPS[50] ={0};				//包含帧速率的字符数组

//-----【地板】
// 定义地板FVF顶点结构
struct CUSTOMVERTEX
{
	FLOAT _x, _y, _z;
	FLOAT _u, _v ;
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)

LPDIRECT3DVERTEXBUFFER9		g_pFloorVBuffer      = NULL;  //地板顶点缓存对象
LPDIRECT3DTEXTURE9			g_pFloorTexture   = NULL;  //地板纹理对象


//------【环境 场景资源模型】
XModelClass*				g_ppXViewModel[MAX_XMODELNUM];
int							XModelClass::XModelNum=0;		//初始化XModelNum
D3DXMATRIX					g_EnvirMatrix[MAX_XMODELNUM];	//场景世界坐标
//--------【输入信息】
bool						g_LMBDown = false;				// GUI中的鼠标状态信息，鼠标左键是否按下的标识
float						g_MouseX = 0, g_MouseY = 0;     //存储鼠标坐标的两个变量

//---------------【角色相关】
People*						g_player=NULL;					//玩家人物	
D3DXMATRIX					g_playerMat;					//玩家全局矩阵
float						g_ACTtime=0;					//动作计时							
People_State				g_PlayerState=People_State_daiji;		//玩家状态 0 正常 -1 战斗 2死亡
People_Action				g_PlayerAction=People_Action_stand;		//角色动作

//----------------【player骨骼动画参数】
CAllocateHierarchy*				g_ppAllocateHier[81]={NULL};
LPD3DXFRAME						 g_ppFrameRoot[81]={0};
D3DXMATRIX* 					g_ppBoneMatrices[81]={NULL};
LPD3DXANIMATIONCONTROLLER		g_ppAnimController[81]={0};

//----------------【NPC骨骼】
int								g_NumNPC=0;
CAllocateHierarchy*				g_ppNPCAllocateHier[MAX_NPCNUM];
LPD3DXFRAME						 g_ppNPCFrameRoot[MAX_NPCNUM];
D3DXMATRIX* 					g_ppNPCBoneMatrices[MAX_NPCNUM];
LPD3DXANIMATIONCONTROLLER		g_ppNPCAnimController[MAX_NPCNUM];
D3DXMATRIX						g_NPCMat[MAX_NPCNUM];					//NPC全局矩阵
//-----------------【环境建筑】


//-----------------------------------【游戏状态宏】---------------------------------------
//	描述：函数的前向声明:
//------------------------------------------------------------------------------------------------
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int );
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				GUICallback(int id, int state);					//GUI过程函数
void				Direct3D_Updata(float fTimeDelta);				//更新
HRESULT				Resource_Init();
HRESULT				Direct3D_Init(HWND hwnd,HINSTANCE hInstance);	//d3d初始化程序
void				Direct3D_Render( HWND hwnd,float fTimeDelta);	//d3d渲染函数
HRESULT				Objects_Init();									//d3d资源初始化
bool				GUI_Init();										//GUI资源初始化
void				LightInit();									//初始化灯光
void				OBJ_render(float fTimeDelta);		//绘制对象		
void				Player_Init();						//player角色动作初始化
void				Player_render(float fTimeDelta,People_Action i);					//人物渲染
void				SetCamera();						//根据人物位置设置摄像机
float				Get_FPS();							//计算帧数
void				Show_FPS(HWND hwnd);							//输出显示帧数
void				XViewModelInit();					//环境 场景资源初始化
void				XViewInitRender();				//环境场景渲染
void				NPCInit();						//NPC初始化
void				NPCRender(float fTimeDelta);	//NPC渲染
void				setcursor();					//限制鼠标移动
void				CheckInput();					//检查鼠标键盘输出设置状态
//-----------【WinMain】-------
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	
	if (!InitInstance (hInstance, nCmdShow ))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGAME));//调入加速键表

//	PlaySound(L"media\\BGM\\仙剑·王蓬絮.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP); //循环播放背景音乐 

	HWND hWnd;
	hWnd=GetTopWindow(NULL);
	// 主消息循环:
	MSG msg={0};
	while (msg.message != WM_QUIT)
	{
		static FLOAT fLastTime  =(float)timeGetTime();  
		static FLOAT fCurrTime  =(float)timeGetTime();  
		static FLOAT fTimeDelta = 0.0f;  
		fCurrTime  = (float)timeGetTime();  
		fTimeDelta= (fCurrTime - fLastTime) / 1000.0f;  
		fLastTime  = fCurrTime; 
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage( &msg );		//将虚拟键消息转换为字符消息
			DispatchMessage( &msg );		//该函数分发一个消息给窗口程序。
		}
		else
		{
			Direct3D_Render(g_Hwnd,fTimeDelta);
		}
	}
	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYGAME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
//    HWND hWnd;
//    hInst = hInstance; // 将实例句柄存储在全局变量中

   g_Hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!g_Hwnd)
   {
      return FALSE;
   }


   //--------------------------------------------------------------------------------------
   //初始化Direct3D
   //--------------------------------------------------------------------------------------

   if (!(S_OK==Direct3D_Init (g_Hwnd,hInstance)))
   {
	   MessageBox(g_Hwnd, _T("Direct3D初始化失败~！"), _T("浅墨的消息窗口"), 0); //使用MessageBox函数，创建一个消息窗口 
   }

   //--------------------------------------------------------------------------------------
   //初始化DirectInput  鼠标和键盘模式：前台，非独占模式
   //--------------------------------------------------------------------------------------
	
   //--------------------------------------------------------------------------------------
   //窗口的显示与更新
   //--------------------------------------------------------------------------------------
   MoveWindow(g_Hwnd,200,20,WINDOW_WIDTH,WINDOW_HEIGHT,true);
   ShowWindow(g_Hwnd, nCmdShow);
   UpdateWindow(g_Hwnd);

   //进行DirectInput类的初始化
   g_pDirectInput = new DInput();
   g_pDirectInput->Init(g_Hwnd,hInstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); //前台，非独占模式

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		g_MouseX=LOWORD(lParam);
		g_MouseY=HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (GAME_RUNNING_STATE_GUI==g_GameRunningState)
		{
			g_MouseX=LOWORD(lParam);
			g_MouseY=HIWORD(lParam);
			g_LMBDown=true;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//-----------------------------------【Direct3D_Init d3d初始化】-------------------------------------
//
//-----------------------------------【Direct3D_Init d3d初始化】-------------------------------------
HRESULT Direct3D_Init(HWND hwnd,HINSTANCE hInstance)
{
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //初始化Direct3D接口对象，并进行DirectX版本协商
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = WINDOW_WIDTH;
	d3dpp.BackBufferHeight           = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 2;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	//【Objects_Init】资源初始化
	//--------------------------------------------------------------------------------------
	if(!(S_OK==Objects_Init())) return E_FAIL;

	//--------------------------------------------------------------------------------------
	//【LPDIRECT3D9接口对象的使命完成，我们将其释放掉】
	//--------------------------------------------------------------------------------------
	SAFE_RELEASE(pD3D)
		return S_OK;

}


//-----------------------------------【Direct3D_Render d3d渲染】-------------------------------------
//void	Direct3D_Render( HWND hwnd,float fLastTime)
//-----------------------------------【Direct3D_Render d3d渲染】-------------------------------------
void	Direct3D_Render( HWND hwnd,float fTimeDelta)
{
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之一】：清屏操作
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(155, 155, 155), 1.0f, 0);


	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之二】：开始绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();  

	Show_FPS(hwnd);

	switch(g_GameRunningState)
	{
	case GAME_RUNNING_STATE_GUI:
		ProcessGUI(g_gui,g_LMBDown, g_MouseX, g_MouseY, GUICallback);
		g_LMBDown=NULL;
		break;
	case GAME_RUNNING_STATE_INIT:
		break;
	case GAME_RUNNING_STATE_UNDERWAY:

		Direct3D_Updata(fTimeDelta);	//更新动作
		
		OBJ_render(fTimeDelta);			//绘制对象
		
		break;
	}
	

	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之四】：结束绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // 结束绘制
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之五】：显示翻转
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // 翻转与显示

}

//-----------------------------------【 Objects_Init()】-------------------------------------
//资源初始化函数
//-----------------------------------【 Objects_Init()】-------------------------------------
HRESULT	 Objects_Init()
{
	if (false==GUI_Init())		//初始化开始游戏界面
	{
		return E_FAIL;
	}

	if (false==Resource_Init())
	{
		return E_FAIL;
	}
	
	return S_OK;
}


//-----------------------------------【Direct3D_Render d3d渲染】-------------------------------------
//void	Direct3D_Render( HWND hwnd,float fLastTime)
//-----------------------------------【Direct3D_Render d3d渲染】-------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_ID_1: 
		//“开始游戏”按钮的相关代码实现
		if(state == UGP_BUTTON_DOWN)
			g_GameRunningState=GAME_RUNNING_STATE_UNDERWAY;
		break;
	}
}



//-----------------------------------【Resource_Init】-------------------------------------
//资源初始化  ——地形 天空 
//-----------------------------------【Resource_Init】-------------------------------------
HRESULT	Resource_Init()
{
	void LightInit();		//初始化光照

	//创建字体
	D3DXCreateFont(g_pd3dDevice, 25, 0, 0, 1000, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);

// 	//初始化人物
	Player_Init();		//加载人物动作模型及其他资源
	D3DXMatrixIdentity(&g_playerMat);
	g_player = new People(g_pd3dDevice,PEOPLE_HERO,L"cloud");
	g_player ->SetPosition(NULL);		//设置开始坐标
	g_player ->SetDirection(NULL);		//设置开始朝向
	g_player ->CaculateMatrix(&g_playerMat);				//计算玩家全局位置
	//--【加载猪脚血条资源】
	g_player->InitHPMP(&D3DXVECTOR3(5.0f,5.0f,0.0f),L"media\\people\\player\\man\\血条\\猪脚血条.png" , L"media\\people\\player\\man\\血条\\血条.png");

	// 创建并初始化虚拟摄像机
	g_pCamera = new CameraClass(g_pd3dDevice);
	D3DXVECTOR3 vec = g_player->GetPosition();
	g_pCamera->SetCameraPosition(&D3DXVECTOR3(vec.x, vec.y+30.0f, vec.z-60.0f));
	g_pCamera->SetTargetPosition(&D3DXVECTOR3(vec.x, vec.y+30.0f, vec.z));
	g_pCamera->SetViewMatrix();  //设置取景变换矩阵
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 15000.0f);
	g_pCamera->SetProjMatrix(&matProj);

	// -----------------------------【创建地面顶点缓存】--------------------------------
	g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, 
		D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &g_pFloorVBuffer, NULL);
	CUSTOMVERTEX *pVertices = NULL;
	g_pFloorVBuffer->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-5000.0f, 0.0f, -5000.0f,  0.0f, 100.0f);
	pVertices[1] = CUSTOMVERTEX(-5000.0f, 0.0f,  5000.0f,  0.0f,  0.0f);
	pVertices[2] = CUSTOMVERTEX( 5000.0f, 0.0f, -5000.0f, 100.0f, 100.0f); 
	pVertices[3] = CUSTOMVERTEX( 5000.0f, 0.0f,  5000.0f, 100.0f,  0.0f);
	g_pFloorVBuffer->Unlock();
	// -----------------------------【创建地面纹理】---------------------------------------
	D3DXCreateTextureFromFile(g_pd3dDevice, L"media\\Terrain\\brick.bmp", &g_pFloorTexture);

	//初始化天空类
	g_pSkyBox = new SkyBoxClass(g_pd3dDevice);
	g_pSkyBox->LoadSkyTextureFromFile(
		L"media\\sky\\雨天\\0.JPG",
		L"media\\sky\\雨天\\2.JPG",
		L"media\\sky\\雨天\\3.JPG",
		L"media\\sky\\雨天\\1.JPG",
		L"media\\sky\\雨天\\4.JPG"
		);
	g_pSkyBox->InitSkyBox(10000);  //设置天空盒的边长

	//初始化场景模型
	XViewModelInit();

	//加载NPC模型
//	NPCInit();
	return true;
}
void Direct3D_Updata(float fTimeDelta)
{	
	//【检查键盘 鼠标输入信息】
	CheckInput();		
	
	

	//【NPC动作识别】

	
	//【限制鼠标移动出框】
	setcursor();
	
}
//【对象渲染函数】
void OBJ_render(float fTimeDelta)
{
	//【绘制玩家	player渲染】
	Player_render(fTimeDelta,g_PlayerAction);
	//【设置摄像机位置】
	SetCamera();

	//---【设置纹理状态】
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	
	//---【绘制地板】
	D3DXMATRIX matFloor;
	D3DXMatrixTranslation(&matFloor, 0.0f, 0.0f, 0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matFloor);
	g_pd3dDevice->SetStreamSource(0, g_pFloorVBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetTexture(0, g_pFloorTexture);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//-----------------------------【绘制天空】------------------------
	D3DXMATRIX matSky,matTransSky,matRotSky;
	D3DXMatrixTranslation(&matTransSky,0.0f,-2500.0f,0.0f);
	D3DXMatrixRotationY(&matRotSky, -0.00002f*timeGetTime());   //旋转天空网格, 简单模拟云彩运动效果
	matSky=matTransSky*matRotSky;
	g_pSkyBox->RenderSkyBox(&matSky, false);

	//【绘制场景对象】

	XViewInitRender();

	//【绘制npc】

	//----【绘制人物血条】
	g_player->HPNPrender();

}

//-----------------------------------【Player_render】-------------------------------------
//【人物渲染——不同动作】
//-----------------------------------【Player_render】-------------------------------------
void	Player_render(float fTimeDelta ,People_Action i)
{
	//记录动作的编号
	static int NUM_ACT=0;
	static float ACT_TIME=0.0f;
	// 	// 设置骨骼动画的矩阵
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_playerMat);
	if (g_PlayerState==People_State_zhandou)			//--【如果状态是攻击】
	{
		switch(i) 
		{
		case People_Action_attack1:
			PlaySound(L"media\\BGM\\剑出鞘1.wav", NULL, SND_FILENAME | SND_ASYNC |SND_NOSTOP); //循环播放背景音乐 
			NUM_ACT = 40;
			ACT_TIME = 1.0f;
			break;
		}

		g_ppAnimController[NUM_ACT]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
		UpdateFrameMatrices(g_ppFrameRoot[NUM_ACT], &g_playerMat);   //更新框架中的变换矩阵
		DrawFrame(g_pd3dDevice, g_ppFrameRoot[NUM_ACT]);
		g_ACTtime+=fTimeDelta;
		if (g_ACTtime>ACT_TIME)	
		{
			g_ACTtime=0;
			g_PlayerState=People_State_daiji;
			g_PlayerAction=People_Action_stand;
		}

	} 
	else if(g_PlayerState==People_State_daiji)		//---【如果状态正常】
	{
		switch(i)
		{
		case People_Action_stand://	--【待机】
			// WALK
			//-----------------------------【绘制骨骼动画】------------------------
			g_ppAnimController[0]->AdvanceTime(0.5*fTimeDelta, NULL);  //设置骨骼动画的时间
			UpdateFrameMatrices(g_ppFrameRoot[0], &g_playerMat);   //更新框架中的变换矩阵
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[0]);
			break;

		case People_Action_walk://	--【行走】
			//RUN
			// 	//-----------------------------【绘制骨骼动画】------------------------
			g_ppAnimController[10]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
			UpdateFrameMatrices(g_ppFrameRoot[10], &g_playerMat);   //更新框架中的变换矩阵
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[10]);
			break;

		case People_Action_walkLeft://	--【行走】
			// 	//-----------------------------【绘制骨骼动画】------------------------
			g_ppAnimController[11]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
			UpdateFrameMatrices(g_ppFrameRoot[11], &g_playerMat);   //更新框架中的变换矩阵
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[11]);
			break;

		case People_Action_walkRight://	--【行走】
			// 	//-----------------------------【绘制骨骼动画】------------------------
			g_ppAnimController[12]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
			UpdateFrameMatrices(g_ppFrameRoot[12], &g_playerMat);   //更新框架中的变换矩阵
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[12]);
			break;

		case People_Action_run:	//--【跑步】
			// 	//-----------------------------【绘制骨骼动画】------------------------
			g_ppAnimController[20]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
			UpdateFrameMatrices(g_ppFrameRoot[20], &g_playerMat);   //更新框架中的变换矩阵
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[20]);
			break;

		case People_Action_back:	//--【后退】
			//-----------------------------【绘制骨骼动画】------------------------
			g_ppAnimController[30]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
			UpdateFrameMatrices(g_ppFrameRoot[30], &g_playerMat);   //更新框架中的变换矩阵
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[30]);
			break;
		case People_Action_back2:	//--【后退】
			//-----------------------------【绘制骨骼动画】------------------------
			g_ppAnimController[31]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
			UpdateFrameMatrices(g_ppFrameRoot[31], &g_playerMat);   //更新框架中的变换矩阵
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[31]);
			break;
		}
	}
}

//-----------------------------------【SetCamera】-------------------------------------
//根据人物位置设置摄像机位置
//-----------------------------------【SetCamera】-------------------------------------
void				SetCamera()
{
	static float fltZ=80;
	fltZ+=0.05f*g_pDirectInput->MouseDz();	//滚轮缩放镜头
	if (fltZ<10.0f)
	{
		fltZ=10.0f;
	}
	//设置摄像机
	D3DXVECTOR3 pos1=g_player->GetPosition();
	D3DXVECTOR3 pos2=g_player->GetDirection();
	D3DXVECTOR3 set1 = D3DXVECTOR3(pos1.x,pos1.y+20,pos1.z)+fltZ*pos2;
	D3DXVECTOR3 set2 = D3DXVECTOR3(pos1.x,pos1.y+20,pos1.z);
	g_pCamera->SetCameraPosition(&set1);
	g_pCamera->SetTargetPosition(&set2);

	//计算取景变换矩阵，并设置
	D3DXMATRIX matView;  
	g_pCamera->CalculateViewMatrix(&matView);  
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}
//-----------------------------------【GUI_Init】-------------------------------------
//GUI界面初始化函数
//-----------------------------------【GUI_Init】-------------------------------------
bool GUI_Init()
{
	g_gui=new D3DGUIClass(g_pd3dDevice,WINDOW_WIDTH,WINDOW_HEIGHT);
	// 添加背景图片
	if(!g_gui->AddBackground(L"media\\GUI menu\\Assassins creed.jpg")) return false;

	// 添加4个按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if(!g_gui->AddButton(BUTTON_ID_1, 650, 340, L"media\\GUI menu\\startUp.png",
		L"media\\GUI menu\\StartOver.png", L"media\\GUI menu\\startDown.png")) return false;

	if(!g_gui->AddButton(BUTTON_ID_2, 650, 385, L"media\\GUI menu\\loadUp.png",
		L"media\\GUI menu\\loadOver.png", L"media\\GUI menu\\loadDown.png")) return false;

	if(!g_gui->AddButton(BUTTON_ID_3, 650, 430, L"media\\GUI menu\\optionsUp.png",
		L"media\\GUI menu\\optionsOver.png", L"media\\GUI menu\\optionsDown.png")) return false;

	if(!g_gui->AddButton(BUTTON_ID_4, 650, 475, L"media\\GUI menu\\quitUp.png",
		L"media\\GUI menu\\quitOver.png", L"media\\GUI menu\\quitDown.png")) return false;

	return true;
}

//-----------------------------------【NPCInit】-------------------------------------
//NPC初始化
//-----------------------------------【NPCInit】-------------------------------------
void				NPCInit()
{
	//NPC初始化
	//美女1
	g_ppNPCAllocateHier[0]=new CAllocateHierarchy();
	g_NumNPC++;
	D3DXLoadMeshHierarchyFromX(L"XingGanNv.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppNPCAllocateHier[0], NULL, &g_ppNPCFrameRoot[0], &g_ppNPCAnimController[0]);
	SetupBoneMatrixPointers(g_ppNPCFrameRoot[0], g_ppNPCFrameRoot[0]);
	D3DXMATRIX mat0,mat0s,mat0r,mat0t;
	D3DXMatrixIdentity(&mat0);
	D3DXMatrixScaling(&mat0s,3.0f,3.0f,3.0f);
	D3DXMatrixRotationY(&mat0r,0.0f);
	D3DXMatrixTranslation(&mat0t,-500.0f,0.0f,500.0f);
	g_NPCMat[0]= mat0 * mat0s * mat0r * mat0t;

}
//-------------【环境资源初始化】
void	XViewModelInit()
{
// 	//0---【城墙】
  	g_ppXViewModel[0]=new XModelClass(g_pd3dDevice);
  	g_ppXViewModel[0]->LoadModelFromXFile(L"media\\建筑\\城墙\\CQ_chengqiang.X");
  	D3DXMATRIX mat0,mat0s,mat0r,mat0t;
  	D3DXMatrixIdentity(&mat0);
  	D3DXMatrixScaling(&mat0s,0.4f,0.4f,0.4f);
  	D3DXMatrixRotationY(&mat0r,0.0f);
  	D3DXMatrixTranslation(&mat0t,-6400.0f,0.0f,6000.0f);
  	g_EnvirMatrix[0]= mat0 * mat0s * mat0r * mat0t;

	//1--【茶肆】
	g_ppXViewModel[1]=new XModelClass(g_pd3dDevice);
	g_ppXViewModel[1]->LoadModelFromXFile(L"media\\建筑\\茶肆01\\JZ_chasi.X");
	D3DXMATRIX mat1,mat1s,mat1r,mat1t;
	D3DXMatrixIdentity(&mat1);
	D3DXMatrixScaling(&mat1s,1.0f,1.0f,1.0f);
	D3DXMatrixRotationY(&mat1r,0.0f);
	D3DXMatrixTranslation(&mat1t,0.0f,0.0f,100.0f);
	g_EnvirMatrix[1]= mat0 * mat0s * mat0r * mat0t;

}
//-------------【环境场景渲染】
void XViewInitRender()
{
 	for (int i=0;i<XModelClass::XModelNum;i++)
 	{
		g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_EnvirMatrix[0]);
		g_ppXViewModel[0]->RenderModel();
	}	

}

//-----------------------------------【NPCRender】-------------------------------------
//NPC渲染
//-----------------------------------【NPCRender】-------------------------------------
void				NPCRender(float fTimeDelta)
{
	for (int i=0;i<g_NumNPC;i++)
	{
		g_ppNPCAnimController[i]->AdvanceTime(fTimeDelta, NULL);  //设置骨骼动画的时间
		UpdateFrameMatrices(g_ppNPCFrameRoot[i], &g_NPCMat[i]);   //更新框架中的变换矩阵
		DrawFrame(g_pd3dDevice, g_ppNPCFrameRoot[i]);
	}
}
//-----------------------------------【LightInit】-------------------------------------
//初始化灯光
//-----------------------------------【LightInit】-------------------------------------
void LightInit()
{
	// 设置光照  
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB(255, 255, 255));
	::ZeroMemory(&g_Light, sizeof(g_Light));  
	g_Light.Type          = D3DLIGHT_DIRECTIONAL;  
	g_Light.Ambient       = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);  
	g_Light.Diffuse       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  
	g_Light.Specular      = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);  
	g_Light.Direction     = D3DXVECTOR3(0.0f, 0.0f, 1.0f);  
	g_pd3dDevice->SetLight(0, &g_Light);  
	g_pd3dDevice->LightEnable(0, true);  
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);  
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}
//-----------------------------------【Get_FPS】-------------------------------------
//--------【计算帧数】---
//-----------------------------------【Get_FPS】-------------------------------------
float Get_FPS()
{
	//定义四个静态变量
	static float  fps = 0; //我们需要计算的FPS值
	static int    frameCount = 0;//帧数
	static float  currentTime =0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

	//如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if(currentTime - lastTime > 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount /(currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		frameCount    = 0;//将本次帧数frameCount值清零
	}

	return fps;
}
//-----------------------------------【Show_FPS】-------------------------------------
//------------【显示帧数】
//-----------------------------------【Show_FPS】-------------------------------------
void	Show_FPS(HWND hwnd)
{
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//在窗口右上角处，显示每秒帧数
	formatRect.top = 20;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));
	D3DXVECTOR3 vp1;
	vp1= g_player->GetPosition();
	formatRect.top = 40;
	int charCount2 = swprintf_s(g_strFPS, 50, _T("XYZ:%0.3f  %0.3f  %0.3f"), vp1.x,vp1.y,vp1.z );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount2 , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));

}
//-----------------------------------【setcursor】-------------------------------------
//------------【限制鼠标移动】
//-----------------------------------【setcursor】-------------------------------------
void setcursor()
{
	//以下这段代码用于限制鼠标光标移动区域
	POINT lt,rb;
	RECT rect;
	GetClientRect(g_Hwnd,&rect);  //取得窗口内部矩形
	//将矩形左上点坐标存入lt中
	lt.x = rect.left;
	lt.y = rect.top;
	//将矩形右下坐标存入rb中
	rb.x = rect.right;
	rb.y = rect.bottom;
	//将lt和rb的窗口坐标转换为屏幕坐标
	ClientToScreen(g_Hwnd,&lt);
	ClientToScreen(g_Hwnd,&rb);
	//以屏幕坐标重新设定矩形区域
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//限制鼠标光标移动区域
	ClipCursor(&rect);
};
//-----------------------------------【CheckInput】-------------------------------------
//------------【检查鼠标键盘输入 修改状态】
//-----------------------------------【CheckInput】-------------------------------------
void				CheckInput()
{
	//使用DirectInput类读取数据
	g_pDirectInput->GetInput();

	//【W前进，A左转，D右转】  

	if(g_ACTtime==0&&g_PlayerState==People_State_daiji)
	{
		g_PlayerAction = People_Action_stand;	//如无操作，为待机状态

		if (g_pDirectInput->IsKeyDown(DIK_W)||g_pDirectInput->IsKeyDown(DIK_S)||g_pDirectInput->IsKeyDown(DIK_A)||g_pDirectInput->IsKeyDown(DIK_D))	
		{
			if (g_pDirectInput->IsKeyDown(DIK_W))
			{
				if (g_pDirectInput->IsKeyDown(DIK_LCONTROL))
				{
					g_player->MoveLook(3.0f);
					g_PlayerAction=People_Action_run;
				} 
				else
				{
					g_player->MoveLook(1.0f);
					g_PlayerAction=People_Action_walk;
				}
			}
			else if(g_pDirectInput->IsKeyDown(DIK_S))
			{
				if (g_pDirectInput->IsKeyDown(DIK_LCONTROL))
				{
					g_player->MoveLook(-3.0f);
					g_PlayerAction=People_Action_back2;
				} 
				else
				{
					g_player->MoveLook(-1.0f);
					g_PlayerAction=People_Action_back;
				}
			}
			else if (g_pDirectInput->IsKeyDown(DIK_A))
			{
				g_player->MoveLR(-0.1f);
				g_PlayerAction = People_Action_walkLeft;
			}
			else if (g_pDirectInput->IsKeyDown(DIK_D))
			{
				g_player->MoveLR(0.1f);
				g_PlayerAction = People_Action_walkRight;
			}
			g_player->Rotation(-0.002f*g_pDirectInput->MouseDx());
			g_player->CaculateMatrix(&g_playerMat);
		}
		if (g_pDirectInput->IsMouseDown(0))		//如果点了左键，旋转方向
		{
			g_player->Rotation(-0.002f*g_pDirectInput->MouseDx());
		}
		if (g_pDirectInput->IsKeyDown(DIK_1))
		{
			g_PlayerState=People_State_zhandou;
			g_PlayerAction=People_Action_attack1;
		}
	}
}
//--------------------------------
//---------------【人物动作模型加载】
//---------------------------------
void			Player_Init()
{
	//---【待机动作People_Action_stand】
	g_ppAllocateHier[0] = new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\0 待机\\stand.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[0], NULL, &g_ppFrameRoot[0], &g_ppAnimController[0]);

	//---【行走People_Action_walk】
	g_ppAllocateHier[10]= new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\1 行走\\walk.X", D3DXMESH_MANAGED, g_pd3dDevice,			//media\\people\\player\\man\\1 行走\\标准男新手装.tga
		g_ppAllocateHier[10], NULL, &g_ppFrameRoot[10], &g_ppAnimController[10]);

	//---【行走People_Action_walkLeft】
	g_ppAllocateHier[11]= new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\1.2 左挪步\\walkleft.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[11], NULL, &g_ppFrameRoot[11], &g_ppAnimController[11]);

	//---【行走People_Action_walkRight】
	g_ppAllocateHier[12]= new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\1.1 右挪步\\walkright.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[12], NULL, &g_ppFrameRoot[12], &g_ppAnimController[12]);

	//---【跑步People_Action_run】
	g_ppAllocateHier[20]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\2 奔跑\\run.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[20], NULL, &g_ppFrameRoot[20], &g_ppAnimController[20]);	

	//---【后退People_Action_back】
	g_ppAllocateHier[30]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\3 后退\\walkback.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[30], NULL, &g_ppFrameRoot[30], &g_ppAnimController[30]);	

	//---【后退People_Action_back2】
	g_ppAllocateHier[31]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\3 后退\\walkbackquik.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[31], NULL, &g_ppFrameRoot[31], &g_ppAnimController[31]);	

	//---【攻击1People_Action_attack1】
	g_ppAllocateHier[40]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\4 攻击-2回旋剑-30\\GJ_huixuanjian.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[40], NULL, &g_ppFrameRoot[40], &g_ppAnimController[40]);


	for (int i=0;i<81;i++)
	{
		if (g_ppAllocateHier[i]!=NULL)
		{
			SetupBoneMatrixPointers(g_ppFrameRoot[i], g_ppFrameRoot[i]);
		}
	}

}