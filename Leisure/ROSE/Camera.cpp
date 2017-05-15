#include "PreHeader.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "DebugHelper.h"

//默认为LandObject，坐标系与世界坐标系重合
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

//初始化
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

//获取观察变换矩阵
void ROSECamera::GetViewMatrix(D3DXMATRIX *pOut)
{
	//重新计算三个方向向量已保持规范化（look和up方向是指定的）
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

//左右移动(RightLeft)
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

//上下移动(UpDown)
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

//前后移动(ForwardBackward)
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

//俯仰(Pitch)，绕right轴
void ROSECamera::Pitch(float angle)
{
	D3DXMATRIX m;
	
	D3DXMatrixRotationAxis(&m,&m_vecRight,angle);
	D3DXVec3TransformNormal(&m_vecUp,&m_vecUp,&m);
	D3DXVec3TransformNormal(&m_vecLook,&m_vecLook,&m);
	//if(m_cameratype!=LandObject) m_mtrRotate*=m;//更新旋转矩阵
	
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
	
}

//扫视(Yaw)，  绕up轴
void ROSECamera::Yaw(float angle)
{
	D3DXMATRIX m;
	
	//if(m_viewtype==ViewType_First)
	{
		if(m_cameratype==LandObject)
			D3DXMatrixRotationY(&m,angle);//对于人来讲，扫视的时候是绕y轴。比如俯视着扫视，绕up轴就错了
		else if(m_cameratype==Aircraft)
			D3DXMatrixRotationAxis(&m,&m_vecUp,angle);
	}//else
	//D3DXVec3TransformNormal(&m_vecRight,&m_vecRight,&m);
	D3DXVec3TransformNormal(&m_vecLook,&m_vecLook,&m);
	D3DXVec3TransformNormal(&m_vecUp,&m_vecUp,&m);
	m_mtrRotate*=m;//更新旋转矩阵
	
	if(m_viewtype==ViewType_Third)
	{
		m_posThirdView.x=m_vecPos.x-m_vecLook.x*m_fRadius;
		m_posThirdView.y=m_vecPos.y-m_vecLook.y*m_fRadius;
		m_posThirdView.z=m_vecPos.z-m_vecLook.z*m_fRadius;
	}
}

//翻滚(Roll)， 绕look轴
void ROSECamera::Roll(float angle)
{
	D3DXMATRIX m;
	D3DXMatrixRotationAxis(&m,&m_vecLook,angle);
	D3DXVec3TransformNormal(&m_vecUp,&m_vecUp,&m);
	D3DXVec3TransformNormal(&m_vecRight,&m_vecRight,&m);
	m_mtrRotate*=m;//更新旋转矩阵
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

//初始化鼠标位置
void ROSECamera::SetInitMousePos(int x,int y)
{
	m_ptPreMousePos.x=x;
	m_ptPreMousePos.y=y;
}

//鼠标移动，移动视角
void ROSECamera::MouseMove(int x,int y)
{
	//获得移动向量
	int dx=x-m_ptPreMousePos.x;
	int dy=y-m_ptPreMousePos.y;
	if(dy!=0) Pitch(dy*3.1415926f/180*0.1);//这里取反，转为x+水平向右，y+竖直向上
	if(dx!=0) Yaw(dx*3.1415926f/180*0.1);
	m_ptPreMousePos.x=x;
	m_ptPreMousePos.y=y;
	char buffer[1024];
	sprintf(buffer,"%d,%d,%f,%f\n",dx,dy,x*3.1415926f/180,-y*3.1415926f/180);
	OutputDebugStringA(buffer);
}