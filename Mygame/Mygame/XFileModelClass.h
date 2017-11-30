#pragma once
#include "D3DUnit.h"


class XFileModelClass
{
public:
	XFileModelClass(IDirect3DDevice9 *pd3dDevice);
	~XFileModelClass(void);
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;			//D3D设备对象  
	LPD3DXMESH				m_pMesh;				//网格模型对象  
	DWORD					m_dwNumMaterials;		//材质的数量  
	D3DMATERIAL9 *			m_pMaterials;			//模型材质结构体的实例  
	LPDIRECT3DTEXTURE9 *	m_pTextures;			//模型纹理结构体的实例
	LPD3DXBUFFER			m_pAdjacencyBuffer;  //网格模型邻接信息  
public:
	LPD3DXMESH GetpMesh(){return m_pMesh;}
	LPD3DXBUFFER GetpAdj(){return m_pAdjacencyBuffer;}
	HRESULT     LoadModelFromXFile(WCHAR* strFilename ); //从.X文件读取三维模型到内存中  
	HRESULT     RenderModel( );  //渲染三维网格模型  
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

