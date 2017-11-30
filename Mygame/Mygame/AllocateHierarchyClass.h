#pragma once
#include "D3DUnit.h"


//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: �̳���DXDXFRAME�ṹ�Ľṹ
//-----------------------------------------------------------------------------
// typedef struct _D3DXFRAME
// {
// 	LPSTR                   Name;					//��������
// 	D3DXMATRIX              TransformationMatrix;	//��������ڸ��ڵ������任����
// 	LPD3DXMESHCONTAINER     pMeshContainer;			//ָ�򱾹�����Ӧ����������
// 	struct _D3DXFRAME       *pFrameSibling;			//ָ����һ���ֵ�����
// 	struct _D3DXFRAME       *pFrameFirstChild;		//ָ���һ����������
// } D3DXFRAME, *LPD3DXFRAME;
struct D3DXFRAME_DERIVED: public D3DXFRAME			//�洢������Ϣ
{
	D3DXMATRIXA16 CombinedTransformationMatrix;		//
};


//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: �̳���D3DXMESHCONTAINER�ṹ�Ľṹ
// typedef struct _D3DXMESHCONTAINER				//
// {
// 	LPSTR                   Name;					//��������
// 	D3DXMESHDATA            MeshData;				//������������ID3DXMesh or ID3DXPatchMesh.
// 	LPD3DXMATERIAL          pMaterials;				//��������
// 	LPD3DXEFFECTINSTANCE    pEffects;				//
// 	DWORD                   NumMaterials;			//��������
// 	DWORD                  *pAdjacency;				//�ڽ���Ϣ
// 	LPD3DXSKININFO          pSkinInfo;				//��Ƥ��Ϣ  ID3DXSkinInfo
// 	struct _D3DXMESHCONTAINER *pNextMeshContainer;
// } D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER	//�洢��Ƥ������Ϣ
{
	LPDIRECT3DTEXTURE9*  ppTextures;            //��������
	LPD3DXMESH           pOrigMesh;             //ԭʼ����
	LPD3DXATTRIBUTERANGE pAttributeTable;		//���Ա�����
	DWORD                NumAttributeGroups;    //����������,������������
	DWORD                NumInfl;               //ÿ����������ܶ��ٹ�����Ӱ��
	LPD3DXBUFFER         pBoneCombinationBuf;   //������ϱ�
	D3DXMATRIX**         ppBoneMatrixPtrs;      //��Ź�������ϱ任����
	D3DXMATRIX*          pBoneOffsetMatrices;   //��Ź����ĳ�ʼ�任����
	DWORD                NumPaletteEntries;     //������������
	bool                 UseSoftwareVP;         //��ʶ�Ƿ�ʹ��������㴦��
};


//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: ����΢��ٷ�DirectX SDK Samples�еĹ��������࣬�����������.X�ļ����ؿ�ܲ�κ�����ģ������
// ���ĵ�:      #define STDMETHOD(method) virtual HRESULT STDMETHODCALLTYPE method 
//-----------------------------------------------------------------------------
// DECLARE_INTERFACE(ID3DXAllocateHierarchy)
// {
//  	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,	//���󴴽�һ��Frame����
// 		LPD3DXFRAME *ppNewFrame) PURE;
// 
// 	 	STDMETHOD(CreateMeshContainer)(				//���󴴽�һ��MeshContainer����
// 		THIS_LPCSTR Name,							//���������
// 		CONST D3DXMESHDATA *pMeshData,				//��������
// 		CONST D3DXMATERIAL *pMaterials,				//����
// 		CONST D3DXEFFECTINSTANCE *pEffectInstances, //D3DXEFFECTINSTANCE
// 		DWORD NumMaterials,							//��������
// 		CONST DWORD *pAdjacency,					//�ڽ���Ϣ
// 		LPD3DXSKININFO pSkinInfo,					//��Ƥ��Ϣ
// 		LPD3DXMESHCONTAINER *ppNewMeshContainer) PURE;

// 		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) PURE; //�ͷ�һ��Frame����
// 
// 		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) PURE; //�ͷ�һ��MeshContainer����
// };
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);	//����CreateFrame����
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name,		//������Ƥ�����������Լ�����Ƥ��Ϣ
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
// Desc: ����΢��ٷ�DirectX SDK Samples�еĹ�������ȫ�ֺ���
//-----------------------------------------------------------------------------
void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame );//���ƹ���
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );//������Ƥ�����е���Ƥ����
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot );		//���úø�����ܵ���ϱ任����
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );	//���¿���еı任����


