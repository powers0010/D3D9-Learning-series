#include "StdAfx.h"
#include "Camera.h"


// D3DXVECTOR3		m_vRightVector;		//�����ҷ�������λ������
// D3DXVECTOR3		m_vUpVector;		//�����Ϸ�������λ������
// D3DXVECTOR3		m_vLookVector;		//�����۲��������λ������
// D3DXVECTOR3		m_vCameraPosition;	//�����λ��
// D3DXVECTOR3		m_vTargetPosition;		//�۲�Ŀ��λ��
// 
// D3DXMATRIX		m_matView;			//ȡ���任����
// D3DXMATRIX		m_matProj;			//ͶӰ�任����
// LPDIRECT3DDEVICE9	m_pd3dDevice;	//Direct3D�豸���� 

CameraClass::CameraClass(IDirect3DDevice9*	m_Device)
{

	m_pd3dDevice	=	m_Device;
	m_vRightVector	=	D3DXVECTOR3(1.0f,0.0f,0.0f);	// Ĭ����������X�������غ�
	m_vUpVector		=	D3DXVECTOR3(0.0f,1.0f,0.0f);	// Ĭ����������Y�������غ�  
	m_vLookVector	=	D3DXVECTOR3(0.0f,0.0f,1.0f);	// Ĭ�Ϲ۲�������Z�������غ�  
	m_vCameraPosition	=	D3DXVECTOR3 (0.0f, 1500.0f, -1200.0f)/*(p1.x,p1.y+20,p1.z-50)*/;		// Ĭ�����������Ϊ(0.0f, 0.0f, -250.0f)  
	m_vTargetPosition	=	D3DXVECTOR3(0.0f, 1200.0f, 0.0f);//p1;		//Ĭ�Ϲ۲�Ŀ��λ��Ϊ(0.0f, 0.0f, 0.0f);  
}

CameraClass::~CameraClass(void)
{
	m_pd3dDevice->Release();
}
void CameraClass::CalculateViewMatrix(D3DXMATRIX * matView)		//���㲢��ȡȡ���任����
{
	//1 �淶�������������� ������ ������ �۲�����
	D3DXVec3Normalize(&m_vRightVector,&m_vRightVector);
	D3DXVec3Cross(&m_vUpVector,&m_vRightVector,&m_vLookVector);
	D3DXVec3Normalize(&m_vUpVector,&m_vUpVector);
	D3DXVec3Cross(&m_vLookVector,&m_vUpVector,&m_vRightVector);
	D3DXVec3Normalize(&m_vLookVector,&m_vLookVector);

	//2 ����ͶӰ�仯����
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
//Set����
void CameraClass::SetCameraPosition(D3DXVECTOR3 *  pCameraPosition)		//���������λ��
{
	if (pCameraPosition!=NULL)
	{
		m_vCameraPosition	=	*	pCameraPosition;
	}
	else 
		m_vCameraPosition	=	D3DXVECTOR3(0.0f,0.0f,-250.0f);
}
void CameraClass::SetTargetPosition(D3DXVECTOR3 *  TargetPosition)		//���ù۲�λ������
{
	if (TargetPosition!=0)
	{
		m_vTargetPosition	=	*  TargetPosition;
	}
	else
		m_vTargetPosition	=	D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vLookVector	=	m_vTargetPosition-m_vCameraPosition;	//�۲��λ�ü������λ�ã��õ��۲췽������ 

	//�������淶��m_vUpVector��m_vRightVector  
	D3DXVec3Normalize(&m_vLookVector,&m_vLookVector);
	D3DXVec3Cross(&m_vRightVector,&m_vLookVector,&m_vUpVector);
	D3DXVec3Normalize(&m_vRightVector,&m_vRightVector);
	D3DXVec3Cross(&m_vUpVector,&m_vRightVector,&m_vLookVector);
	D3DXVec3Normalize(&m_vUpVector,&m_vUpVector);
}
void CameraClass::SetProjMatrix(D3DXMATRIX * matProj)		//����ͶӰ�任����
{
	if (matProj!=NULL)
	{
		m_matProj	=	* matProj;
	}
	else
		D3DXMatrixPerspectiveFovLH(&m_matProj,0.25f*D3DX_PI,(float)(WINDOW_WIDTH/WINDOW_HEIGHT),1.0F,30000.0F);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION,&m_matProj);

}
void CameraClass::SetViewMatrix(D3DXMATRIX * matView)		//����ȡ���任����
{
	if (matView!=NULL)
	{
		m_matView	=	* matView;
	}
	else
		CalculateViewMatrix(&m_matView);
	//����ȡ���任���� ������������ֵ
	m_vRightVector	=	D3DXVECTOR3(m_matView._11,m_matView._21,m_matView._31);		
	m_vUpVector		=	D3DXVECTOR3(m_matView._12,m_matView._22,m_matView._32);		
	m_vLookVector	=	D3DXVECTOR3(m_matView._13,m_matView._23,m_matView._33);
}

//����ƽ�Ƽ���ת
void CameraClass::MoveRight(float fUnits)		//���������ƶ�
{
	m_vCameraPosition	+=	fUnits*m_vRightVector;
	m_vTargetPosition	+=	fUnits*m_vRightVector;
}
void CameraClass::MoveUp(float fUnits)			//���������ƶ�
{
	m_vCameraPosition	+=	fUnits*m_vUpVector;
	m_vTargetPosition	+=	fUnits*m_vUpVector;
}
void CameraClass::MoveLook(float fUnits)		//�ع۲������ƶ�
{
	m_vCameraPosition	+=	fUnits*m_vLookVector;
	m_vTargetPosition	+=	fUnits*m_vLookVector;
}
void CameraClass::MoveLookZ(float fUnits)					//��look����Zˮƽ�����ƶ�
{
	D3DXVECTOR3 lookZ=m_vLookVector;
	lookZ.y=0;
	m_vCameraPosition+=fUnits*lookZ;
}
void CameraClass::RotationRight(float fAngle)	//����������ת
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&m_vRightVector,fAngle);		//��������m_vRightVector��תfAngle���Ƕȵ�R����  
	D3DXVec3TransformCoord(&m_vUpVector,&m_vUpVector,&R);		//��m_vUpVector������m_vRightVector��תfAngle���Ƕ�  
	D3DXVec3TransformCoord(&m_vLookVector,&m_vLookVector,&R);	//��m_vLookVector������m_vRightVector��תfAngle���Ƕ�  
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//���¹۲������
}
void CameraClass::RotationUp(float fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&m_vUpVector,fAngle);		//��������m_vRightVector��תfAngle���Ƕȵ�R����  
	D3DXVec3TransformCoord(&m_vRightVector,&m_vRightVector,&R);		//��RightVector������m_vUpVectorm_v��תfAngle���Ƕ�  
	D3DXVec3TransformCoord(&m_vLookVector,&m_vLookVector,&R);	//��m_vLookVector������m_vUpVectorm_v��תfAngle���Ƕ�  
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//���¹۲������

}
void CameraClass::RotationUpY(float fAngle)	//����UP����ֱ������ת
{
	D3DXVECTOR3 lookY=m_vUpVector;
	lookY.x=0;
	lookY.z=0;
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&lookY,fAngle);	
	D3DXVec3TransformCoord(&m_vRightVector,&m_vRightVector,&R);
	D3DXVec3TransformCoord(&m_vLookVector,&m_vLookVector,&R);
	D3DXVec3TransformCoord(&m_vUpVector,&m_vUpVector,&R);
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//���¹۲������
}
void CameraClass::RotationLook(float fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R,&m_vLookVector,fAngle);		//��������m_vLookVector��תfAngle���Ƕȵ�R����  
	D3DXVec3TransformCoord(&m_vRightVector,&m_vRightVector,&R);		//��RightVector������m_vLookVector��תfAngle���Ƕ�  
	D3DXVec3TransformCoord(&m_vUpVector,&m_vUpVector,&R);	//��m_vUpVector������m_vLookVector��תfAngle���Ƕ�  
	m_vTargetPosition=m_vLookVector*D3DXVec3Length(&m_vTargetPosition);//���¹۲������

}
