#include "StdAfx.h"
#include "Camera.h"


// D3DXVECTOR3		m_vRightVector;		//创建右分量（单位向量）
// D3DXVECTOR3		m_vUpVector;		//创建上分量（单位向量）
// D3DXVECTOR3		m_vLookVector;		//创建观察分量（单位向量）
// D3DXVECTOR3		m_vCameraPosition;	//摄像机位置
// D3DXVECTOR3		m_vTargetPosition;		//观察目标位置
// 
// D3DXMATRIX		m_matView;			//取景变换矩阵
// D3DXMATRIX		m_matProj;			//投影变换矩阵
// LPDIRECT3DDEVICE9	m_pd3dDevice;	//Direct3D设备对象 

CameraClass::CameraClass(IDirect3DDevice9*	m_Device)
{

	m_pd3dDevice	=	m_Device;
	m_vRightVector	=	D3DXVECTOR3(1.0f,0.0f,0.0f);	// 默认右向量与X正半轴重合
	m_vUpVector		=	D3DXVECTOR3(0.0f,1.0f,0.0f);	// 默认上向量与Y正半轴重合  
	m_vLookVector	=	D3DXVECTOR3(0.0f,0.0f,1.0f);	// 默认观察向量与Z正半轴重合  
	m_vCameraPosition	=	D3DXVECTOR3 (0.0f, 1500.0f, -1200.0f)/*(p1.x,p1.y+20,p1.z-50)*/;		// 默认摄像机坐标为(0.0f, 0.0f, -250.0f)  
	m_vTargetPosition	=	D3DXVECTOR3(0.0f, 1200.0f, 0.0f);//p1;		//默认观察目标位置为(0.0f, 0.0f, 0.0f);  
}

CameraClass::~CameraClass(void)
{
	m_pd3dDevice->Release();
}
void CameraClass::CalculateViewMatrix(D3DXMATRIX * matView)		//计算并获取取景变换矩阵
{
	//1 规范化正交三个向量 右向量 上向量 观察向量
	D3DXVec3Normalize(&m_vRightVector,&m_vRightVector);
	D3DXVec3Cross(&m_vUpVector,&m_vRightVector,&m_vLookVector);
	D3DXVec3Normalize(&m_vUpVector,&m_vUpVector);
	D3DXVec3Cross(&m_vLookVector,&m_vUpVector,&m_vRightVector);
	D3DXVec3Normalize(&m_vLookVector,&m_vLookVector);

	//2 计算投影变化矩阵
	matView->_11	=	m_vRightVector.x;
	matView->_12	=	m_vUpVector.x;
	matView->_13	=	m_vLookVector.x;
	matView->_14	=	0.0f;
	matView->_21	=	m_vRightVector.y;
	matView->_22	=	m_vUpVector.y;
	matView->_23	=	m_vLookVector.y;
	matView->_24	=	0.0f;
	matView->_31	=	m_vRightVector.z;
	matView->_32	=	m_vUpVector.z;
	matView->_33	=	m_vLookVector.z;
	matView->_34	=	0.0f;
	matView->_41	=	-D3DXVec3Dot(&m_vCameraPosition,&m_vRightVector);
	matView->_42	=	-D3DXVec3Dot(&m_vCameraPosition,&m_vUpVector);
	matView->_43	=	-D3DXVec3Dot(&m_vCameraPosition,&m_vLookVector);
	matView->_44	=	1.0f;

}
//Set函数
void CameraClass::SetCameraPosition(D3DXVECTOR3 *  pCameraPosition)		//设置摄像机位置
{
	if (pCameraPosition!=NULL)
	{
		m_vCameraPosition	=	*	pCameraPosition;
	}
	else 
		m_vCameraPosition	=	D3DXVECTOR3(0.0f,0.0f,-250.0f);
}
void CameraClass::SetTargetPosition(D3DXVECTOR3 *  TargetPosition)		//设置观察位置坐标
{
	if (TargetPosition!=0)
	{
		m_vTargetPosition	=	*  TargetPosition;
	}
	else
		m_vTargetPosition	=	D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vLookVector	=	m_vTargetPosition-m_vCameraPosition;	//观察点位置减摄像机位置，得到观察方向向量 

	//正交并规范化m_vUpVector和m_vRightVector  
	D3DXVec3Normalize(&m_vLookVector,&m_vLookVector);
	D3DXVec3Cross(&m_vRightVector,&m_vLookVector,&m_vUpVector);
	D3DXVec3Normalize(&m_vRightVector,&m_vRightVector);
	D3DXVec3Cross(&m_vUpVector,&m_vRightVector,&m_vLookVector);
	D3DXVec3Normalize(&m_vUpVector,&m_vUpVector);
}
void CameraClass::SetProjMatrix(D3DXMATRIX * matProj)		//设置投影变换矩阵
{
	if (matProj!=NULL)
	{
		m_matProj	=	* matProj;
	}
	else
		D3DXMatrixPerspectiveFovLH(&m_matProj,0.25f*D3DX_PI,(float)(WINDOW_WIDTH/WINDOW_HEIGHT),1.0F,30000.0F);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION,&m_matProj);

}
void CameraClass::SetViewMatrix(D3DXMATRIX * matView)		//设置取景变换矩阵
{
	if (matView!=NULL)
	{
		m_matView	=	* matView;
	}
	else
		CalculateViewMatrix(&m_matView);
	//根据取景变换矩阵 给三大向量赋值
	m_vRightVector	=	D3DXVECTOR3(m_matView._11,m_matView._21,m_matView._31);		
	m_vUpVector		=	D3DXVECTOR3(m_matView._12,m_matView._22,m_matView._32);		
	m_vLookVector	=	D3DXVECTOR3(m_matView._13,m_matView._23,m_matView._33);
}

//三轴平移及旋转
void CameraClass::MoveRight(float fUnits)		//沿右向量移动
{
	m_vCameraPosition	+=	fUnits*m_vRightVector;
	m_vTargetPosition	+=	fUnits*m_vRightVector;
}
void CameraClass::MoveUp(float fUnits)			//沿上向量移动
{
	m_vCameraPosition	+=	fUnits*m_vUpVector;
	m_vTargetPosition	+=	fUnits*m_vUpVector;
}
void CameraClass::MoveLook(float fUnits)		//沿观察向量移动
{
	m_vCameraPosition	+=	fUnits*m_vLookVector;
	m_vTargetPosition	+=	fUnits*m_vLookVector;
}
void CameraClass::MoveLookZ(float fUnits)					//沿look方向Z水平方向移动
{
	D3DXVECTOR3 lookZ=m_vLookVector;
	lookZ.y=0;
	m_vCameraPosition+=fUnits*lookZ;
}
void CameraClass::RotationRight(float fAngle)	//沿右向量旋转
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&m_vRightVector,fAngle);		//创建出绕m_vRightVector旋转fAngle个角度的R矩阵  
	D3DXVec3TransformCoord(&m_vUpVector,&m_vUpVector,&R);		//让m_vUpVector向量绕m_vRightVector旋转fAngle个角度  
	D3DXVec3TransformCoord(&m_vLookVector,&m_vLookVector,&R);	//让m_vLookVector向量绕m_vRightVector旋转fAngle个角度  
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//更新观察点坐标
}
void CameraClass::RotationUp(float fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&m_vUpVector,fAngle);		//创建出绕m_vRightVector旋转fAngle个角度的R矩阵  
	D3DXVec3TransformCoord(&m_vRightVector,&m_vRightVector,&R);		//让RightVector向量绕m_vUpVectorm_v旋转fAngle个角度  
	D3DXVec3TransformCoord(&m_vLookVector,&m_vLookVector,&R);	//让m_vLookVector向量绕m_vUpVectorm_v旋转fAngle个角度  
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//更新观察点坐标

}
void CameraClass::RotationUpY(float fAngle)	//沿着UP方向垂直分量旋转
{
	D3DXVECTOR3 lookY=m_vUpVector;
	lookY.x=0;
	lookY.z=0;
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&lookY,fAngle);	
	D3DXVec3TransformCoord(&m_vRightVector,&m_vRightVector,&R);
	D3DXVec3TransformCoord(&m_vLookVector,&m_vLookVector,&R);
	D3DXVec3TransformCoord(&m_vUpVector,&m_vUpVector,&R);
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//更新观察点坐标
}
void CameraClass::RotationLook(float fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&m_vLookVector,fAngle);		//创建出绕m_vLookVector旋转fAngle个角度的R矩阵  
	D3DXVec3TransformCoord(&m_vRightVector,&m_vRightVector,&R);		//让RightVector向量绕m_vLookVector旋转fAngle个角度  
	D3DXVec3TransformCoord(&m_vUpVector,&m_vUpVector,&R);	//让m_vUpVector向量绕m_vLookVector旋转fAngle个角度  
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//更新观察点坐标

}
