#pragma once
#include "D3DUnit.h"


//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: 继承自DXDXFRAME结构的结构
//-----------------------------------------------------------------------------
// typedef struct _D3DXFRAME
// {
// 	LPSTR                   Name;					//骨骼名称
// 	D3DXMATRIX              TransformationMatrix;	//骨骼相对于父节点的坐标变换矩阵
// 	LPD3DXMESHCONTAINER     pMeshContainer;			//指向本骨骼对应的网格容器
// 	struct _D3DXFRAME       *pFrameSibling;			//指向下一个兄弟数据
// 	struct _D3DXFRAME       *pFrameFirstChild;		//指向第一个儿子数据
// } D3DXFRAME, *LPD3DXFRAME;
struct D3DXFRAME_DERIVED: public D3DXFRAME			//存储骨骼信息
{
	D3DXMATRIXA16 CombinedTransformationMatrix;		//
};


//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: 继承自D3DXMESHCONTAINER结构的结构
// typedef struct _D3DXMESHCONTAINER				//
// {
// 	LPSTR                   Name;					//网格名称
// 	D3DXMESHDATA            MeshData;				//网格数据类型ID3DXMesh or ID3DXPatchMesh.
// 	LPD3DXMATERIAL          pMaterials;				//材质数组
// 	LPD3DXEFFECTINSTANCE    pEffects;				//
// 	DWORD                   NumMaterials;			//材质数量
// 	DWORD                  *pAdjacency;				//邻接信息
// 	LPD3DXSKININFO          pSkinInfo;				//蒙皮信息  ID3DXSkinInfo
// 	struct _D3DXMESHCONTAINER *pNextMeshContainer;
// } D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER	//存储蒙皮网格信息
{
	LPDIRECT3DTEXTURE9*  ppTextures;            //纹理数组
	LPD3DXMESH           pOrigMesh;             //原始网格
	LPD3DXATTRIBUTERANGE pAttributeTable;		//属性表数组
	DWORD                NumAttributeGroups;    //属性组数量,即子网格数量
	DWORD                NumInfl;               //每个顶点最多受多少骨骼的影响
	LPD3DXBUFFER         pBoneCombinationBuf;   //骨骼结合表
	D3DXMATRIX**         ppBoneMatrixPtrs;      //存放骨骼的组合变换矩阵
	D3DXMATRIX*          pBoneOffsetMatrices;   //存放骨骼的初始变换矩阵
	DWORD                NumPaletteEntries;     //骨骼数量上限
	bool                 UseSoftwareVP;         //标识是否使用软件顶点处理
};


//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: 来自微软官方DirectX SDK Samples中的骨骼动画类，这个类用来从.X文件加载框架层次和网格模型数据
// 核心点:      #define STDMETHOD(method) virtual HRESULT STDMETHODCALLTYPE method 
//-----------------------------------------------------------------------------
// DECLARE_INTERFACE(ID3DXAllocateHierarchy)
// {
//  	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,	//请求创建一个Frame对象
// 		LPD3DXFRAME *ppNewFrame) PURE;
// 
// 	 	STDMETHOD(CreateMeshContainer)(				//请求创建一个MeshContainer对象
// 		THIS_LPCSTR Name,							//网格的名称
// 		CONST D3DXMESHDATA *pMeshData,				//网格属性
// 		CONST D3DXMATERIAL *pMaterials,				//材质
// 		CONST D3DXEFFECTINSTANCE *pEffectInstances, //D3DXEFFECTINSTANCE
// 		DWORD NumMaterials,							//材质数量
// 		CONST DWORD *pAdjacency,					//邻接信息
// 		LPD3DXSKININFO pSkinInfo,					//蒙皮信息
// 		LPD3DXMESHCONTAINER *ppNewMeshContainer) PURE;

// 		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) PURE; //释放一个Frame对象
// 
// 		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) PURE; //释放一个MeshContainer对象
// };
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);	//重载CreateFrame函数
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name,		//创建蒙皮网格容器，以加载蒙皮信息
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD                     NumMaterials, 
		CONST DWORD *             pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);    
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};




//-----------------------------------------------------------------------------
// Desc: 来自微软官方DirectX SDK Samples中的骨骼动画全局函数
//-----------------------------------------------------------------------------
void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame );//绘制骨骼
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );//绘制蒙皮容器中的蒙皮网格
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot );		//设置好各级框架的组合变换矩阵
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );	//更新框架中的变换矩阵


