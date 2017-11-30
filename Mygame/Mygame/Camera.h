#pragma once

#include "D3DUnit.h"
#include "People.h"

#ifndef CAMERACLASS_H
#define CAMERACLASS_H

class CameraClass
{
public:
	CameraClass(IDirect3DDevice9*	m_Device);
	virtual ~CameraClass(void);
public:
	void CalculateViewMatrix(D3DXMATRIX * matView);	//���㲢��ȡȡ���任����

	//����Getϵ�к���  
	void GetProjMatrix(D3DXMATRIX * pmatProj)		//��ȡͶӰ�任����
	{
		*pmatProj=m_matProj;
	}
	void GetCameraPosition(D3DXVECTOR3 * pCameraPosition)//��ȡ�����λ������
	{
		*pCameraPosition=m_vCameraPosition;
	}
	void GetLookVector(D3DXVECTOR3 * pLookVector)//��ȡ��ǰ�۲�����
	{
		*pLookVector=m_vLookVector;
	}

	//Set����
	void SetCameraPosition(D3DXVECTOR3 *  CameraPosition=NULL);	//���������λ��
	void SetTargetPosition(D3DXVECTOR3 *  TargetPosition=NULL);	//���ù۲�λ������
	void SetProjMatrix(D3DXMATRIX * matProj=NULL);	//����ͶӰ�任����
	void SetViewMatrix(D3DXMATRIX * matView=NULL);	//����ȡ���任����

	//����ƽ�Ƽ���ת
	void MoveRight(float fUnits);
	void MoveUp(float fUnits);
	void MoveLook(float fUnits);
	void MoveLookZ(float fUnits);	//����ˮƽ����ƽ��

	void RotationRight(float fAngle);
	void RotationUp(float fAngle);
	void RotationUpY(float fAngle);	//����UP����ֱ������ת
	void RotationLook(float fAngle);

private:
	D3DXVECTOR3		m_vRightVector;		//�����ҷ�������λ������
	D3DXVECTOR3		m_vUpVector;		//�����Ϸ�������λ������
	D3DXVECTOR3		m_vLookVector;		//�����۲��������λ������
	D3DXVECTOR3		m_vCameraPosition;	//�����λ��
	D3DXVECTOR3		m_vTargetPosition;		//�۲�Ŀ��λ��

	D3DXMATRIX		m_matView;			//ȡ���任����
	D3DXMATRIX		m_matProj;			//ͶӰ�任����
	LPDIRECT3DDEVICE9	m_pd3dDevice;	//Direct3D�豸���� 
};

#endif
