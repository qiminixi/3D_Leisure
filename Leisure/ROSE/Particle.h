#ifndef D3DU_PARTICLE_H
#define D3DU_PARTICLE_H

#include <vector>

namespace d3du
{

struct SParticleVertex
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
	//float size;
	static const DWORD FVF;
};

struct SParticle
{
	D3DXVECTOR3 position;		//位置
	D3DXVECTOR3 velocity;		//速度
	D3DXVECTOR3 acceleration;	//加速度
	float lifetime;				//寿命
	float age;					//年龄
	D3DCOLOR color;			//颜色
	D3DCOLOR colorfade;		//颜色如何随时间减弱
	BOOL isAlive;				//是否处于活动状态
};

/*
 *（1）多线程安全上：
 *    Update正常情况下是在另一个线程中的，它进行写，Dislay所在的线程进行读。
 *虽然可能出现读的时候粒子死掉然后进行了写，导致读到脏数据的情况，但是也并
 *不影响。不可能出现写的时候的进行读，因为粒子还没有重生。
 *（2）默认是在正方形(-1,1)(1,1)(1,-1)(-1,1)内的随机出现的点
 *（3）粒子的是否死亡并不一定是由存在时间决定的，也可能是由位置、速度等决定的。
 *（4）不使用list记录粒子，虽然list可以让活动的和死亡的粒子分别集中在前面和后面
 *从而提高绘制效率，但是由于链表元素的位置在不断移动会产生线程同步上的问题，这
 *反倒划不来了。
 *（5）频繁的删除和创建内存是低效的，所以这里是一次将所需的粒子全部创建了。
 *
 */
class CParticleSys
{
public:

	CParticleSys();
	~CParticleSys();
	
	void Init(IDirect3DDevice9 *pDevice,UINT nNumParticles,UINT nMaxSegmentSize);//初始化，将调用ResetParticleBuffer
	void SetTexture(IDirect3DTexture9 *pTex){m_pTex=pTex;}
	void Display();//显示粒子
	virtual void Update();//更新粒子状态，杀死粒子、激活粒子
	virtual void ResetParticleBuffer();//重置粒子缓存中的所有粒子
	virtual void PreRender();//设置渲染状态
	virtual void PostRender();//恢复渲染状态
protected:
	IDirect3DDevice9 *m_pDevice;
	UINT m_nNumParticles;//粒子数
	UINT m_nMaxSegmentSize;//最大段大小，即绘制粒子时一次绘制的最大粒子数，实际绘制时可能会小于这个值
	float m_fParticleSize;//粒子尺寸
	
	UINT m_nVertBufPtr;//当前顶点缓存中最后一个数据的指针，初始值为0，缓存满了值是顶点数
	IDirect3DVertexBuffer9 *m_pVB;//顶点缓存，它的大小与粒子数相同
	IDirect3DTexture9 *m_pTex;//纹理
	std::vector<SParticle> m_ParticleBufffer;
};

//雪
class CSnow : public CParticleSys
{
public:
	virtual void Update();
	virtual void ResetParticleBuffer();
	virtual void PreRender();
};

//starts
class CStars:public CParticleSys
{
public:
	virtual void Update();
	virtual void ResetParticleBuffer();
	virtual void PreRender();
};

//爆炸
class CBlast:public CParticleSys
{
public:
	D3DXVECTOR3 m_posOrg;//起点
	D3DXCOLOR m_clr;//颜色
	virtual void Update();
	virtual void ResetParticleBuffer();
	virtual void PreRender();
	void SetParticleOrg(D3DXVECTOR3 *pPos){m_posOrg=*pPos;}
	void SetColor(D3DXCOLOR *pClr){m_clr=*pClr;}
};

}

#endif