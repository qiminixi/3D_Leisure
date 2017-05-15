#include "PreHeader.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "DebugHelper.h"

//Ĭ��ΪLandObject������ϵ����������ϵ�غ�
ROSECamera::ROSECamera()
{
	m_cameratype=ROSECamera::LandObject;
	m_vecPos=  D3DXVECTOR3(0,2,0);
	m_vecRight=D3DXVECTOR3(1,0,0);
	m_vecUp=   D3DXVECTOR3(0,1,0);
	m_vecLook= D3DXVECTOR3(1,0,1);
	D3DXMatrixIdentity(&m_mtrRotate);	
}

ROSECamera::~ROSECamera()
{
}

//��ʼ��
void ROSECamera::Init(CameraType ctype,
	ViewType vtype, 
	D3DXVECTOR3 *pVectPos,
	D3DXVECTOR3 *pVectLook,
	D3DXVECTOR3 *pVectUp)
{
	m_cameratype=ctype;
	m_viewtype=vtype;
	if(m_vecPos)   m_vecPos=   *pVectPos;
	if(m_vecUp)    m_vecUp=    *pVectUp;
	if(m_vecLook)  m_vecLook=  *pVectLook;
	m_fRadius=5.0f;
	m_fAngelR=0.0f;
	m_fAngelU=0.0f;
	m_fAngelL=0.0f;
	
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
}

//��ȡ�۲�任����
void ROSECamera::GetViewMatrix(D3DXMATRIX *pOut)
{
	//���¼����������������ѱ��ֹ淶����look��up������ָ���ģ�
	D3DXVec3Normalize(&m_vecLook,&m_vecLook);
	
	D3DXVec3Cross(&m_vecRight,&m_vecUp,&m_vecLook);//x=y X z
	D3DXVec3Normalize(&m_vecRight,&m_vecRight);
	
	D3DXVec3Cross(&m_vecUp,&m_vecLook,&m_vecRight);//y=z X x
	D3DXVec3Normalize(&m_vecUp,&m_vecUp);
	//---------------------------
	
	D3DXVECTOR3 *pPos=NULL;
	if(m_viewtype==ViewType_First) pPos=&m_vecPos;
	else if(m_viewtype==ViewType_Third) pPos=&m_posThirdView;
	//pPos=&m_vecPos;
	
	float x=-D3DXVec3Dot(&m_vecRight,pPos);
	float y=-D3DXVec3Dot(&m_vecUp,pPos);
	float z=-D3DXVec3Dot(&m_vecLook,pPos);
	
	(*pOut)(0,0)=m_vecRight.x; (*pOut)(0,1)=m_vecUp.x; (*pOut)(0,2)=m_vecLook.x; (*pOut)(0,3)=0.0f;
	(*pOut)(1,0)=m_vecRight.y; (*pOut)(1,1)=m_vecUp.y; (*pOut)(1,2)=m_vecLook.y; (*pOut)(1,3)=0.0f;
	(*pOut)(2,0)=m_vecRight.z; (*pOut)(2,1)=m_vecUp.z; (*pOut)(2,2)=m_vecLook.z; (*pOut)(2,3)=0.0f;
	(*pOut)(3,0)=x;            (*pOut)(3,1)=y;         (*pOut)(3,2)=z;           (*pOut)(3,3)=1.0f;
}

//�����ƶ�(RightLeft)
void ROSECamera::MoveRL(float units)
{
	if(m_cameratype==LandObject)
		m_vecPos+=D3DXVECTOR3(m_vecRight.x,0.0f,m_vecRight.z)*units;
	else m_vecPos+=m_vecRight*units;
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
}

//�����ƶ�(UpDown)
void ROSECamera::MoveUD(float units)
{
	if(m_cameratype==ROSECamera::Aircraft)
		m_vecPos.y+=units;//m_vecPos+=m_vecUp*units;
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
}

//ǰ���ƶ�(ForwardBackward)
void ROSECamera::MoveFB(float units)
{
	if(m_cameratype==LandObject)
		m_vecPos+=D3DXVECTOR3(m_vecLook.x,0.0f,m_vecLook.z)*units;
	else m_vecPos+=m_vecLook*units;
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
}

//����(Pitch)����right��
void ROSECamera::Pitch(float angle)
{
	D3DXMATRIX m;
	
	D3DXMatrixRotationAxis(&m,&m_vecRight,angle);
	D3DXVec3TransformNormal(&m_vecUp,&m_vecUp,&m);
	D3DXVec3TransformNormal(&m_vecLook,&m_vecLook,&m);
	//if(m_cameratype!=LandObject) m_mtrRotate*=m;//������ת����
	
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
	
}

//ɨ��(Yaw)��  ��up��
void ROSECamera::Yaw(float angle)
{
	D3DXMATRIX m;
	
	//if(m_viewtype==ViewType_First)
	{
		if(m_cameratype==LandObject)
			D3DXMatrixRotationY(&m,angle);//������������ɨ�ӵ�ʱ������y�ᡣ���縩����ɨ�ӣ���up��ʹ���
		else if(m_cameratype==Aircraft)
			D3DXMatrixRotationAxis(&m,&m_vecUp,angle);
	}//else
	//D3DXVec3TransformNormal(&m_vecRight,&m_vecRight,&m);
	D3DXVec3TransformNormal(&m_vecLook,&m_vecLook,&m);
	D3DXVec3TransformNormal(&m_vecUp,&m_vecUp,&m);
	m_mtrRotate*=m;//������ת����
	
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
}

//����(Roll)�� ��look��
void ROSECamera::Roll(float angle)
{
	D3DXMATRIX m;
	D3DXMatrixRotationAxis(&m,&m_vecLook,angle);
	D3DXVec3TransformNormal(&m_vecUp,&m_vecUp,&m);
	D3DXVec3TransformNormal(&m_vecRight,&m_vecRight,&m);
	m_mtrRotate*=m;//������ת����
}

void ROSECamera::SetPosY(float y)
{
	m_vecPos.y=y+m_fHeight;
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
}

void ROSECamera::GetPosition(D3DXVECTOR3 *pOut)
{
	*pOut=m_vecPos;
}

//��ʼ�����λ��
void ROSECamera::SetInitMousePos(int x,int y)
{
	m_ptPreMousePos.x=x;
	m_ptPreMousePos.y=y;
}

//����ƶ����ƶ��ӽ�
void ROSECamera::MouseMove(int x,int y)
{
	//����ƶ�����
	int dx=x-m_ptPreMousePos.x;
	int dy=y-m_ptPreMousePos.y;
	if(dy!=0) Pitch(dy*3.1415926f/180*0.1);//����ȡ����תΪx+ˮƽ���ң�y+��ֱ����
	if(dx!=0) Yaw(dx*3.1415926f/180*0.1);
	m_ptPreMousePos.x=x;
	m_ptPreMousePos.y=y;
	char buffer[1024];
	sprintf(buffer,"%d,%d,%f,%f\n",dx,dy,x*3.1415926f/180,-y*3.1415926f/180);
	OutputDebugStringA(buffer);
}