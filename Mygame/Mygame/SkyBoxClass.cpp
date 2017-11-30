#include "StdAfx.h"
#include "SkyBoxClass.h"

//=============================================================================
#include "d3dx9.h"
#include "SkyBoxClass.h"
#include "D3DUnit.h"


//-------------------------------------------------------------------------------------------------
// Desc: 构造函数
//-------------------------------------------------------------------------------------------------
SkyBoxClass::SkyBoxClass( LPDIRECT3DDEVICE9  pDevice )
{
	//给各个参数赋初值
	m_pVertexBuffer=NULL;
	m_pd3dDevice=pDevice;
	for(int i=0; i<5; i++)
		m_pTexture[i] = NULL; 
	m_Length = 0.0f;
}

//-------------------------------------------------------------------------------------------------
// Name:  SkyBoxClass::InitSkyBox( )
// Desc: 天空盒初始化函数，顶点缓冲区的赋值
//-------------------------------------------------------------------------------------------------
BOOL SkyBoxClass::InitSkyBox( float Length )
{
	m_Length=Length;

	//1.创建。创建顶点缓存
	m_pd3dDevice->CreateVertexBuffer( 20 * sizeof(SKYBOXVERTEX), 0, 
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pVertexBuffer, 0 );

	//用一个结构体把顶点数据先准备好
	SKYBOXVERTEX vertices[] =
	{
		//前面的四个顶点
		{ -m_Length/2, 0.0f,    m_Length/2, 0.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,   m_Length/2, 0.0f, 0.0f, },
		{  m_Length/2, 0.0f,    m_Length/2, 1.0f, 1.0f, },
		{  m_Length/2, m_Length/2,   m_Length/2, 1.0f, 0.0f, },

		//背面的四个顶点
		{  m_Length/2, 0.0f,   -m_Length/2, 0.0f, 1.0f, },
		{  m_Length/2, m_Length/2,  -m_Length/2, 0.0f, 0.0f, },
		{ -m_Length/2, 0.0f,   -m_Length/2, 1.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,  -m_Length/2, 1.0f, 0.0f, },

		//左面的四个顶点
		{ -m_Length/2, 0.0f,   -m_Length/2, 0.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,  -m_Length/2, 0.0f, 0.0f, },
		{ -m_Length/2, 0.0f,    m_Length/2, 1.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,   m_Length/2, 1.0f, 0.0f, },

		//右面的四个顶点
		{ m_Length/2, 0.0f,   m_Length/2, 0.0f, 1.0f, },
		{ m_Length/2, m_Length/2,  m_Length/2, 0.0f, 0.0f, },
		{ m_Length/2, 0.0f,  -m_Length/2, 1.0f, 1.0f, },
		{ m_Length/2, m_Length/2, -m_Length/2, 1.0f, 0.0f, },

		//上面的四个顶点
		{  m_Length/2, m_Length/2, -m_Length/2, 1.0f, 0.0f, },
		{  m_Length/2, m_Length/2,  m_Length/2, 1.0f, 1.0f, },
		{ -m_Length/2, m_Length/2, -m_Length/2, 0.0f, 0.0f, },
		{ -m_Length/2, m_Length/2,  m_Length/2, 0.0f, 1.0f, },

	};

	//准备填充顶点数据
	void* pVertices;
	//2.加锁
	m_pVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );
	//3.访问。把结构体中的数据直接拷到顶点缓冲区中
	memcpy( pVertices, vertices, sizeof(vertices) );
	//4.解锁
	m_pVertexBuffer->Unlock();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
// Name:  SkyBoxClass::LoadSkyTextureFromFile( )
// Desc: 天空盒纹理加载函数
//-------------------------------------------------------------------------------------------------
BOOL SkyBoxClass::LoadSkyTextureFromFile(
	wchar_t *pFrontTextureFile, 
	wchar_t *pBackTextureFile,
	wchar_t *pLeftTextureFile, 
	wchar_t *pRightTextureFile,
	wchar_t *pTopTextureFile)
{
	//从文件加载五张纹理
	D3DXCreateTextureFromFile( m_pd3dDevice , pFrontTextureFile, &m_pTexture[0] );  //前面
	D3DXCreateTextureFromFile( m_pd3dDevice , pBackTextureFile,  &m_pTexture[1] );  //后面
	D3DXCreateTextureFromFile( m_pd3dDevice , pLeftTextureFile,  &m_pTexture[2] );  //左面
	D3DXCreateTextureFromFile( m_pd3dDevice , pRightTextureFile, &m_pTexture[3] );  //右面
	D3DXCreateTextureFromFile( m_pd3dDevice , pTopTextureFile,   &m_pTexture[4] );  //上面	
	return TRUE;
}



//--------------------------------------------------------------------------------------
// Name: SkyBoxClass::RenderSkyBox()
// Desc: 绘制出天空盒，可以通过第二个参数选择是否绘制出线框
//--------------------------------------------------------------------------------------
void SkyBoxClass::RenderSkyBox( D3DXMATRIX *pMatWorld, BOOL bRenderFrame )
{
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   true );
//	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);  //将纹理颜色混合的第一个参数的颜色值用于输出
//	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );   //纹理颜色混合的第一个参数的值就取纹理颜色值
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);   //开启背面消隐
	m_pd3dDevice->SetTransform( D3DTS_WORLD, pMatWorld );						 //设置世界矩阵
	m_pd3dDevice->SetStreamSource(0,m_pVertexBuffer, 0, sizeof(SKYBOXVERTEX));    //把包含的几何体信息的顶点缓存和渲染流水线相关联  
	m_pd3dDevice->SetFVF(D3DFVF_SKYBOX);  //设置FVF灵活顶点格式

	//一个for循环，将5个面绘制出来
	for(int i =0; i<5; i++)
	{
		m_pd3dDevice->SetTexture(0, m_pTexture[i]);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*4, 2);
	}
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 
}


//-------------------------------------------------------------------------------------------------
// Desc: 析构函数
//-------------------------------------------------------------------------------------------------
SkyBoxClass::~SkyBoxClass(void)
{
	SAFE_RELEASE( m_pVertexBuffer );
	for(int i=0; i<5; i++)
	{
		SAFE_RELEASE( m_pTexture[i] );
	}
}