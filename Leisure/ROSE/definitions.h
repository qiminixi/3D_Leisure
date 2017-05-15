#ifndef D3DUTILITIES_DEF_H
#define D3DUTILITIES_DEF_H

//ÑÕÉ«
#define D3DU_COLOR_RED			0xffff0000		//ºì
#define D3DU_COLOR_YELLO		0xffff0000		//»Æ
#define D3DU_COLOR_GREEN		0xff00ff00		//ÂÌ
#define D3DU_COLOR_BLUE			0xff0000ff		//À¶

#define D3DU_XCOLOR_RED			(1,0,0,1)		//ºì
#define D3DU_XCOLOR_YELLO		(1,1,0,1)		//»Æ
#define D3DU_XCOLOR_GREEN		(0,1,0,1)		//ÂÌ
#define D3DU_XCOLOR_BLUE		(0,0,1,1)		//À¶

#define ROSE_ONE_DEGREE 0.01745329f
//#define ROSE_RANDF(a,b) rand()

//²ÄÖÊ
#define D3DU_MTRL_RED(mtrl)\
	mtrl

//¹âÕÕ
#define D3DU_LIGHT_D_RED(light)\
	light

struct ROSEPOINTF
{
	float _x,_y;
	ROSEPOINTF(float x=0.0f,float y=0.0f)
	{
		_x=x;_y=y;
	}
};

struct ROSEVertTexture
{
	float _x,_y,_z;
	float _u,_v;
	ROSEVertTexture(float x,float y,float z,float u,float v)
	{
		_x=x;_y=y;_z=z;
		_u=u;_v=v;
	}
	static const DWORD FVF;
};

//const DWORD ROSEVertTexture::FVF=D3DFVF_XYZ|D3DFVF_TEX1;

#endif