#pragma once
#include "D3DUnit.h"


class XFileModelClass
{
public:
	XFileModelClass(IDirect3DDevice9 *pd3dDevice);
	~XFileModelClass(void);
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;			//D3D�豸����  
	LPD3DXMESH				m_pMesh;				//����ģ�Ͷ���  
	DWORD					m_dwNumMaterials;		//���ʵ�����  
	D3DMATERIAL9 *			m_pMaterials;			//ģ�Ͳ��ʽṹ���ʵ��  
	LPDIRECT3DTEXTURE9 *	m_pTextures;			//ģ������ṹ���ʵ��
	LPD3DXBUFFER			m_pAdjacencyBuffer;  //����ģ���ڽ���Ϣ  
public:
	LPD3DXMESH GetpMesh(){return m_pMesh;}
	LPD3DXBUFFER GetpAdj(){return m_pAdjacencyBuffer;}
	HRESULT     LoadModelFromXFile(WCHAR* strFilename ); //��.X�ļ���ȡ��άģ�͵��ڴ���  
	HRESULT     RenderModel( );  //��Ⱦ��ά����ģ��  
	void		SetAlpha(float x)
	{
		for (DWORD i=0; i<m_dwNumMaterials; i++)   
		{  
			m_pMaterials[i].Diffuse.a+= (x*0.01f);  
		}
	}

};


class XModelClass :public XFileModelClass
{
public:
	XModelClass(IDirect3DDevice9 *pd3dDevice):XFileModelClass(pd3dDevice)
	{
		XModelNum++;
	}
	~XModelClass();
	
	static int XModelNum;

private:
	D3DXMATRIX				m_matrix;
};

