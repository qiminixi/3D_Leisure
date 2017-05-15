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
	LONG lResult;//LONG型变量－保存函数返回值  

	//查询"SYSTEM\\CurrentControlSet\\Services"下的所有子键保存到keyServ  
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("SYSTEM\\CurrentControlSet\\Services"),0,KEY_READ,&keyServ);  
	if (ERROR_SUCCESS != lResult)  
		return;  


	//查询"SYSTEM\\CurrentControlSet\\Enum"下的所有子键保存到keyEnum  
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("SYSTEM\\CurrentControlSet\\Enum"),0,KEY_READ,&keyEnum);  
	if (ERROR_SUCCESS != lResult)  
		return;  

	int i = 0,count = 0;  
	DWORD size = 0,type = 0;  
	for (;;++i)  
	{  
		Sleep(5);  
		size = 512;  
		TCHAR name[512] = {0};//保存keyServ下各子项的字段名称  

		//逐个枚举keyServ下的各子项字段保存到name中  
		lResult = RegEnumKeyEx(keyServ,i,name,&size,NULL,NULL,NULL,NULL);  

		//要读取的子项不存在，即keyServ的子项全部遍历完时跳出循环  
		if(lResult == ERROR_NO_MORE_ITEMS)  
			break;  

		//打开keyServ的子项字段为name所标识的字段的值保存到key  
		lResult = RegOpenKeyEx(keyServ,name,0,KEY_READ,&key);  
		if (lResult != ERROR_SUCCESS)  
		{  
			RegCloseKey(keyServ);  
			return;  
		}  


		size = 512;  
		//查询key下的字段为Group的子键字段名保存到name  
		lResult = RegQueryValueEx(key,TEXT("Group"),0,&type,(LPBYTE)name,&size);  
		if(lResult == ERROR_FILE_NOT_FOUND)  
		{  
			//?键不存在  
			RegCloseKey(key);  
			continue;  
		};  



		//如果查询到的name不是Video则说明该键不是显卡驱动项  
		if(_tcscmp(TEXT("Video"),name)!=0)  
		{  
			RegCloseKey(key);  
			continue;     //返回for循环  
		};  

		//如果程序继续往下执行的话说明已经查到了有关显卡的信息，所以在下面的代码执行完之后要break第一个for循环，函数返回  
		lResult = RegOpenKeyEx(key,TEXT("Enum"),0,KEY_READ,&key2);  
		RegCloseKey(key);  
		key = key2;  
		size = sizeof(count);  
		lResult = RegQueryValueEx(key,TEXT("Count"),0,&type,(LPBYTE)&count,&size);//查询Count字段（显卡数目）  

		dwNum = count;//保存显卡数目  
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
				chCardName[j] = sz;//保存显卡名称  
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
