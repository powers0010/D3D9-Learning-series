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
	void CalculateViewMatrix(D3DXMATRIX * matView);	//计算并获取取景变换矩阵

	//三个Get系列函数  
	void GetProjMatrix(D3DXMATRIX * pmatProj)		//获取投影变换矩阵
	{
		*pmatProj=m_matProj;
	}
	void GetCameraPosition(D3DXVECTOR3 * pCameraPosition)//获取摄像机位置向量
	{
		*pCameraPosition=m_vCameraPosition;
	}
	void GetLookVector(D3DXVECTOR3 * pLookVector)//获取当前观察向量
	{
		*pLookVector=m_vLookVector;
	}

	//Set函数
	void SetCameraPosition(D3DXVECTOR3 *  CameraPosition=NULL);	//设置摄像机位置
	void SetTargetPosition(D3DXVECTOR3 *  TargetPosition=NULL);	//设置观察位置坐标
	void SetProjMatrix(D3DXMATRIX * matProj=NULL);	//设置投影变换矩阵
	void SetViewMatrix(D3DXMATRIX * matView=NULL);	//设置取景变换矩阵

	//三轴平移及旋转
	void MoveRight(float fUnits);
	void MoveUp(float fUnits);
	void MoveLook(float fUnits);
	void MoveLookZ(float fUnits);	//沿着水平分量平移

	void RotationRight(float fAngle);
	void RotationUp(float fAngle);
	void RotationUpY(float fAngle);	//沿着UP方向垂直分量旋转
	void RotationLook(float fAngle);

private:
	D3DXVECTOR3		m_vRightVector;		//创建右分量（单位向量）
	D3DXVECTOR3		m_vUpVector;		//创建上分量（单位向量）
	D3DXVECTOR3		m_vLookVector;		//创建观察分量（单位向量）
	D3DXVECTOR3		m_vCameraPosition;	//摄像机位置
	D3DXVECTOR3		m_vTargetPosition;		//观察目标位置

	D3DXMATRIX		m_matView;			//取景变换矩阵
	D3DXMATRIX		m_matProj;			//投影变换矩阵
	LPDIRECT3DDEVICE9	m_pd3dDevice;	//Direct3D设备对象 
};

#endif
