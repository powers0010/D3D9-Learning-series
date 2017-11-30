#include "StdAfx.h"
#include "People.h"



People::~People(void)
{

}

void People::CaculateMatrix(D3DXMATRIX * mat)		//计算人物世界坐标系
{
	D3DXMATRIX I1,m1,r1,sl1;
	D3DXMatrixTranslation(&m1, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationY(&r1,m_Volangle);
	D3DXMatrixScaling(&sl1,0.4f,0.4f,0.4f);
	D3DXMatrixIdentity(&I1);
	*mat = I1 * sl1 * r1  * m1;
}

//【人物移动】
void People::MoveLook(float ftime)
{
	m_position -= ftime*m_speed*m_direction;
}
void People::MoveLR(float ftime)
{
	D3DXVECTOR3 Vleft,Vup = D3DXVECTOR3(0.0f,1.0f,0.0f);
	D3DXVec3Cross(&Vleft,&Vup,&m_direction);
	D3DXVec3Normalize(&Vleft,&Vleft);
	m_position += ftime*Vleft;

}
//【人物旋转】
void People::Rotation(float fang)
{
	m_angle=fang;
	m_Volangle+=fang;
	D3DXVECTOR3 v1=D3DXVECTOR3(0.0f,0.0f,1.0f);
	D3DXMATRIX r1;
	D3DXMatrixRotationY(&r1,m_Volangle);
	D3DXVec3TransformCoord(&m_direction,&v1,&r1);
}
//----【人物血条初始化】
void People::InitHPMP(const D3DXVECTOR3* vec1 , wchar_t * TextureFile , wchar_t * HPFile)
{
	m_HPVec = *vec1;	//记录血条背景纹理坐标
	//创建血蓝条背景索引缓存
	m_pd3dDevice->CreateVertexBuffer(4*sizeof(HPDVERTEX),0,D3DFVF_HPVERTEX,D3DPOOL_MANAGED,&m_pHPVBuffer,0);
	HPDVERTEX *pVertices = NULL;
	m_pHPVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = HPDVERTEX(vec1->x,			vec1->y+100.0f,	vec1->z, 1.0f, 0.0f, 1.0f);
	pVertices[1] = HPDVERTEX(vec1->x,			vec1->y,		vec1->z, 1.0f, 0.0f, 0.0f); 
	pVertices[2] = HPDVERTEX(vec1->x+250.0f,	vec1->y+100.0f, vec1->z, 1.0f, 1.0f, 1.0f); 
	pVertices[3] = HPDVERTEX(vec1->x+250.0f,	vec1->y,		vec1->z, 1.0f, 1.0f, 0.0f);
	m_pHPVBuffer ->Unlock();

	//创建血蓝索引缓存
	m_pd3dDevice->CreateVertexBuffer(4*sizeof(HPDVERTEX),0,D3DFVF_HPVERTEX,D3DPOOL_MANAGED,&m_phpVertexBuf,0);
	pVertices = NULL;
	m_phpVertexBuf ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = HPDVERTEX(vec1->x+83.0f,		vec1->y+35.0f,	vec1->z, 1.0f, 0.0f, 1.0f);
	pVertices[1] = HPDVERTEX(vec1->x+83.0f,		vec1->y+22.0f,	vec1->z, 1.0f, 0.0f, 0.0f); 
	pVertices[2] = HPDVERTEX(vec1->x+83.0f+m_HP/MAX_HP*110.0f ,	vec1->y+35.0f,	vec1->z, 1.0f, 1.0f, 1.0f); 
	pVertices[3] = HPDVERTEX(vec1->x+83.0f+m_HP/MAX_HP*110.0f,	vec1->y+22.0f,	vec1->z, 1.0f, 1.0f, 0.0f);
	m_phpVertexBuf ->Unlock();
	//加载纹理
	D3DXCreateTextureFromFile(m_pd3dDevice,TextureFile,&m_pHPTexture);

	D3DXCreateTextureFromFile(m_pd3dDevice,HPFile,&m_phpTextureBuf);
}
//----【人物血条绘制】
void People::HPNPrender()
{
	//----【设置混合】
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   true );
 	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
 	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//【画血蓝条背景】
	m_pd3dDevice->SetTexture( 0, m_pHPTexture );
	m_pd3dDevice->SetStreamSource( 0, m_pHPVBuffer , 0, sizeof(HPDVERTEX) );
	m_pd3dDevice->SetFVF( D3DFVF_HPVERTEX );
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	//【血条】
	//【更新血条】
	HPDVERTEX *pVertices = NULL;
	m_phpVertexBuf ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = HPDVERTEX(m_HPVec.x+83.0f,		m_HPVec.y+35.0f,	m_HPVec.z, 1.0f, 0.0f, 1.0f);
	pVertices[1] = HPDVERTEX(m_HPVec.x+83.0f,		m_HPVec.y+22.0f,	m_HPVec.z, 1.0f, 0.0f, 0.0f); 
	pVertices[2] = HPDVERTEX(m_HPVec.x+83.0f+m_HP/MAX_HP*110.0f ,	m_HPVec.y+35.0f,	m_HPVec.z, 1.0f, 1.0f, 1.0f); 
	pVertices[3] = HPDVERTEX(m_HPVec.x+83.0f+m_HP/MAX_HP*110.0f,	m_HPVec.y+22.0f,	m_HPVec.z, 1.0f, 1.0f, 0.0f);
	m_phpVertexBuf ->Unlock();

	//【绘制血条】
	m_pd3dDevice->SetTexture( 0, m_phpTextureBuf );
	m_pd3dDevice->SetStreamSource( 0, m_phpVertexBuf , 0, sizeof(HPDVERTEX) );
	m_pd3dDevice->SetFVF( D3DFVF_HPVERTEX );
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	//----【关闭混合】
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   false );
}