#include "PreHeader.h"
#include "scenes.h"
using d3du::CylinderScene;

static ID3DXMesh *g_pMesh=NULL;

CylinderScene::CylinderScene()
{
	m_pDevice=NULL;
	m_pMesh=NULL;
}
CylinderScene::CylinderScene(IDirect3DDevice9 *pDevice)
{
	m_pDevice=pDevice;
	m_pMesh=NULL;
}
CylinderScene::~CylinderScene()
{
	if(m_pMesh!=NULL) m_pMesh->Release();
}

void CylinderScene::Init(
	IDirect3DDevice9 *pDevice/*=NULL*/,
	float fRadius1/*=0.3f*/,
	float fRadius2/*=0.3f*/,
	float fLength/*=2.0f*/,
	UINT nSlices/*=10*/,
	UINT nStacks/*=10*/)
{
	m_pDevice=pDevice;
	m_fRadius1=fRadius1;
	m_fRadius2=fRadius2;
	m_fLength=fLength;
	m_nSlices=nSlices;
	m_nStacks=nStacks;
	D3DXCreateCylinder(
		m_pDevice,
		m_fRadius1,
		m_fRadius2,
		m_fLength,
		m_nSlices,
		m_nStacks,
		&m_pMesh,NULL);
	D3DXCreateSphere(m_pDevice,m_fRadius1,m_nSlices,m_nSlices,&g_pMesh,NULL);
}

void DrawSphere(IDirect3DDevice9 *m_pDevice)
{
	D3DMATERIAL9 mtl={0};
	mtl.Ambient=D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	mtl.Diffuse=D3DXCOLOR(0.8f,0.7f,0.8f,1.0f);
	mtl.Emissive=D3DXCOLOR(0.1f,0.0f,0.1f,1.0f);
	mtl.Specular=D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	mtl.Power=5.0f;
	m_pDevice->SetMaterial(&mtl);
	//FFD700
	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(light));
	light.Diffuse=D3DXCOLOR(0.9f,0.9f,0.9f,1.0f);
	light.Ambient=D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	light.Specular=D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	light.Type=D3DLIGHT_DIRECTIONAL;
	light.Direction=D3DXVECTOR3(0.0f,0.0f,1.0f);
	m_pDevice->SetLight(0,&light);
	m_pDevice->LightEnable(0,TRUE);

	D3DXMATRIX m;
	D3DXMatrixRotationX(&m,3.1515926f/2);
	m(3,0)=-2.0f;
	m(3,1)=2.55f;
	//x=-1
	for(int i=0;i<16;i++)
	{
		m_pDevice->SetTransform(D3DTS_WORLD,&m);
		g_pMesh->DrawSubset(0);
		m(3,2)+=2.0f;
	}

	m(3,0)=2.0f;
	m(3,2)=0.0f;
	for(int i=0;i<16;i++)
	{
		m_pDevice->SetTransform(D3DTS_WORLD,&m);
		g_pMesh->DrawSubset(0);
		m(3,2)+=2.0f;
	}
}

/*
 *从x=0开始，沿z=-2和z=2放一溜圆柱
 *
 */
void CylinderScene::Display()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	D3DMATERIAL9 mtl={0};
	mtl.Ambient=D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	mtl.Diffuse=D3DXCOLOR(0.7f,0.7f,0.7f,1.0f);
	mtl.Emissive=D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
	mtl.Specular=D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	mtl.Power=5.0f;
	m_pDevice->SetMaterial(&mtl);
	//FFD700
	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(light));
	light.Diffuse=D3DXCOLOR(0.9f,0.9f,0.9f,1.0f);
	light.Ambient=D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	light.Specular=D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	light.Type=D3DLIGHT_DIRECTIONAL;
	light.Direction=D3DXVECTOR3(-1.0f,0.0f,0.0f);
	m_pDevice->SetLight(0,&light);
	m_pDevice->LightEnable(0,TRUE);
	
	
	if(m_pTex!=NULL)
		m_pDevice->SetTexture(0,m_pTex);
	
	D3DXMATRIX m;
	D3DXMatrixRotationX(&m,3.1515926f/2);
	m(3,0)=-2.0f;
	m(3,1)=1.0f;
	//x=-1
	for(int i=0;i<16;i++)
	{
		m_pDevice->SetTransform(D3DTS_WORLD,&m);
		m_pMesh->DrawSubset(0);
		m(3,2)+=2.0f;
	}

	m(3,0)=2.0f;
	m(3,2)=0.0f;
	for(int i=0;i<16;i++)
	{
		m_pDevice->SetTransform(D3DTS_WORLD,&m);
		m_pMesh->DrawSubset(0);
		m(3,2)+=2.0f;
	}
	
	//DrawSphere
	DrawSphere(m_pDevice);
	
	D3DXMatrixTranslation(&m,0.0f,0.0f,0.0f);
	m_pDevice->SetTransform(D3DTS_WORLD,&m);
	
	if(m_pTex!=NULL)
		m_pDevice->SetTexture(0,NULL);
	mtl.Ambient=D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mtl.Diffuse=D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mtl.Emissive=D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mtl.Power=5.0f;
	m_pDevice->SetMaterial(&mtl);
	m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

}

void CylinderScene::SetTexture(IDirect3DTexture9 *pTex)
{
	m_pTex=pTex;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//AxesScene
using d3du::AxesScene;

AxesScene::AxesScene()
{
	m_pDevice=NULL;
	m_pVB=NULL;
}
AxesScene::~AxesScene()
{
	if(m_pVB!=NULL) m_pVB->Release();
}
void AxesScene::Init(IDirect3DDevice9 *pDevice)
{
	m_pDevice=pDevice;
	m_pDevice->CreateVertexBuffer(
		6*sizeof(D3DXVECTOR3),
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ,
		D3DPOOL_MANAGED,
		&m_pVB,NULL);
	
	D3DXVECTOR3 *vertices;
	m_pVB->Lock(0,0,(void**)&vertices,0);
	vertices[0]=D3DXVECTOR3(-2, 0, 0);
	vertices[1]=D3DXVECTOR3( 2, 0, 0);
	vertices[2]=D3DXVECTOR3( 0, 2, 0);
	vertices[3]=D3DXVECTOR3( 0,-2, 0);
	vertices[4]=D3DXVECTOR3( 0, 0,-2);
	vertices[5]=D3DXVECTOR3( 0, 0, 2);
	m_pVB->Unlock();
}
void AxesScene::Display()
{
	m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(D3DXVECTOR3));
	m_pDevice->SetFVF(D3DFVF_XYZ);
	m_pDevice->DrawPrimitive(D3DPT_LINELIST,0,3);
}

