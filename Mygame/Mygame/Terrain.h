#pragma once

#include <vector>  
#include <fstream>  
#include  "D3DUnit.h"

class TerrainClass
{
public:
	TerrainClass(IDirect3DDevice9 *pd3dDevice);
	virtual ~TerrainClass(void);
private:
	IDirect3DDevice9	*	m_pd3dDevice;			//
	IDirect3DTexture9	*	m_pTexture;				//地形纹理
	IDirect3DVertexBuffer9	*	m_pVertexBuffer;	//顶点缓存
	IDirect3DIndexBuffer9	*	m_pIndexBuffer;		//索引缓存

	int			m_nCellsPerRow;     // 每行的单元格数  
	int			m_nCellsPerCol;		// 每列的单元格数  
	int			m_nVertsPerRow;     // 每行的顶点数  
	int			m_nVertsPerCol;		// 每列的顶点数  
	int			m_nNumVertices;     // 顶点总数  
	FLOAT		m_fTerrainWidth;	// 地形的宽度  
	FLOAT		m_fTerrainDepth;	// 地形的深度  
	FLOAT		m_fCellSpacing;		// 单元格的间距  
	FLOAT		m_fHeightScale;		// 高度缩放系数  
	std::vector<FLOAT>   m_vHeightInfo;           // 用于存放高度信息	

	//定义一个地形的FVF顶点格式  
	struct TERRAINVERTEX  
	{  
		FLOAT _x, _y, _z;  
		FLOAT _u, _v;  
		TERRAINVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)   
			:_x(x), _y(y), _z(z), _u(u), _v(v) {}  
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;  
	};  
public:
	BOOL LoadTerrainFromFile(wchar_t *pRawFileName, wchar_t *pTextureFile);     //从文件加载高度图和纹理的函数  
	BOOL InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale);			//地形初始化函数  
	BOOL RenderTerrain(D3DXMATRIX *pMatWorld, BOOL bDrawFrame=FALSE);			//地形渲染函数
	float GetHeight(int Row,int Col);//获取高度
	float GetMapHeight(int x,int y);//获取每个像素是的高度
	float ComputeShade(int x,int y,D3DXVECTOR3 * Dlight);//计算地形上每个位置的光暗因子
	bool LightTerrain(D3DXVECTOR3 * Dlight);		//给地形每个点计算光暗因子
	float ClassGetMapHeight(float x,float z);		//获取实时地形高度


};

float Lerp(float x,float y,float s);			//线性差值函数


