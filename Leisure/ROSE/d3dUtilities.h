#ifndef D3DUTILITIES_H
#define D3DUTILITIES_H

#include "definitions.h"
#include "Camera.h"
#include "terrain.h"
#include "particle.h"
#include "scenes.h"
#include "text.h"
#include "DebugHelper.h"
#include "math.h"


/*
namespace d3du
{
class Ray
{
public:
	D3DXVECTOR3 m_posOrg;
	D3DXVECTOR3 m_vecDirection;
	
};

class Sphere
{
public:
	D3DXVECTOR3 m_posCenter;
	float m_fRadius;
};


struct Vertex_COLOR
{
	float m_x,m_y,m_z;
	D3DCOLOR m_color;
	static const DWORD m_FVF;
	Vertex_COLOR(float x,float y,float z,D3DCOLOR clr)
	{
		m_x=x;m_y=y;m_z=z;m_color=clr;
	}
};

const DWORD Vertex_COLOR::m_FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE;

}

d3du::Ray CalcPickingRay(IDirect3DDevice9 *pDevice,int x,int y)
{
	float px=0.0f;
	float py=0.0f;
	
	D3DVIEWPORT9 vp;
	pDevice->GetViewport(&vp);
	
	D3DXMATRIX proj;
	pDevice->GetTransform(D3DTS_PROJECTION,&proj);
	px=(((2.0f*x)/vp.Width)-1.0f) / proj(0,0);
	py=(((-2.0f*y)/vp.Height)+1.0f) / proj(1,1);
	
	d3du::Ray ray;
	ray.m_posOrg=D3DXVECTOR3(0.0f,0.0f,0.0f);
	ray.m_vecDirection=D3DXVECTOR3(px,py,1.0f);
	return ray;
}

void TransformRay(d3du::Ray *pRay,D3DXMATRIX *pM)
{
	D3DXVec3TransformCoord(&pRay->m_posOrg,&pRay->m_vecDirection,pM);
	D3DXVec3TransformNormal(&pRay->m_vecDirection,&pRay->m_vecDirection,pM);
	D3DXVec3Normalize(&pRay->m_vecDirection,&pRay->m_vecDirection);
}

bool RaySphereIntTest(d3du::Ray *pRay,d3du::Sphere *pSphere)
{
	D3DXVECTOR3 v=pRay->m_posOrg-pSphere->m_posCenter;
	float b=2.0f*D3DXVec3Dot(&pRay->m_vecDirection,&v);
	float c=D3DXVec3Dot(&v,&v)-(pSphere->m_fRadius*pSphere->m_fRadius);
	float discriminant=b*b-4.0f*c;
	if(discriminant < 0.0f)
		return false;
	
	discriminant=sqrtf(discriminant);
	float s0=(-b + discriminant) / 2.0f;
	float s1=(-b - discriminant) / 2.0f;
	
	if(s0>=0.0f || s1>=0.0f)
		return true;
	return false;
}
*/
/*void GetDisplayCardInfo(DWORD &dwNum,CString chCardName[])  
{  
	HKEY keyServ;  
	HKEY keyEnum;  
	HKEY key;  
	HKEY key2;  
	LONG lResult;//LONG�ͱ��������溯������ֵ  

	//��ѯ"SYSTEM\\CurrentControlSet\\Services"�µ������Ӽ����浽keyServ  
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("SYSTEM\\CurrentControlSet\\Services"),0,KEY_READ,&keyServ);  
	if (ERROR_SUCCESS != lResult)  
		return;  


	//��ѯ"SYSTEM\\CurrentControlSet\\Enum"�µ������Ӽ����浽keyEnum  
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("SYSTEM\\CurrentControlSet\\Enum"),0,KEY_READ,&keyEnum);  
	if (ERROR_SUCCESS != lResult)  
		return;  

	int i = 0,count = 0;  
	DWORD size = 0,type = 0;  
	for (;;++i)  
	{  
		Sleep(5);  
		size = 512;  
		TCHAR name[512] = {0};//����keyServ�¸�������ֶ�����  

		//���ö��keyServ�µĸ������ֶα��浽name��  
		lResult = RegEnumKeyEx(keyServ,i,name,&size,NULL,NULL,NULL,NULL);  

		//Ҫ��ȡ��������ڣ���keyServ������ȫ��������ʱ����ѭ��  
		if(lResult == ERROR_NO_MORE_ITEMS)  
			break;  

		//��keyServ�������ֶ�Ϊname����ʶ���ֶε�ֵ���浽key  
		lResult = RegOpenKeyEx(keyServ,name,0,KEY_READ,&key);  
		if (lResult != ERROR_SUCCESS)  
		{  
			RegCloseKey(keyServ);  
			return;  
		}  


		size = 512;  
		//��ѯkey�µ��ֶ�ΪGroup���Ӽ��ֶ������浽name  
		lResult = RegQueryValueEx(key,TEXT("Group"),0,&type,(LPBYTE)name,&size);  
		if(lResult == ERROR_FILE_NOT_FOUND)  
		{  
			//?��������  
			RegCloseKey(key);  
			continue;  
		};  



		//�����ѯ����name����Video��˵���ü������Կ�������  
		if(_tcscmp(TEXT("Video"),name)!=0)  
		{  
			RegCloseKey(key);  
			continue;     //����forѭ��  
		};  

		//��������������ִ�еĻ�˵���Ѿ��鵽���й��Կ�����Ϣ������������Ĵ���ִ����֮��Ҫbreak��һ��forѭ������������  
		lResult = RegOpenKeyEx(key,TEXT("Enum"),0,KEY_READ,&key2);  
		RegCloseKey(key);  
		key = key2;  
		size = sizeof(count);  
		lResult = RegQueryValueEx(key,TEXT("Count"),0,&type,(LPBYTE)&count,&size);//��ѯCount�ֶΣ��Կ���Ŀ��  

		dwNum = count;//�����Կ���Ŀ  
		for(int j=0;j <count;++j)  
		{  
			TCHAR sz[512] = {0};  
			TCHAR name[64] = {0};  
			wsprintf(name,TEXT("%d"),j);  
			size = sizeof(sz);  
			lResult  = RegQueryValueEx(key,name,0,&type,(LPBYTE)sz,&size);  


			lResult = RegOpenKeyEx(keyEnum,sz,0,KEY_READ,&key2);  
			if (ERROR_SUCCESS)  
			{  
				RegCloseKey(keyEnum);  
				return;  
			}  


			size = sizeof(sz);  
			lResult = RegQueryValueEx(key2,TEXT("FriendlyName"),0,&type,(LPBYTE)sz,&size);  
			if(lResult == ERROR_FILE_NOT_FOUND)  
			{  
				size = sizeof(sz);  
				lResult = RegQueryValueEx(key2,TEXT("DeviceDesc"),0,&type,(LPBYTE)sz,&size);  
				chCardName[j] = sz;//�����Կ�����  
			};  
			RegCloseKey(key2);  
			key2 = NULL;  
		};  
		RegCloseKey(key);  
		key = NULL;  
		break;  
	}  
}
*/

#endif
