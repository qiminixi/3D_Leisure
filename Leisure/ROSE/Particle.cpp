#include "PreHeader.h"
#include "particle.h"

using d3du::CParticleSys;
using d3du::SParticle;
using d3du::SParticleVertex;

const DWORD SParticleVertex::FVF=D3DFVF_XYZ | D3DFVF_DIFFUSE;// |D3DFVF_PSIZE;

CParticleSys::CParticleSys()
{
	m_pVB=NULL;
	m_pTex=NULL;
	m_fParticleSize=0.02f;
}

CParticleSys::~CParticleSys()
{
	if(m_pVB) m_pVB->Release();
	if(m_pTex) m_pTex->Release();
	if(!m_ParticleBufffer.empty()) m_ParticleBufffer.clear();
}

void CParticleSys::Init(IDirect3DDevice9 *pDevice,UINT nNumParticles,UINT nMaxSegmentSize)
{
	m_pDevice=pDevice;
	m_nNumParticles=nNumParticles;
	m_nMaxSegmentSize=nMaxSegmentSize;
	
	m_pDevice->CreateVertexBuffer(
		m_nNumParticles*sizeof(SParticleVertex),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
		SParticleVertex::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		NULL);
	ResetParticleBuffer();
}

//重置粒子缓存中的所有粒子
void CParticleSys::ResetParticleBuffer()
{
	m_ParticleBufffer.resize(m_nNumParticles);
	for(unsigned int i=0;i<m_nNumParticles;i++)
	{
		m_ParticleBufffer[i].position.x=(rand()%200-100)*0.01f;//[-1,1)
		m_ParticleBufffer[i].position.y=(rand()%200-100)*0.01f;//[-1,1)
		m_ParticleBufffer[i].position.z=(rand()%200-100)*0.01f;//[-1,1)
		m_ParticleBufffer[i].color=0xffffffff;
	}
}
/*
void CParticleSys::Display()
{
	//UINT nSegmentStartIndex;
	UINT nSegmentSize;
	UINT nSegmentDataSize;//一个分段里里面的顶点数，最后那个段里面的顶点数可能小于段大小
	PreRender();

	int nTest=0;
	UINT i=0;
	
	m_nVertBufPtr=0;
	m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(SParticleVertex));
	m_pDevice->SetFVF(SParticleVertex::FVF);

	//遍历粒子缓存
	while(i<m_nNumParticles)
	{
		//段的大小计算
		//nSegmentStartIndex=m_nVertBufPtr;	
		nSegmentSize=m_nNumParticles-m_nVertBufPtr;
		if(nSegmentSize>m_nMaxSegmentSize)
			nSegmentSize=m_nMaxSegmentSize;

		//锁定缓存
		SParticleVertex *vertices=NULL;
		m_pVB->Lock(
			m_nVertBufPtr,
			nSegmentSize,
			(void**)&vertices,
			D3DLOCK_DISCARD);

		//将活动的粒子加到顶点缓存中
		nSegmentDataSize=0;
		for(i;i<m_nNumParticles;i++)
		{
			if(m_ParticleBufffer[i].isAlive)
			{
				vertices[nSegmentDataSize].pos=m_ParticleBufffer[i].position;
				vertices[nSegmentDataSize].color=0xffffff00;
				nSegmentDataSize++;
			}
			if(nSegmentDataSize>=nSegmentSize){
				i++;//直接break就没有算最后那个元素了
				break;
			}
		}
		m_pVB->Unlock();
		nTest+=nSegmentDataSize;

		//绘制图形
		m_pDevice->DrawPrimitive(D3DPT_POINTLIST,m_nVertBufPtr,nSegmentDataSize);
		m_nVertBufPtr+=nSegmentDataSize;
		if(m_nVertBufPtr>=m_nNumParticles)
			m_nVertBufPtr=0;
	}

	PostRender();
}*/
//*
void CParticleSys::Display()
{
	UINT nSegmentSize;//片段的大小
	UINT nSegmentDataSize;//一个分段里里面的顶点数，最后那个段里面的顶点数可能小于段大小
	UINT i=0;
	
	m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(SParticleVertex));
	m_pDevice->SetFVF(SParticleVertex::FVF);
	if(m_pTex) m_pDevice->SetTexture(0,m_pTex);
	PreRender();
	
	//遍历粒子缓存
	while(i<m_nNumParticles)
	{
		//片段的大小计算
		nSegmentSize=m_nNumParticles-m_nVertBufPtr;
		if(nSegmentSize>m_nMaxSegmentSize)
			nSegmentSize=m_nMaxSegmentSize;
		
		//锁定缓存
		SParticleVertex *vertices=NULL;
		m_pVB->Lock(
			m_nVertBufPtr*sizeof(SParticleVertex),
			nSegmentSize*sizeof(SParticleVertex),
			(void**)&vertices,
			(m_nVertBufPtr? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD));	
		
		//将活动的粒子加到顶点缓存中
		nSegmentDataSize=0;
		for(i;i<m_nNumParticles;i++)
		{
			if(m_ParticleBufffer[i].isAlive)
			{
				vertices[nSegmentDataSize].pos=m_ParticleBufffer[i].position;
				vertices[nSegmentDataSize].color=m_ParticleBufffer[i].color;
				nSegmentDataSize++;//计算片段中顶点的数量
			}
			if(nSegmentDataSize>=nSegmentSize){
				i++;//直接break就没有算最后那个元素了
				break;
			}
		}
		m_pVB->Unlock();
		
		//绘制图形
		m_pDevice->DrawPrimitive(D3DPT_POINTLIST,m_nVertBufPtr,nSegmentDataSize);
		m_nVertBufPtr+=nSegmentDataSize;
		if(m_nVertBufPtr>=m_nNumParticles)
			m_nVertBufPtr=0;
	}
	if(m_pTex) m_pDevice->SetTexture(0,NULL);
	PostRender();
}

//设置渲染状态
void CParticleSys::PreRender()
{
	m_fParticleSize=0.8f;
	float f;
	m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,TRUE);
	
	//点尺寸
	m_pDevice->SetRenderState(D3DRS_POINTSIZE,*((DWORD*)&m_fParticleSize));
	f=0.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN,*((DWORD*)&f));//像素
	f=2.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MAX,*((DWORD*)&f));//像素
	
	//缩放模式
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,TRUE);
	f=0.0f;m_pDevice->SetRenderState(D3DRS_POINTSCALE_A,*((DWORD*)&f));
	f=0.0f;m_pDevice->SetRenderState(D3DRS_POINTSCALE_B,*((DWORD*)&f));
	f=1.0f;m_pDevice->SetRenderState(D3DRS_POINTSCALE_C,*((DWORD*)&f));
	
	//Alpha混合
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//恢复渲染状态
void CParticleSys::PostRender()
{
	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}

//更新粒子状态，杀死粒子、激活粒子
void CParticleSys::Update()
{
	for(UINT i=0;i<m_nNumParticles;i++)
	{
		m_ParticleBufffer[i].position.x=(rand()%200-100)*0.01f;//[-1,1)
		m_ParticleBufffer[i].position.y=(rand()%200-100)*0.01f;//[-1,1)
		m_ParticleBufffer[i].position.z=-0.5f;//[-1,1)
		m_ParticleBufffer[i].color=0xffffffff;
	}
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//Snow
using d3du::CSnow;
void CSnow::ResetParticleBuffer()
{
	m_ParticleBufffer.resize(m_nNumParticles);
	for(unsigned int i=0;i<m_nNumParticles;i++)
	{
		m_ParticleBufffer[i].position.x=(rand()%1200-600)*0.01f;//[-3,3)
		m_ParticleBufffer[i].position.y=3.0f;
		m_ParticleBufffer[i].position.z=(rand()%600-300)*0.01f;//[-3,3)
		m_ParticleBufffer[i].color=0xffffffff;
	}
}

//更新粒子状态，杀死粒子、激活粒子
void CSnow::Update()
{
	for(UINT i=0;i<m_nNumParticles;i++)
	{
		m_ParticleBufffer[i].position.y-=0.02f;
		if(m_ParticleBufffer[i].position.y<0.0f)
			m_ParticleBufffer[i].position.y=3.0f;
		m_ParticleBufffer[i].color=0xffffffff;
	}
}

void CSnow::PreRender()
{
	CParticleSys::PreRender();
	
	float f;
	f=1.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE,*((DWORD*)&f));
	f=0.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN,*((DWORD*)&f));
	f=10.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MAX,*((DWORD*)&f));
}

//Stars
using d3du::CStars;
void CStars::ResetParticleBuffer()
{
	m_ParticleBufffer.resize(m_nNumParticles);
	for(unsigned int i=0;i<m_nNumParticles;i++)
	{
		m_ParticleBufffer[i].position.x=(rand()%800-400);//[-3,3)
		m_ParticleBufffer[i].position.y=(rand()%50+80);
		m_ParticleBufffer[i].position.z=(rand()%600-300);//[-3,3)
		m_ParticleBufffer[i].color=0x00ffffff;
	}
}

//更新粒子状态，杀死粒子、激活粒子
void CStars::Update()
{
	for(UINT i=0;i<m_nNumParticles;i++)
	{
		;
	}
}

void CStars::PreRender()
{
	CParticleSys::PreRender();

	float f;
	f=1.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE,*((DWORD*)&f));
	f=0.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN,*((DWORD*)&f));
	f=10.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MAX,*((DWORD*)&f));
}

using d3du::CBlast;
void CBlast::ResetParticleBuffer()
{
	m_ParticleBufffer.resize(m_nNumParticles);
	for(unsigned int i=0;i<m_nNumParticles;i++)
	{
		m_ParticleBufffer[i].isAlive=TRUE;
		m_ParticleBufffer[i].position=m_posOrg;
		m_ParticleBufffer[i].color=(D3DCOLOR)m_clr;
		m_ParticleBufffer[i].velocity=D3DXVECTOR3(
			((float)(rand()%10))/10-0.5,
			((float)(rand()%10))/10-0.5,
			((float)(rand()%10))/10-0.5
			);
		m_ParticleBufffer[i].acceleration=m_ParticleBufffer[i].velocity;
	}
}

//更新粒子状态，杀死粒子、激活粒子
void CBlast::Update()
{
	for(UINT i=0;i<m_nNumParticles;i++)
	{
		m_ParticleBufffer[i].position+=m_ParticleBufffer[i].velocity;
		m_ParticleBufffer[i].velocity+=m_ParticleBufffer[i].acceleration;
	}
}

void CBlast::PreRender()
{
	CParticleSys::PreRender();

	float f;
	f=1.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE,*((DWORD*)&f));
	f=0.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN,*((DWORD*)&f));
	f=2.0f;m_pDevice->SetRenderState(D3DRS_POINTSIZE_MAX,*((DWORD*)&f));
}