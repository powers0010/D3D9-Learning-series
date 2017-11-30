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

BOOL TerrainClass::LoadTerrainFromFile(wchar_t *pRawFileName, wchar_t *pTextureFile)    //从文件加载高度图和纹理的函数  
{
	//从文件创建纹理
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,pTextureFile,&m_pTexture)))
	{
		return FALSE;
	}


	//从文件导入顶点高度数据
	ifstream inFile;
	inFile.open(pRawFileName,std::ios::binary);		//打开文件
	inFile.seekg(0,std::ios::end);					//指向文件末尾

	vector<byte> inData(inFile.tellg());			//tellg()代表当前get 流指针的位置，可以用来表示文件大小

	inFile.seekg(0,std::ios::beg);
	inFile.read((char*)&inData[0],inData.size());		//读取文件
	inFile.close();

	m_vHeightInfo.resize(inData.size());
	for (unsigned int i=0;i<inData.size();i++)
	{
		m_vHeightInfo[i]=inData[i];
	}
	return TRUE;

}
BOOL TerrainClass::InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale)			//地形初始化函数  
{
	m_nCellsPerRow  = nRows;  //每行的单元格数目
	m_nCellsPerCol  = nCols;  //每列的单元格数目
	m_fCellSpacing  = fSpace;	//单元格间的间距
	m_fHeightScale  = fScale; //高度缩放系数
	m_fTerrainWidth = nRows * fSpace;  //地形的宽度
	m_fTerrainDepth = nCols * fSpace;  //地形的深度
	m_nVertsPerRow  = m_nCellsPerCol + 1;  //每行的顶点数
	m_nVertsPerCol  = m_nCellsPerRow + 1; //每列的顶点数
	m_nNumVertices  = m_nVertsPerRow * m_nVertsPerCol; 

	//高度乘以高度缩放系数
	for (unsigned int i=0;i<m_vHeightInfo.size();i++)	
	{
		m_vHeightInfo[i]*=m_fHeightScale;
	}

	//【1】创建顶点缓存
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_nNumVertices * sizeof(TERRAINVERTEX),0,TERRAINVERTEX::FVF,D3DPOOL_MANAGED,&m_pVertexBuffer,0)))
	{
		return FALSE;
	}

	TERRAINVERTEX * pVertices = NULL;
	m_pVertexBuffer->Lock(0,0,(void**)&pVertices,0);		//加锁
	FLOAT fStartX = -m_fTerrainWidth / 2.0f, fEndX =  m_fTerrainWidth / 2.0f;		//指定起始点和结束点的X坐标值
	FLOAT fStartZ =  m_fTerrainDepth / 2.0f, fEndZ = -m_fTerrainDepth / 2.0f;	//指定起始点和结束点的Z坐标值
	FLOAT fCoordU = 1.0f / (FLOAT)m_nCellsPerRow;     //指定纹理的横坐标值
	FLOAT fCoordV = 1.0f / (FLOAT)m_nCellsPerCol;		//指定纹理的纵坐标值

	int nIndex = 0, i = 0;
	for (float z = fStartZ; z > fEndZ; z -= m_fCellSpacing)		//Z坐标方向上起始顶点到结束顶点行间的遍历
	{
		int j = 0;
		for (float x = fStartX; x < fEndX; x += m_fCellSpacing)	//X坐标方向上起始顶点到结束顶点行间的遍历
		{
			nIndex = i * m_nCellsPerRow + j;		//指定当前顶点在顶点缓存中的位置
			pVertices[nIndex] = TERRAINVERTEX(x, m_vHeightInfo[nIndex], z, j*fCoordU, i*fCoordV); //把顶点位置索引在高度图中对应的各个顶点参数以及纹理坐标赋值给赋给当前的顶点
			j++;
		}
		i++;
	}

	m_pVertexBuffer->Unlock();			//解锁

	//【2】创建索引缓存
	if (FAILED(m_pd3dDevice->CreateIndexBuffer(6*m_nNumVertices*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIndexBuffer,0)))
	{
		return FALSE;
	}
	WORD * pIndices =NULL;
	m_pIndexBuffer->Lock(0,0,(void**)&pIndices,0);		//加锁
	nIndex=0;
	for(int row = 0; row < m_nCellsPerRow-1; row++)   //遍历每行  
	{  
		for(int col = 0; col < m_nCellsPerCol-1; col++)  //遍历每列  
		{  
			//三角形ABC的三个顶点  
			pIndices[nIndex]   =  row * m_nCellsPerRow + col;           //顶点A  
			pIndices[nIndex+1] =  row * m_nCellsPerRow + col + 1;  //顶点B  
			pIndices[nIndex+2] = (row+1) * m_nCellsPerRow + col;    //顶点C  
			//三角形CBD的三个顶点  
			pIndices[nIndex+3] = (row+1) * m_nCellsPerRow + col;        //顶点C  
			pIndices[nIndex+4] =  row * m_nCellsPerRow + col + 1;       //顶点B  
			pIndices[nIndex+5] = (row+1) * m_nCellsPerRow + col + 1;//顶点D  
			//处理完一个单元格，索引加上6  
			nIndex += 6;  //索引自加6  
		}  
	}
	m_pIndexBuffer->Unlock();		//解锁

	return TRUE;
}
BOOL TerrainClass::RenderTerrain(D3DXMATRIX *pMatWorld, BOOL bDrawFrame)		//地形渲染函数
{
	m_pd3dDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(TERRAINVERTEX));	//把包含的几何体信息的顶点缓存和渲染流水线相关联  
	m_pd3dDevice->SetFVF(TERRAINVERTEX::FVF);									//设置顶点格式 FVF
	m_pd3dDevice->SetIndices(m_pIndexBuffer);									//设置索引缓存
	m_pd3dDevice->SetTexture(0,m_pTexture);										//设置纹理

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);   //关闭背面消隐

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);      //关闭光照  
	m_pd3dDevice->SetTransform(D3DTS_WORLD,pMatWorld); //设置世界矩阵

	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nNumVertices,0,m_nNumVertices*2); //绘制顶点 

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);   //开启背面消隐
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);  //打开光照  
	m_pd3dDevice->SetTexture(0, 0); //纹理置空

	return	TRUE;

}
float TerrainClass::GetHeight(int Row,int Col)	//获取高度信息
{
	int nIndex=Row*m_nVertsPerRow+Col;
	return m_vHeightInfo[nIndex];
}
float TerrainClass::ComputeShade(int Row,int Col,D3DXVECTOR3 * Dlight)//计算地形上每个位置的光暗因子
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
// 	float cosine=D3DXVec3Dot(&NA,&light);	//角度的cos值，明暗因子
// 	if(cosine<0)
// 		cosine=0;
// 	return cosine;
	return 0.0f;
}

bool TerrainClass::LightTerrain(D3DXVECTOR3 * Dlight)		//给地形每个点计算光暗因子
{
// 	D3DSURFACE_DESC  suf;
// 	m_pTexture->GetLevelDesc(0,&suf);	//获取纹理图片信息
// 	if (suf.Format!=D3DFMT_X8R8G8B8)
// 	{
// 		return false;
// 	}
// 	D3DLOCKED_RECT Lrec;
// 
// 	m_pTexture->LockRect(0,&Lrec,0,0);	//地形图加锁
// 
// 	DWORD * imageData = (DWORD*)Lrec.pBits;	//取出地形数据
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
// 			//保存
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

//	取整
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