#include "StdAfx.h"
#include "Terrain.h"


//
using std::ifstream;
using std::vector;

TerrainClass::TerrainClass(IDirect3DDevice9 *pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pTexture = NULL;
	m_pIndexBuffer = NULL;
	m_pVertexBuffer = NULL;
	m_nCellsPerRow = 0;
	m_nCellsPerCol = 0;
	m_nVertsPerRow = 0;
	m_nVertsPerCol = 0;
	m_nNumVertices = 0;
	m_fTerrainWidth = 0.0f;
	m_fTerrainDepth = 0.0f;
	m_fCellSpacing = 0.0f;
	m_fHeightScale = 0.0f;
}


TerrainClass::~TerrainClass(void)
{
}

BOOL TerrainClass::LoadTerrainFromFile(wchar_t *pRawFileName, wchar_t *pTextureFile)    //���ļ����ظ߶�ͼ������ĺ���  
{
	//���ļ���������
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,pTextureFile,&m_pTexture)))
	{
		return FALSE;
	}


	//���ļ����붥��߶�����
	ifstream inFile;
	inFile.open(pRawFileName,std::ios::binary);		//���ļ�
	inFile.seekg(0,std::ios::end);					//ָ���ļ�ĩβ

	vector<byte> inData(inFile.tellg());			//tellg()����ǰget ��ָ���λ�ã�����������ʾ�ļ���С

	inFile.seekg(0,std::ios::beg);
	inFile.read((char*)&inData[0],inData.size());		//��ȡ�ļ�
	inFile.close();

	m_vHeightInfo.resize(inData.size());
	for (unsigned int i=0;i<inData.size();i++)
	{
		m_vHeightInfo[i]=inData[i];
	}
	return TRUE;

}
BOOL TerrainClass::InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale)			//���γ�ʼ������  
{
	m_nCellsPerRow  = nRows;  //ÿ�еĵ�Ԫ����Ŀ
	m_nCellsPerCol  = nCols;  //ÿ�еĵ�Ԫ����Ŀ
	m_fCellSpacing  = fSpace;	//��Ԫ���ļ��
	m_fHeightScale  = fScale; //�߶�����ϵ��
	m_fTerrainWidth = nRows * fSpace;  //���εĿ��
	m_fTerrainDepth = nCols * fSpace;  //���ε����
	m_nVertsPerRow  = m_nCellsPerCol + 1;  //ÿ�еĶ�����
	m_nVertsPerCol  = m_nCellsPerRow + 1; //ÿ�еĶ�����
	m_nNumVertices  = m_nVertsPerRow * m_nVertsPerCol; 

	//�߶ȳ��Ը߶�����ϵ��
	for (unsigned int i=0;i<m_vHeightInfo.size();i++)	
	{
		m_vHeightInfo[i]*=m_fHeightScale;
	}

	//��1���������㻺��
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_nNumVertices * sizeof(TERRAINVERTEX),0,TERRAINVERTEX::FVF,D3DPOOL_MANAGED,&m_pVertexBuffer,0)))
	{
		return FALSE;
	}

	TERRAINVERTEX * pVertices = NULL;
	m_pVertexBuffer->Lock(0,0,(void**)&pVertices,0);		//����
	FLOAT fStartX = -m_fTerrainWidth / 2.0f, fEndX =  m_fTerrainWidth / 2.0f;		//ָ����ʼ��ͽ������X����ֵ
	FLOAT fStartZ =  m_fTerrainDepth / 2.0f, fEndZ = -m_fTerrainDepth / 2.0f;	//ָ����ʼ��ͽ������Z����ֵ
	FLOAT fCoordU = 1.0f / (FLOAT)m_nCellsPerRow;     //ָ������ĺ�����ֵ
	FLOAT fCoordV = 1.0f / (FLOAT)m_nCellsPerCol;		//ָ�������������ֵ

	int nIndex = 0, i = 0;
	for (float z = fStartZ; z > fEndZ; z -= m_fCellSpacing)		//Z���귽������ʼ���㵽���������м�ı���
	{
		int j = 0;
		for (float x = fStartX; x < fEndX; x += m_fCellSpacing)	//X���귽������ʼ���㵽���������м�ı���
		{
			nIndex = i * m_nCellsPerRow + j;		//ָ����ǰ�����ڶ��㻺���е�λ��
			pVertices[nIndex] = TERRAINVERTEX(x, m_vHeightInfo[nIndex], z, j*fCoordU, i*fCoordV); //�Ѷ���λ�������ڸ߶�ͼ�ж�Ӧ�ĸ�����������Լ��������긳ֵ��������ǰ�Ķ���
			j++;
		}
		i++;
	}

	m_pVertexBuffer->Unlock();			//����

	//��2��������������
	if (FAILED(m_pd3dDevice->CreateIndexBuffer(6*m_nNumVertices*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIndexBuffer,0)))
	{
		return FALSE;
	}
	WORD * pIndices =NULL;
	m_pIndexBuffer->Lock(0,0,(void**)&pIndices,0);		//����
	nIndex=0;
	for(int row = 0; row < m_nCellsPerRow-1; row++)   //����ÿ��  
	{  
		for(int col = 0; col < m_nCellsPerCol-1; col++)  //����ÿ��  
		{  
			//������ABC����������  
			pIndices[nIndex]   =  row * m_nCellsPerRow + col;           //����A  
			pIndices[nIndex+1] =  row * m_nCellsPerRow + col + 1;  //����B  
			pIndices[nIndex+2] = (row+1) * m_nCellsPerRow + col;    //����C  
			//������CBD����������  
			pIndices[nIndex+3] = (row+1) * m_nCellsPerRow + col;        //����C  
			pIndices[nIndex+4] =  row * m_nCellsPerRow + col + 1;       //����B  
			pIndices[nIndex+5] = (row+1) * m_nCellsPerRow + col + 1;//����D  
			//������һ����Ԫ����������6  
			nIndex += 6;  //�����Լ�6  
		}  
	}
	m_pIndexBuffer->Unlock();		//����

	return TRUE;
}
BOOL TerrainClass::RenderTerrain(D3DXMATRIX *pMatWorld, BOOL bDrawFrame)		//������Ⱦ����
{
	m_pd3dDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(TERRAINVERTEX));	//�Ѱ����ļ�������Ϣ�Ķ��㻺�����Ⱦ��ˮ�������  
	m_pd3dDevice->SetFVF(TERRAINVERTEX::FVF);									//���ö����ʽ FVF
	m_pd3dDevice->SetIndices(m_pIndexBuffer);									//������������
	m_pd3dDevice->SetTexture(0,m_pTexture);										//��������

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);   //�رձ�������

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);      //�رչ���  
	m_pd3dDevice->SetTransform(D3DTS_WORLD,pMatWorld); //�����������

	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nNumVertices,0,m_nNumVertices*2); //���ƶ��� 

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);   //������������
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);  //�򿪹���  
	m_pd3dDevice->SetTexture(0, 0); //�����ÿ�

	return	TRUE;

}
float TerrainClass::GetHeight(int Row,int Col)	//��ȡ�߶���Ϣ
{
	int nIndex=Row*m_nVertsPerRow+Col;
	return m_vHeightInfo[nIndex];
}
float TerrainClass::ComputeShade(int Row,int Col,D3DXVECTOR3 * Dlight)//���������ÿ��λ�õĹⰵ����
{
// 	D3DXVECTOR3 light=*Dlight;
// 	D3DXVec3Normalize(&light,&light);
// 	float Ha,Hb,Hc;
// 	Ha=GetHeight(Row,Col);
// 	Hb=GetHeight(Row,Col+1);
// 	Hc=GetHeight(Row+1,Col);
// 	//---A---B
// 	//---|
// 	//---C---
// 	D3DXVECTOR3 u(m_fCellSpacing,Ha-Hb,0.0f);
// 	D3DXVECTOR3 v(0.0f,Hc-Ha,-m_fCellSpacing);
// 
// 	D3DXVECTOR3 NA;
// 	D3DXVec3Cross(&NA,&u,&v);
// 	D3DXVec3Normalize(&NA,&NA);
// 
// 	float cosine=D3DXVec3Dot(&NA,&light);	//�Ƕȵ�cosֵ����������
// 	if(cosine<0)
// 		cosine=0;
// 	return cosine;
	return 0.0f;
}

bool TerrainClass::LightTerrain(D3DXVECTOR3 * Dlight)		//������ÿ�������ⰵ����
{
// 	D3DSURFACE_DESC  suf;
// 	m_pTexture->GetLevelDesc(0,&suf);	//��ȡ����ͼƬ��Ϣ
// 	if (suf.Format!=D3DFMT_X8R8G8B8)
// 	{
// 		return false;
// 	}
// 	D3DLOCKED_RECT Lrec;
// 
// 	m_pTexture->LockRect(0,&Lrec,0,0);	//����ͼ����
// 
// 	DWORD * imageData = (DWORD*)Lrec.pBits;	//ȡ����������
// 
// 	for (INT i=0;i<suf.Height;i++)
// 	{
// 		for (INT j=0;j<suf.Width;j++)
// 		{
// 			int nIdex=i*Lrec.Pitch / 4 +j;
// 
// 			D3DXCOLOR c(imageData[nIdex]);
// 			//
// 			c*=ComputeShade(i,j,Dlight);
// 			//����
// 			imageData[nIdex]=(D3DCOLOR)c;
// 		}
// 	}
// 	m_pTexture->UnlockRect(0);
	return true;
}
float TerrainClass::ClassGetMapHeight(float x,float z)
{
//	Translate on xz-plane by the transformation that takes
//		the terrain START point to the origin.
	x = ((float)m_fTerrainWidth / 2.0f) + x;
	z = ((float)m_fTerrainDepth / 2.0f) - z;

	x = (float)m_fCellSpacing;
	z = (float)m_fCellSpacing;

//	ȡ��
	int X = (int)floorf(x);
	int Z = (int)floorf(z);
//	A   B
//	---
//
//	---  
//	C   D

		if (X>m_nCellsPerRow)
		{
			X=m_nCellsPerRow;
		}
		else if(X<0)
		{
			X=0;
		}
		if (Z>m_nCellsPerCol)
		{
			Z=m_nCellsPerCol;
		}
		else if(Z<0)
		{
			Z=0;
		}

		float A,B,C,D;

		if ((X==m_nCellsPerRow)||(Z==m_nCellsPerCol))
		{
			A=GetHeight(X,Z);
			B=GetHeight(X,Z);
			C=GetHeight(X,Z);
			D=GetHeight(X,Z);
		}
		else
		{
			A=GetHeight(X,Z);
			B=GetHeight(X,Z+1);
			C=GetHeight(X+1,Z);
			D=GetHeight(X+1,Z+1);
		}
//		Find the triangle we are in
		float dx = x - X;
		float dz = z - Z;
		float height = 0.0f;
		if(dz<1.0f - dx)	//  upper triangle ABC
		{
			float uy = B - A; // A-B
			float vy = C - A; // A-C

//				Linearly interpolate on each vector.  The height is the vertex
//				height the vectors u and v originate from {A}, plus the heights
//				found by interpolating on each vector u and v.
				height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
		}
		else	//lower triangle DCB
		{
			float uy = C - D;  // D-C
			float vy = B - D;  // D-B

//				Linearly interpolate on each vector.  The height is the vertex
//				height the vectors u and v originate from {D}, plus the heights
//				found by interpolating on each vector u and v.
			height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
		}
		return height;
}


float Lerp(float x,float y,float s)
{
	return x+s*(y-x);
}