// Mygame.cpp : ����Ӧ�ó������ڵ㡣
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



//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------
#pragma comment(lib, "winmm.lib") //timegettimeʹ��


//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
//-----�� ����ȫ�ֱ�����
HINSTANCE					hInst;								// ��ǰʵ��		
HWND						g_Hwnd;
TCHAR						szTitle[MAX_LOADSTRING];			// �������ı�
TCHAR						szWindowClass[MAX_LOADSTRING];		// ����������
D3DLIGHT9					g_Light;   //ȫ�ֹ���
//------����Դ�� ����
LPDIRECT3DDEVICE9			g_pd3dDevice = NULL;			//Direct3D�豸����
DInput*						g_pDirectInput=NULL;			//DirectInput����
D3DGUIClass*				g_gui=NULL;						//GUI����ָ��
CameraClass*				g_pCamera=NULL;					//�����
TerrainClass*				g_pTerrain=NULL;				//������
SkyBoxClass*				g_pSkyBox=NULL;					//��պ�
LPD3DXFONT					g_pTextFPS =NULL;				//����COM�ӿ�
wchar_t						g_strFPS[50] ={0};				//����֡���ʵ��ַ�����

//-----���ذ塿
// ����ذ�FVF����ṹ
struct CUSTOMVERTEX
{
	FLOAT _x, _y, _z;
	FLOAT _u, _v ;
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)

LPDIRECT3DVERTEXBUFFER9		g_pFloorVBuffer      = NULL;  //�ذ嶥�㻺�����
LPDIRECT3DTEXTURE9			g_pFloorTexture   = NULL;  //�ذ��������


//------������ ������Դģ�͡�
XModelClass*				g_ppXViewModel[MAX_XMODELNUM];
int							XModelClass::XModelNum=0;		//��ʼ��XModelNum
D3DXMATRIX					g_EnvirMatrix[MAX_XMODELNUM];	//������������
//--------��������Ϣ��
bool						g_LMBDown = false;				// GUI�е����״̬��Ϣ���������Ƿ��µı�ʶ
float						g_MouseX = 0, g_MouseY = 0;     //�洢����������������

//---------------����ɫ��ء�
People*						g_player=NULL;					//�������	
D3DXMATRIX					g_playerMat;					//���ȫ�־���
float						g_ACTtime=0;					//������ʱ							
People_State				g_PlayerState=People_State_daiji;		//���״̬ 0 ���� -1 ս�� 2����
People_Action				g_PlayerAction=People_Action_stand;		//��ɫ����

//----------------��player��������������
CAllocateHierarchy*				g_ppAllocateHier[81]={NULL};
LPD3DXFRAME						 g_ppFrameRoot[81]={0};
D3DXMATRIX* 					g_ppBoneMatrices[81]={NULL};
LPD3DXANIMATIONCONTROLLER		g_ppAnimController[81]={0};

//----------------��NPC������
int								g_NumNPC=0;
CAllocateHierarchy*				g_ppNPCAllocateHier[MAX_NPCNUM];
LPD3DXFRAME						 g_ppNPCFrameRoot[MAX_NPCNUM];
D3DXMATRIX* 					g_ppNPCBoneMatrices[MAX_NPCNUM];
LPD3DXANIMATIONCONTROLLER		g_ppNPCAnimController[MAX_NPCNUM];
D3DXMATRIX						g_NPCMat[MAX_NPCNUM];					//NPCȫ�־���
//-----------------������������


//-----------------------------------����Ϸ״̬�꡿---------------------------------------
//	������������ǰ������:
//------------------------------------------------------------------------------------------------
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int );
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				GUICallback(int id, int state);					//GUI���̺���
void				Direct3D_Updata(float fTimeDelta);				//����
HRESULT				Resource_Init();
HRESULT				Direct3D_Init(HWND hwnd,HINSTANCE hInstance);	//d3d��ʼ������
void				Direct3D_Render( HWND hwnd,float fTimeDelta);	//d3d��Ⱦ����
HRESULT				Objects_Init();									//d3d��Դ��ʼ��
bool				GUI_Init();										//GUI��Դ��ʼ��
void				LightInit();									//��ʼ���ƹ�
void				OBJ_render(float fTimeDelta);		//���ƶ���		
void				Player_Init();						//player��ɫ������ʼ��
void				Player_render(float fTimeDelta,People_Action i);					//������Ⱦ
void				SetCamera();						//��������λ�����������
float				Get_FPS();							//����֡��
void				Show_FPS(HWND hwnd);							//�����ʾ֡��
void				XViewModelInit();					//���� ������Դ��ʼ��
void				XViewInitRender();				//����������Ⱦ
void				NPCInit();						//NPC��ʼ��
void				NPCRender(float fTimeDelta);	//NPC��Ⱦ
void				setcursor();					//��������ƶ�
void				CheckInput();					//����������������״̬
//-----------��WinMain��-------
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	
	if (!InitInstance (hInstance, nCmdShow ))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGAME));//������ټ���

//	PlaySound(L"media\\BGM\\�ɽ���������.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP); //ѭ�����ű������� 

	HWND hWnd;
	hWnd=GetTopWindow(NULL);
	// ����Ϣѭ��:
	MSG msg={0};
	while (msg.message != WM_QUIT)
	{
		static FLOAT fLastTime  =(float)timeGetTime();  
		static FLOAT fCurrTime  =(float)timeGetTime();  
		static FLOAT fTimeDelta = 0.0f;  
		fCurrTime  = (float)timeGetTime();  
		fTimeDelta= (fCurrTime - fLastTime) / 1000.0f;  
		fLastTime  = fCurrTime; 
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );		//�ú����ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			Direct3D_Render(g_Hwnd,fTimeDelta);
		}
	}
	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
//    HWND hWnd;
//    hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   g_Hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!g_Hwnd)
   {
      return FALSE;
   }


   //--------------------------------------------------------------------------------------
   //��ʼ��Direct3D
   //--------------------------------------------------------------------------------------

   if (!(S_OK==Direct3D_Init (g_Hwnd,hInstance)))
   {
	   MessageBox(g_Hwnd, _T("Direct3D��ʼ��ʧ��~��"), _T("ǳī����Ϣ����"), 0); //ʹ��MessageBox����������һ����Ϣ���� 
   }

   //--------------------------------------------------------------------------------------
   //��ʼ��DirectInput  ���ͼ���ģʽ��ǰ̨���Ƕ�ռģʽ
   //--------------------------------------------------------------------------------------
	
   //--------------------------------------------------------------------------------------
   //���ڵ���ʾ�����
   //--------------------------------------------------------------------------------------
   MoveWindow(g_Hwnd,200,20,WINDOW_WIDTH,WINDOW_HEIGHT,true);
   ShowWindow(g_Hwnd, nCmdShow);
   UpdateWindow(g_Hwnd);

   //����DirectInput��ĳ�ʼ��
   g_pDirectInput = new DInput();
   g_pDirectInput->Init(g_Hwnd,hInstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); //ǰ̨���Ƕ�ռģʽ

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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

//-----------------------------------��Direct3D_Init d3d��ʼ����-------------------------------------
//
//-----------------------------------��Direct3D_Init d3d��ʼ����-------------------------------------
HRESULT Direct3D_Init(HWND hwnd,HINSTANCE hInstance)
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
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
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	//��Objects_Init����Դ��ʼ��
	//--------------------------------------------------------------------------------------
	if(!(S_OK==Objects_Init())) return E_FAIL;

	//--------------------------------------------------------------------------------------
	//��LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ���
	//--------------------------------------------------------------------------------------
	SAFE_RELEASE(pD3D)
		return S_OK;

}


//-----------------------------------��Direct3D_Render d3d��Ⱦ��-------------------------------------
//void	Direct3D_Render( HWND hwnd,float fLastTime)
//-----------------------------------��Direct3D_Render d3d��Ⱦ��-------------------------------------
void	Direct3D_Render( HWND hwnd,float fTimeDelta)
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮һ������������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(155, 155, 155), 1.0f, 0);


	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮��������ʼ����
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

		Direct3D_Updata(fTimeDelta);	//���¶���
		
		OBJ_render(fTimeDelta);			//���ƶ���
		
		break;
	}
	

	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�ġ�����������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // ��������
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�塿����ʾ��ת
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // ��ת����ʾ

}

//-----------------------------------�� Objects_Init()��-------------------------------------
//��Դ��ʼ������
//-----------------------------------�� Objects_Init()��-------------------------------------
HRESULT	 Objects_Init()
{
	if (false==GUI_Init())		//��ʼ����ʼ��Ϸ����
	{
		return E_FAIL;
	}

	if (false==Resource_Init())
	{
		return E_FAIL;
	}
	
	return S_OK;
}


//-----------------------------------��Direct3D_Render d3d��Ⱦ��-------------------------------------
//void	Direct3D_Render( HWND hwnd,float fLastTime)
//-----------------------------------��Direct3D_Render d3d��Ⱦ��-------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_ID_1: 
		//����ʼ��Ϸ����ť����ش���ʵ��
		if(state == UGP_BUTTON_DOWN)
			g_GameRunningState=GAME_RUNNING_STATE_UNDERWAY;
		break;
	}
}



//-----------------------------------��Resource_Init��-------------------------------------
//��Դ��ʼ��  �������� ��� 
//-----------------------------------��Resource_Init��-------------------------------------
HRESULT	Resource_Init()
{
	void LightInit();		//��ʼ������

	//��������
	D3DXCreateFont(g_pd3dDevice, 25, 0, 0, 1000, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);

// 	//��ʼ������
	Player_Init();		//�������ﶯ��ģ�ͼ�������Դ
	D3DXMatrixIdentity(&g_playerMat);
	g_player = new People(g_pd3dDevice,PEOPLE_HERO,L"cloud");
	g_player ->SetPosition(NULL);		//���ÿ�ʼ����
	g_player ->SetDirection(NULL);		//���ÿ�ʼ����
	g_player ->CaculateMatrix(&g_playerMat);				//�������ȫ��λ��
	//--���������Ѫ����Դ��
	g_player->InitHPMP(&D3DXVECTOR3(5.0f,5.0f,0.0f),L"media\\people\\player\\man\\Ѫ��\\���Ѫ��.png" , L"media\\people\\player\\man\\Ѫ��\\Ѫ��.png");

	// ��������ʼ�����������
	g_pCamera = new CameraClass(g_pd3dDevice);
	D3DXVECTOR3 vec = g_player->GetPosition();
	g_pCamera->SetCameraPosition(&D3DXVECTOR3(vec.x, vec.y+30.0f, vec.z-60.0f));
	g_pCamera->SetTargetPosition(&D3DXVECTOR3(vec.x, vec.y+30.0f, vec.z));
	g_pCamera->SetViewMatrix();  //����ȡ���任����
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 15000.0f);
	g_pCamera->SetProjMatrix(&matProj);

	// -----------------------------���������涥�㻺�桿--------------------------------
	g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, 
		D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &g_pFloorVBuffer, NULL);
	CUSTOMVERTEX *pVertices = NULL;
	g_pFloorVBuffer->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-5000.0f, 0.0f, -5000.0f,  0.0f, 100.0f);
	pVertices[1] = CUSTOMVERTEX(-5000.0f, 0.0f,  5000.0f,  0.0f,  0.0f);
	pVertices[2] = CUSTOMVERTEX( 5000.0f, 0.0f, -5000.0f, 100.0f, 100.0f); 
	pVertices[3] = CUSTOMVERTEX( 5000.0f, 0.0f,  5000.0f, 100.0f,  0.0f);
	g_pFloorVBuffer->Unlock();
	// -----------------------------��������������---------------------------------------
	D3DXCreateTextureFromFile(g_pd3dDevice, L"media\\Terrain\\brick.bmp", &g_pFloorTexture);

	//��ʼ�������
	g_pSkyBox = new SkyBoxClass(g_pd3dDevice);
	g_pSkyBox->LoadSkyTextureFromFile(
		L"media\\sky\\����\\0.JPG",
		L"media\\sky\\����\\2.JPG",
		L"media\\sky\\����\\3.JPG",
		L"media\\sky\\����\\1.JPG",
		L"media\\sky\\����\\4.JPG"
		);
	g_pSkyBox->InitSkyBox(10000);  //������պеı߳�

	//��ʼ������ģ��
	XViewModelInit();

	//����NPCģ��
//	NPCInit();
	return true;
}
void Direct3D_Updata(float fTimeDelta)
{	
	//�������� ���������Ϣ��
	CheckInput();		
	
	

	//��NPC����ʶ��

	
	//����������ƶ�����
	setcursor();
	
}
//��������Ⱦ������
void OBJ_render(float fTimeDelta)
{
	//���������	player��Ⱦ��
	Player_render(fTimeDelta,g_PlayerAction);
	//�����������λ�á�
	SetCamera();

	//---����������״̬��
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	
	//---�����Ƶذ塿
	D3DXMATRIX matFloor;
	D3DXMatrixTranslation(&matFloor, 0.0f, 0.0f, 0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matFloor);
	g_pd3dDevice->SetStreamSource(0, g_pFloorVBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetTexture(0, g_pFloorTexture);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//-----------------------------��������ա�------------------------
	D3DXMATRIX matSky,matTransSky,matRotSky;
	D3DXMatrixTranslation(&matTransSky,0.0f,-2500.0f,0.0f);
	D3DXMatrixRotationY(&matRotSky, -0.00002f*timeGetTime());   //��ת�������, ��ģ���Ʋ��˶�Ч��
	matSky=matTransSky*matRotSky;
	g_pSkyBox->RenderSkyBox(&matSky, false);

	//�����Ƴ�������

	XViewInitRender();

	//������npc��

	//----����������Ѫ����
	g_player->HPNPrender();

}

//-----------------------------------��Player_render��-------------------------------------
//��������Ⱦ������ͬ������
//-----------------------------------��Player_render��-------------------------------------
void	Player_render(float fTimeDelta ,People_Action i)
{
	//��¼�����ı��
	static int NUM_ACT=0;
	static float ACT_TIME=0.0f;
	// 	// ���ù��������ľ���
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_playerMat);
	if (g_PlayerState==People_State_zhandou)			//--�����״̬�ǹ�����
	{
		switch(i) 
		{
		case People_Action_attack1:
			PlaySound(L"media\\BGM\\������1.wav", NULL, SND_FILENAME | SND_ASYNC |SND_NOSTOP); //ѭ�����ű������� 
			NUM_ACT = 40;
			ACT_TIME = 1.0f;
			break;
		}

		g_ppAnimController[NUM_ACT]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
		UpdateFrameMatrices(g_ppFrameRoot[NUM_ACT], &g_playerMat);   //���¿���еı任����
		DrawFrame(g_pd3dDevice, g_ppFrameRoot[NUM_ACT]);
		g_ACTtime+=fTimeDelta;
		if (g_ACTtime>ACT_TIME)	
		{
			g_ACTtime=0;
			g_PlayerState=People_State_daiji;
			g_PlayerAction=People_Action_stand;
		}

	} 
	else if(g_PlayerState==People_State_daiji)		//---�����״̬������
	{
		switch(i)
		{
		case People_Action_stand://	--��������
			// WALK
			//-----------------------------�����ƹ���������------------------------
			g_ppAnimController[0]->AdvanceTime(0.5*fTimeDelta, NULL);  //���ù���������ʱ��
			UpdateFrameMatrices(g_ppFrameRoot[0], &g_playerMat);   //���¿���еı任����
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[0]);
			break;

		case People_Action_walk://	--�����ߡ�
			//RUN
			// 	//-----------------------------�����ƹ���������------------------------
			g_ppAnimController[10]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
			UpdateFrameMatrices(g_ppFrameRoot[10], &g_playerMat);   //���¿���еı任����
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[10]);
			break;

		case People_Action_walkLeft://	--�����ߡ�
			// 	//-----------------------------�����ƹ���������------------------------
			g_ppAnimController[11]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
			UpdateFrameMatrices(g_ppFrameRoot[11], &g_playerMat);   //���¿���еı任����
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[11]);
			break;

		case People_Action_walkRight://	--�����ߡ�
			// 	//-----------------------------�����ƹ���������------------------------
			g_ppAnimController[12]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
			UpdateFrameMatrices(g_ppFrameRoot[12], &g_playerMat);   //���¿���еı任����
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[12]);
			break;

		case People_Action_run:	//--���ܲ���
			// 	//-----------------------------�����ƹ���������------------------------
			g_ppAnimController[20]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
			UpdateFrameMatrices(g_ppFrameRoot[20], &g_playerMat);   //���¿���еı任����
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[20]);
			break;

		case People_Action_back:	//--�����ˡ�
			//-----------------------------�����ƹ���������------------------------
			g_ppAnimController[30]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
			UpdateFrameMatrices(g_ppFrameRoot[30], &g_playerMat);   //���¿���еı任����
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[30]);
			break;
		case People_Action_back2:	//--�����ˡ�
			//-----------------------------�����ƹ���������------------------------
			g_ppAnimController[31]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
			UpdateFrameMatrices(g_ppFrameRoot[31], &g_playerMat);   //���¿���еı任����
			DrawFrame(g_pd3dDevice, g_ppFrameRoot[31]);
			break;
		}
	}
}

//-----------------------------------��SetCamera��-------------------------------------
//��������λ�����������λ��
//-----------------------------------��SetCamera��-------------------------------------
void				SetCamera()
{
	static float fltZ=80;
	fltZ+=0.05f*g_pDirectInput->MouseDz();	//�������ž�ͷ
	if (fltZ<10.0f)
	{
		fltZ=10.0f;
	}
	//���������
	D3DXVECTOR3 pos1=g_player->GetPosition();
	D3DXVECTOR3 pos2=g_player->GetDirection();
	D3DXVECTOR3 set1 = D3DXVECTOR3(pos1.x,pos1.y+20,pos1.z)+fltZ*pos2;
	D3DXVECTOR3 set2 = D3DXVECTOR3(pos1.x,pos1.y+20,pos1.z);
	g_pCamera->SetCameraPosition(&set1);
	g_pCamera->SetTargetPosition(&set2);

	//����ȡ���任���󣬲�����
	D3DXMATRIX matView;  
	g_pCamera->CalculateViewMatrix(&matView);  
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}
//-----------------------------------��GUI_Init��-------------------------------------
//GUI�����ʼ������
//-----------------------------------��GUI_Init��-------------------------------------
bool GUI_Init()
{
	g_gui=new D3DGUIClass(g_pd3dDevice,WINDOW_WIDTH,WINDOW_HEIGHT);
	// ��ӱ���ͼƬ
	if(!g_gui->AddBackground(L"media\\GUI menu\\Assassins creed.jpg")) return false;

	// ���4����ť���ֱ��ǿ�ʼ��Ϸ��������ȣ�ѡ����˳���Ϸ��ÿ����ť��Ӧ3��ͼ
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

//-----------------------------------��NPCInit��-------------------------------------
//NPC��ʼ��
//-----------------------------------��NPCInit��-------------------------------------
void				NPCInit()
{
	//NPC��ʼ��
	//��Ů1
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
//-------------��������Դ��ʼ����
void	XViewModelInit()
{
// 	//0---����ǽ��
  	g_ppXViewModel[0]=new XModelClass(g_pd3dDevice);
  	g_ppXViewModel[0]->LoadModelFromXFile(L"media\\����\\��ǽ\\CQ_chengqiang.X");
  	D3DXMATRIX mat0,mat0s,mat0r,mat0t;
  	D3DXMatrixIdentity(&mat0);
  	D3DXMatrixScaling(&mat0s,0.4f,0.4f,0.4f);
  	D3DXMatrixRotationY(&mat0r,0.0f);
  	D3DXMatrixTranslation(&mat0t,-6400.0f,0.0f,6000.0f);
  	g_EnvirMatrix[0]= mat0 * mat0s * mat0r * mat0t;

	//1--��������
	g_ppXViewModel[1]=new XModelClass(g_pd3dDevice);
	g_ppXViewModel[1]->LoadModelFromXFile(L"media\\����\\����01\\JZ_chasi.X");
	D3DXMATRIX mat1,mat1s,mat1r,mat1t;
	D3DXMatrixIdentity(&mat1);
	D3DXMatrixScaling(&mat1s,1.0f,1.0f,1.0f);
	D3DXMatrixRotationY(&mat1r,0.0f);
	D3DXMatrixTranslation(&mat1t,0.0f,0.0f,100.0f);
	g_EnvirMatrix[1]= mat0 * mat0s * mat0r * mat0t;

}
//-------------������������Ⱦ��
void XViewInitRender()
{
 	for (int i=0;i<XModelClass::XModelNum;i++)
 	{
		g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_EnvirMatrix[0]);
		g_ppXViewModel[0]->RenderModel();
	}	

}

//-----------------------------------��NPCRender��-------------------------------------
//NPC��Ⱦ
//-----------------------------------��NPCRender��-------------------------------------
void				NPCRender(float fTimeDelta)
{
	for (int i=0;i<g_NumNPC;i++)
	{
		g_ppNPCAnimController[i]->AdvanceTime(fTimeDelta, NULL);  //���ù���������ʱ��
		UpdateFrameMatrices(g_ppNPCFrameRoot[i], &g_NPCMat[i]);   //���¿���еı任����
		DrawFrame(g_pd3dDevice, g_ppNPCFrameRoot[i]);
	}
}
//-----------------------------------��LightInit��-------------------------------------
//��ʼ���ƹ�
//-----------------------------------��LightInit��-------------------------------------
void LightInit()
{
	// ���ù���  
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
//-----------------------------------��Get_FPS��-------------------------------------
//--------������֡����---
//-----------------------------------��Get_FPS��-------------------------------------
float Get_FPS()
{
	//�����ĸ���̬����
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int    frameCount = 0;//֡��
	static float  currentTime =0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

	//�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if(currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount /(currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount    = 0;//������֡��frameCountֵ����
	}

	return fps;
}
//-----------------------------------��Show_FPS��-------------------------------------
//------------����ʾ֡����
//-----------------------------------��Show_FPS��-------------------------------------
void	Show_FPS(HWND hwnd)
{
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//�ڴ������ϽǴ�����ʾÿ��֡��
	formatRect.top = 20;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));
	D3DXVECTOR3 vp1;
	vp1= g_player->GetPosition();
	formatRect.top = 40;
	int charCount2 = swprintf_s(g_strFPS, 50, _T("XYZ:%0.3f  %0.3f  %0.3f"), vp1.x,vp1.y,vp1.z );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount2 , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));

}
//-----------------------------------��setcursor��-------------------------------------
//------------����������ƶ���
//-----------------------------------��setcursor��-------------------------------------
void setcursor()
{
	//������δ�����������������ƶ�����
	POINT lt,rb;
	RECT rect;
	GetClientRect(g_Hwnd,&rect);  //ȡ�ô����ڲ�����
	//���������ϵ��������lt��
	lt.x = rect.left;
	lt.y = rect.top;
	//�����������������rb��
	rb.x = rect.right;
	rb.y = rect.bottom;
	//��lt��rb�Ĵ�������ת��Ϊ��Ļ����
	ClientToScreen(g_Hwnd,&lt);
	ClientToScreen(g_Hwnd,&rb);
	//����Ļ���������趨��������
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//����������ƶ�����
	ClipCursor(&rect);
};
//-----------------------------------��CheckInput��-------------------------------------
//------------��������������� �޸�״̬��
//-----------------------------------��CheckInput��-------------------------------------
void				CheckInput()
{
	//ʹ��DirectInput���ȡ����
	g_pDirectInput->GetInput();

	//��Wǰ����A��ת��D��ת��  

	if(g_ACTtime==0&&g_PlayerState==People_State_daiji)
	{
		g_PlayerAction = People_Action_stand;	//���޲�����Ϊ����״̬

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
		if (g_pDirectInput->IsMouseDown(0))		//��������������ת����
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
//---------------�����ﶯ��ģ�ͼ��ء�
//---------------------------------
void			Player_Init()
{
	//---����������People_Action_stand��
	g_ppAllocateHier[0] = new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\0 ����\\stand.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[0], NULL, &g_ppFrameRoot[0], &g_ppAnimController[0]);

	//---������People_Action_walk��
	g_ppAllocateHier[10]= new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\1 ����\\walk.X", D3DXMESH_MANAGED, g_pd3dDevice,			//media\\people\\player\\man\\1 ����\\��׼������װ.tga
		g_ppAllocateHier[10], NULL, &g_ppFrameRoot[10], &g_ppAnimController[10]);

	//---������People_Action_walkLeft��
	g_ppAllocateHier[11]= new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\1.2 ��Ų��\\walkleft.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[11], NULL, &g_ppFrameRoot[11], &g_ppAnimController[11]);

	//---������People_Action_walkRight��
	g_ppAllocateHier[12]= new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\1.1 ��Ų��\\walkright.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[12], NULL, &g_ppFrameRoot[12], &g_ppAnimController[12]);

	//---���ܲ�People_Action_run��
	g_ppAllocateHier[20]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\2 ����\\run.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[20], NULL, &g_ppFrameRoot[20], &g_ppAnimController[20]);	

	//---������People_Action_back��
	g_ppAllocateHier[30]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\3 ����\\walkback.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[30], NULL, &g_ppFrameRoot[30], &g_ppAnimController[30]);	

	//---������People_Action_back2��
	g_ppAllocateHier[31]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\3 ����\\walkbackquik.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[31], NULL, &g_ppFrameRoot[31], &g_ppAnimController[31]);	

	//---������1People_Action_attack1��
	g_ppAllocateHier[40]=new CAllocateHierarchy();
	D3DXLoadMeshHierarchyFromX(L"media\\people\\player\\man\\4 ����-2������-30\\GJ_huixuanjian.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		g_ppAllocateHier[40], NULL, &g_ppFrameRoot[40], &g_ppAnimController[40]);


	for (int i=0;i<81;i++)
	{
		if (g_ppAllocateHier[i]!=NULL)
		{
			SetupBoneMatrixPointers(g_ppFrameRoot[i], g_ppFrameRoot[i]);
		}
	}

}