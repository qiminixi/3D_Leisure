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
	D3DXVECTOR3 position;		//λ��
	D3DXVECTOR3 velocity;		//�ٶ�
	D3DXVECTOR3 acceleration;	//���ٶ�
	float lifetime;				//����
	float age;					//����
	D3DCOLOR color;			//��ɫ
	D3DCOLOR colorfade;		//��ɫ�����ʱ�����
	BOOL isAlive;				//�Ƿ��ڻ״̬
};

/*
 *��1�����̰߳�ȫ�ϣ�
 *    Update���������������һ���߳��еģ�������д��Dislay���ڵ��߳̽��ж���
 *��Ȼ���ܳ��ֶ���ʱ����������Ȼ�������д�����¶��������ݵ����������Ҳ��
 *��Ӱ�졣�����ܳ���д��ʱ��Ľ��ж�����Ϊ���ӻ�û��������
 *��2��Ĭ������������(-1,1)(1,1)(1,-1)(-1,1)�ڵ�������ֵĵ�
 *��3�����ӵ��Ƿ���������һ�����ɴ���ʱ������ģ�Ҳ��������λ�á��ٶȵȾ����ġ�
 *��4����ʹ��list��¼���ӣ���Ȼlist�����û�ĺ����������ӷֱ�����ǰ��ͺ���
 *�Ӷ���߻���Ч�ʣ�������������Ԫ�ص�λ���ڲ����ƶ�������߳�ͬ���ϵ����⣬��
 *�����������ˡ�
 *��5��Ƶ����ɾ���ʹ����ڴ��ǵ�Ч�ģ�����������һ�ν����������ȫ�������ˡ�
 *
 */
class CParticleSys
{
public:

	CParticleSys();
	~CParticleSys();
	
	void Init(IDirect3DDevice9 *pDevice,UINT nNumParticles,UINT nMaxSegmentSize);//��ʼ����������ResetParticleBuffer
	void SetTexture(IDirect3DTexture9 *pTex){m_pTex=pTex;}
	void Display();//��ʾ����
	virtual void Update();//��������״̬��ɱ�����ӡ���������
	virtual void ResetParticleBuffer();//�������ӻ����е���������
	virtual void PreRender();//������Ⱦ״̬
	virtual void PostRender();//�ָ���Ⱦ״̬
protected:
	IDirect3DDevice9 *m_pDevice;
	UINT m_nNumParticles;//������
	UINT m_nMaxSegmentSize;//���δ�С������������ʱһ�λ��Ƶ������������ʵ�ʻ���ʱ���ܻ�С�����ֵ
	float m_fParticleSize;//���ӳߴ�
	
	UINT m_nVertBufPtr;//��ǰ���㻺�������һ�����ݵ�ָ�룬��ʼֵΪ0����������ֵ�Ƕ�����
	IDirect3DVertexBuffer9 *m_pVB;//���㻺�棬���Ĵ�С����������ͬ
	IDirect3DTexture9 *m_pTex;//����
	std::vector<SParticle> m_ParticleBufffer;
};

//ѩ
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

//��ը
class CBlast:public CParticleSys
{
public:
	D3DXVECTOR3 m_posOrg;//���
	D3DXCOLOR m_clr;//��ɫ
	virtual void Update();
	virtual void ResetParticleBuffer();
	virtual void PreRender();
	void SetParticleOrg(D3DXVECTOR3 *pPos){m_posOrg=*pPos;}
	void SetColor(D3DXCOLOR *pClr){m_clr=*pClr;}
};

}

#endif