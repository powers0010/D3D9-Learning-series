#include "StdAfx.h"
#include "XFileModelClass.h"


XFileModelClass::XFileModelClass(IDirect3DDevice9 *pd3dDevice)	//���캯��
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

HRESULT XFileModelClass::LoadModelFromXFile( wchar_t* strFilename ) //��.X�ļ���ȡ��άģ�͵��ڴ���  
{

	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;   //�洢����ģ�Ͳ��ʵĻ������  

	//�Ӵ����ļ���������ģ��  
	D3DXLoadMeshFromX(strFilename,D3DXMESH_MANAGED,m_pd3dDevice,&m_pAdjacencyBuffer,&pD3DXMtrlBuffer,NULL,&m_dwNumMaterials,&m_pMesh); 

	// ��ȡ���ʺ���������
	D3DXMATERIAL * d3dxMaterials  = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer(); //����һ��D3DXMATERIAL�ṹ�����ڶ�ȡ���ʺ�������Ϣ
	m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for(DWORD i=0; i<m_dwNumMaterials; i++ )  
	{  
		//��ȡ���ʣ�������һ�»��������ɫֵ  
		m_pMaterials[i]= d3dxMaterials[i].MatD3D;  
//		m_pMaterials[i].Ambient= m_pMaterials[i].Diffuse;  

		//����һ���������  
		m_pTextures[i]= NULL;  
		if(d3dxMaterials[i].pTextureFilename != NULL &&  
			strlen(d3dxMaterials[i].pTextureFilename)> 0 )  
		{  
			//��������  
			if(FAILED( D3DXCreateTextureFromFileA(m_pd3dDevice,d3dxMaterials[i].pTextureFilename, &m_pTextures[i] ) ) )  
			{  
				MessageBox(NULL,L"SORRY~!û���ҵ������ļ�!", L"XFileModelClass���ȡ�ļ�����", MB_OK);  
			}  
		}  
	}

	//�����Ż�
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_STRIPREORDER,(DWORD*)m_pAdjacencyBuffer->GetBufferPointer(),0,0,0);
	d3dxMaterials=NULL;
	SAFE_RELEASE(pD3DXMtrlBuffer);

	return S_OK;
}
HRESULT  XFileModelClass::RenderModel( )  //��Ⱦ��ά����ģ��  
{
	// ��һ��forѭ������������������ֵĻ���
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		m_pd3dDevice->SetMaterial(&m_pMaterials[i]);//���ô˲��ֵĲ���
		m_pd3dDevice->SetTexture(0, m_pTextures[i]);//���ô˲��ֵ�����
		m_pMesh->DrawSubset(i);//���ƴ˲���
	}
	return S_OK;
}