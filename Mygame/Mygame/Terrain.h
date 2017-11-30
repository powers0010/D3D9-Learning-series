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
	IDirect3DTexture9	*	m_pTexture;				//��������
	IDirect3DVertexBuffer9	*	m_pVertexBuffer;	//���㻺��
	IDirect3DIndexBuffer9	*	m_pIndexBuffer;		//��������

	int			m_nCellsPerRow;     // ÿ�еĵ�Ԫ����  
	int			m_nCellsPerCol;		// ÿ�еĵ�Ԫ����  
	int			m_nVertsPerRow;     // ÿ�еĶ�����  
	int			m_nVertsPerCol;		// ÿ�еĶ�����  
	int			m_nNumVertices;     // ��������  
	FLOAT		m_fTerrainWidth;	// ���εĿ��  
	FLOAT		m_fTerrainDepth;	// ���ε����  
	FLOAT		m_fCellSpacing;		// ��Ԫ��ļ��  
	FLOAT		m_fHeightScale;		// �߶�����ϵ��  
	std::vector<FLOAT>   m_vHeightInfo;           // ���ڴ�Ÿ߶���Ϣ	

	//����һ�����ε�FVF�����ʽ  
	struct TERRAINVERTEX  
	{  
		FLOAT _x, _y, _z;  
		FLOAT _u, _v;  
		TERRAINVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)   
			:_x(x), _y(y), _z(z), _u(u), _v(v) {}  
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;  
	};  
public:
	BOOL LoadTerrainFromFile(wchar_t *pRawFileName, wchar_t *pTextureFile);     //���ļ����ظ߶�ͼ������ĺ���  
	BOOL InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale);			//���γ�ʼ������  
	BOOL RenderTerrain(D3DXMATRIX *pMatWorld, BOOL bDrawFrame=FALSE);			//������Ⱦ����
	float GetHeight(int Row,int Col);//��ȡ�߶�
	float GetMapHeight(int x,int y);//��ȡÿ�������ǵĸ߶�
	float ComputeShade(int x,int y,D3DXVECTOR3 * Dlight);//���������ÿ��λ�õĹⰵ����
	bool LightTerrain(D3DXVECTOR3 * Dlight);		//������ÿ�������ⰵ����
	float ClassGetMapHeight(float x,float z);		//��ȡʵʱ���θ߶�


};

float Lerp(float x,float y,float s);			//���Բ�ֵ����


