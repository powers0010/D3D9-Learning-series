#include "StdAfx.h"
#include "XFileModelClass.h"


XFileModelClass::XFileModelClass(IDirect3DDevice9 *pd3dDevice)	//构造函数
{
	m_pd3dDevice	=	pd3dDevice;

	// 	m_pMesh	=	NULL;
	// 	m_dwNumMaterials	=	0;
	// 	m_pMaterials	=	NULL;
	// 	m_pTextures		=	NULL;
}


XFileModelClass::~XFileModelClass(void)
{
	for (DWORD i = 0; i<m_dwNumMaterials; i++) 
		SAFE_RELEASE(m_pTextures[i]);
	SAFE_DELETE(m_pMaterials); 
	SAFE_RELEASE(m_pMesh);
	SAFE_DELETE(m_pd3dDevice);
}

HRESULT XFileModelClass::LoadModelFromXFile( wchar_t* strFilename ) //从.X文件读取三维模型到内存中  
{

	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;   //存储网格模型材质的缓存对象  

	//从磁盘文件加载网格模型  
	D3DXLoadMeshFromX(strFilename,D3DXMESH_MANAGED,m_pd3dDevice,&m_pAdjacencyBuffer,&pD3DXMtrlBuffer,NULL,&m_dwNumMaterials,&m_pMesh); 

	// 读取材质和纹理数据
	D3DXMATERIAL * d3dxMaterials  = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer(); //创建一个D3DXMATERIAL结构体用于读取材质和纹理信息
	m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for(DWORD i=0; i<m_dwNumMaterials; i++ )  
	{  
		//获取材质，并设置一下环境光的颜色值  
		m_pMaterials[i]= d3dxMaterials[i].MatD3D;  
//		m_pMaterials[i].Ambient= m_pMaterials[i].Diffuse;  

		//创建一下纹理对象  
		m_pTextures[i]= NULL;  
		if(d3dxMaterials[i].pTextureFilename != NULL &&  
			strlen(d3dxMaterials[i].pTextureFilename)> 0 )  
		{  
			//创建纹理  
			if(FAILED( D3DXCreateTextureFromFileA(m_pd3dDevice,d3dxMaterials[i].pTextureFilename, &m_pTextures[i] ) ) )  
			{  
				MessageBox(NULL,L"SORRY~!没有找到纹理文件!", L"XFileModelClass类读取文件错误", MB_OK);  
			}  
		}  
	}

	//网格优化
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_STRIPREORDER,(DWORD*)m_pAdjacencyBuffer->GetBufferPointer(),0,0,0);
	d3dxMaterials=NULL;
	SAFE_RELEASE(pD3DXMtrlBuffer);

	return S_OK;
}
HRESULT  XFileModelClass::RenderModel( )  //渲染三维网格模型  
{
	// 用一个for循环，进行网格各个部分的绘制
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		m_pd3dDevice->SetMaterial(&m_pMaterials[i]);//设置此部分的材质
		m_pd3dDevice->SetTexture(0, m_pTextures[i]);//设置此部分的纹理
		m_pMesh->DrawSubset(i);//绘制此部分
	}
	return S_OK;
}