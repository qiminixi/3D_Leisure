#include <windows.h>
#include <d3dx9.h>

#include "Game.h"
#include "sky.h"
#include "toy.h"
#include "../ROSE/d3dUtilities.h"


//d3dxloadm

struct SObjecMesh
{
	D3DXVECTOR3 _pos;
	ID3DXMesh *_pMesh;
	IDirect3DTexture9 *_pTexture;
	SObjecMesh *_pNext;
};

struct SObjectVertex
{
	D3DXVECTOR3 _pos;
	IDirect3DVertexBuffer9 *_pVB;
	UINT _nNumVertices;
	UINT _nNumFaces;
};

struct SObjectIndex
{
};

struct SThing
{
	ID3DXMesh *_pMeshUp;
	ID3DXMesh *_pMeshDown;
	
};
struct ROSECCopys
{
#define ROSE_COPYS_NUM 200
	SThing m_thing;
	D3DXVECTOR3 m_arPos[ROSE_COPYS_NUM];//λ��
	float m_arScales[ROSE_COPYS_NUM];//�ߴ�
	D3DXCOLOR m_arDiffuse[ROSE_COPYS_NUM];//�����ɫ
	D3DXCOLOR m_arEmissive[ROSE_COPYS_NUM];//������ɫ
};

BOOL ActorXXX(const D3DXVECTOR3 *pMoveVec);
BOOL WeaponXXX();

static IDirect3D9 *g_pD3D=NULL;
static IDirect3DDevice9 *g_pDevice=NULL;
static ROSECTerrain g_mainland;
static ROSECamera g_camera;
static d3du::AxesScene g_axes;
static ID3DXMesh *g_pMesh;

static ROSECSky g_sky;
static CToy g_toy;
static BOOL g_nToyMovedTimes;//actor�ƶ��Ĵ�����0��ʾ���ƶ�������1��ʾ�ƶ��Ĵ���
static UINT vkdown;//����ȥ�ļ�

SThing g_tingA;
ROSECCopys g_copys;
int g_nActionState;//����״̬��0=�޶�����1=׼����2=�ж���3=����ֻ���˶����������������˶�
D3DXVECTOR3 g_posWeapon;//��������������ϵ����


static d3du::CBlast g_blast;
BOOL g_bRenderBlast;//�Ƿ���Ⱦ��ը
static d3du::CText g_text;

IDirect3DTexture9 *g_pTextureA=NULL;

//��ʼ����Ϸ��������Ϸ��Դ
void GameInit(HWND hWnd)
{
	HRESULT hr;
	g_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
	
	D3DPRESENT_PARAMETERS d3dpp={0};
	d3dpp.Windowed=TRUE;
	d3dpp.EnableAutoDepthStencil=TRUE;
	d3dpp.AutoDepthStencilFormat=D3DFMT_D24S8;
	d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
	hr=g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pDevice);
	if(FAILED(hr))
	{
		return;
	}
	
	D3DCAPS9 d3dcaps;
	g_pDevice->GetDeviceCaps(&d3dcaps);
	
	g_axes.Init(g_pDevice);
	//�������
	g_camera.Init(
		ROSECamera::LandObject,//Aircraft,
		ROSECamera::ViewType_Third,
		&D3DXVECTOR3(0.0f,2.8f,-6.0f),
		&D3DXVECTOR3(0.0f,0.0f,1.0f),
		&D3DXVECTOR3(0.0f,1.0f,0.0f));
	g_camera.SetHeight(2.8f);
	g_camera.SetCenter(&D3DXVECTOR3(0.0f,2.8f,0.3f));
	POINT pos;
	GetCursorPos(&pos);
	g_camera.SetInitMousePos(pos.x,pos.y);
	
	//�������
	int x=200;
	g_mainland.Init(g_pDevice,ROSEPOINTF(-x,x),x,x,2.0f,1.0f,TEXT("res\\mainland.raw"));
	IDirect3DTexture9 *pTexture=NULL;
	D3DXCreateTextureFromFile(g_pDevice,TEXT("res\\ground.jpg"),&pTexture);
	g_mainland.SetTexture(pTexture);
	g_mainland.m_pCamera=&g_camera;
	g_mainland.m_fViewRadius=50.0f;
	g_camera.SetPosY(g_mainland.GetHeight(0,0));//�����ڵر��λ��
	
	//���
	g_sky.Init(g_pDevice);
	
	//��������
	D3DXCreateBox(g_pDevice,1,6,1,&g_tingA._pMeshDown,NULL);
	D3DXCreateSphere(g_pDevice,0.6f,10,10,&g_tingA._pMeshUp,NULL);
	D3DXVECTOR3 *pVetex=NULL;
	char *pBuffer=NULL;
	IDirect3DVertexBuffer9 *pVB=NULL;
	UINT nStride=0;
	//--���Ʒ���
	g_tingA._pMeshDown->GetVertexBuffer(&pVB);
	nStride=g_tingA._pMeshDown->GetNumBytesPerVertex();
	pVB->Lock(0,0,(void**)&pBuffer,0);
	for(int i=0;i<g_tingA._pMeshDown->GetNumVertices();i++)
	{
		pVetex=(D3DXVECTOR3*)pBuffer;
		*pVetex+=D3DXVECTOR3(0.0f,2.9f,0.0f);
		pBuffer+=nStride;
	}
	pVB->Unlock();
	pVB=NULL;
	//--��������
	g_tingA._pMeshUp->GetVertexBuffer(&pVB);
	nStride=g_tingA._pMeshUp->GetNumBytesPerVertex();
	pVB->Lock(0,0,(void**)&pBuffer,0);
	for(int i=0;i<g_tingA._pMeshUp->GetNumVertices();i++)
	{
		pVetex=(D3DXVECTOR3*)pBuffer;
		*pVetex+=D3DXVECTOR3(0.0f,7.0f,0.0f);
		pBuffer+=nStride;
	}
	pVB->Unlock();
	
	//���ɻ�Ӱ��λ����(-200~200)����С�ڣ�0.5~3)
	g_copys.m_thing=g_tingA;
	for(int i=0;i<ROSE_COPYS_NUM;i++)
	{
		g_copys.m_arPos[i].x=rand()%400-200;
		g_copys.m_arPos[i].z=rand()%400-200;
		g_copys.m_arPos[i].y=g_mainland.GetHeight(g_copys.m_arPos[i].x,g_copys.m_arPos[i].z);
		
		g_copys.m_arScales[i]=((float)(rand()%40))/10+1.0;
		g_copys.m_arDiffuse[i]=D3DXCOLOR(float(rand()%100)/100,float(rand()%100)/100,float(rand()%100)/100,1);
		g_copys.m_arEmissive[i]=D3DXCOLOR(float(rand()%100)/100,float(rand()%100)/100,float(rand()%100)/100,1);
	}
	
	D3DXCreateBox(g_pDevice,1,1,1,&g_pMesh,NULL);
	
	//Toy
	g_toy.Init(g_pDevice);
	g_nToyMovedTimes=0;
	g_toy.pCamera=&g_camera;
	g_toy.UpdatePos();
	g_nActionState=0;
	
	//blast
	g_blast.Init(g_pDevice,1000,500);
	
	//����
	char szTest[]="(1)W=ǰ����S=���ˣ�A=���ƣ�D=����\n"
		"(2)�������=���Ͽ�����=���¿�����=��ɨ�ӣ���=��ɨ��\n"
		"(3)��C�������ڵ�һ�˳��ӽǺ͵����˳��ӽ�֮������л�\n"
		"(4)��Space�����Խ�����Ͷ����ȥ���������ӵĻ���������Ч��\n"
		"(*)�������Ҫ��GT225M�����Կ���Pentium II���ϴ�����";
	g_text.Init(g_pDevice,0,0,400,200,szTest,NULL,0xffffff00);
	
	D3DXCreateTextureFromFile(g_pDevice,"res\\2.jpg",&g_pTextureA);
}

//�ر���Ϸ
void GameDistroy()
{
	//...
}


void RenderThing(IDirect3DDevice9 *g_pDevice,SThing *pThing)
{
	pThing->_pMeshDown->DrawSubset(0);
	pThing->_pMeshUp->DrawSubset(0);
}

void GameRender()
{	
	g_pDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xffffffff,1.0f,0);
	
	D3DXMATRIX mtrView,mtrProj;
	g_camera.GetViewMatrix(&mtrView);
	D3DXMatrixPerspectiveFovLH(&mtrProj,3.1415926f/2,1.333f,0.1f,1000.0f);
	g_pDevice->SetTransform(D3DTS_VIEW,&mtrView);
	g_pDevice->SetTransform(D3DTS_PROJECTION,&mtrProj);
	
	//�������е�����
	g_pDevice->BeginScene();
	
	g_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	//g_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	//g_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	g_mainland.Display();
	g_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
	//g_axes.Display();
	//g_pMesh->DrawSubset(0);
	//RenderThing(g_pDevice,&g_tingA);
	
	//���
	g_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	g_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
	g_sky.Render(g_pDevice);
	g_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	
	g_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	//����
	D3DMATERIAL9 matl={0};
	D3DLIGHT9 light;
	D3DXMATRIX mtrWorld;
	D3DXMatrixIdentity(&mtrWorld);
	light.Type=D3DLIGHT_DIRECTIONAL;
	light.Ambient=D3DXCOLOR(0.9f,0.9f,0.9f,1);
	light.Diffuse=D3DXCOLOR(0.9f,0.9f,0.9f,1);
	light.Specular=D3DXCOLOR(0.9f,0.9f,0.9f,1);
	light.Direction=D3DXVECTOR3(-1.0f, -0.6f, 0.8f);
	
	g_pDevice->SetLight(0,&light);
	g_pDevice->LightEnable(0,TRUE);
	for(int i=0;i<ROSE_COPYS_NUM;i++)//ROSE_COPYS_NUM
	{
		mtrWorld._41=g_copys.m_arPos[i].x;
		mtrWorld._42=g_copys.m_arPos[i].y;
		mtrWorld._43=g_copys.m_arPos[i].z;
		mtrWorld._11=g_copys.m_arScales[i];
		mtrWorld._22=g_copys.m_arScales[i];
		mtrWorld._33=g_copys.m_arScales[i];
		matl.Diffuse=g_copys.m_arDiffuse[i];
		//matl.Emissive=g_copys.m_arEmissive[i];
		matl.Emissive.r=g_copys.m_arDiffuse[i].r*0.3;
		matl.Emissive.g=g_copys.m_arDiffuse[i].g*0.3;
		matl.Emissive.b=g_copys.m_arDiffuse[i].b*0.3;
		matl.Specular.r=g_copys.m_arDiffuse[i].r*0.1;
		matl.Specular.g=g_copys.m_arDiffuse[i].g*0.1;
		matl.Specular.b=g_copys.m_arDiffuse[i].b*0.1;
		g_pDevice->SetMaterial(&matl);
		g_pDevice->SetTransform(D3DTS_WORLD,&mtrWorld);
		g_copys.m_thing._pMeshDown->DrawSubset(0);
		g_copys.m_thing._pMeshUp->DrawSubset(0);
	}
	D3DXMatrixIdentity(&mtrWorld);
	g_pDevice->SetTransform(D3DTS_WORLD,&mtrWorld);
	
	g_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	g_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS,TRUE);
	
	g_camera.GetRotateMatrix(&mtrWorld);
	mtrWorld._41=g_toy.m_pos.x;//g_camera.m_vecPos.x;
	mtrWorld._42=g_toy.m_pos.y;//g_camera.m_vecPos.y-g_camera.GetHeight();
	mtrWorld._43=g_toy.m_pos.z;//g_camera.m_vecPos.z-0.3;
	g_pDevice->SetTransform(D3DTS_WORLD,&mtrWorld);
	ROSEDump_OutputWindow("%3.f,%3.3f,%3.3f\n",g_toy.m_pos.x,g_toy.m_pos.y,g_toy.m_pos.z);
	
	matl.Diffuse=D3DXCOLOR(0.4f,0.0f,0.4f,1.0f);
	matl.Emissive=D3DXCOLOR(0.3f,0.0f,0.5f,1.0f);
	matl.Ambient=D3DXCOLOR(0.3f,0.0f,0.3f,1.0f);
	matl.Specular=D3DXCOLOR(0.1f,0.1f,0.1f,1.0f);
	//matl.Power=5.0f;
	g_pDevice->SetMaterial(&matl);
	g_toy.Render(g_pDevice);
	
	D3DXMATRIX mtrR,mtrWorldT;
	if(g_nActionState>0 && g_nActionState<3)
	{
		//����ֻ�֣��ֲ���ת--�ֲ�ƽ��--������ת--����ƽ��
		g_camera.GetRotateMatrix(&mtrWorld);
		D3DXMatrixRotationX(&mtrR,g_toy.m_fAngelThirdHand);
		D3DXMatrixIdentity(&mtrWorldT);
		mtrWorldT._41=0.51f;
		mtrWorldT._42=2.70f;
		mtrWorldT._43=0.00f;
		mtrWorld._41=g_toy.m_pos.x;//g_camera.m_vecPos.x;
		mtrWorld._42=g_toy.m_pos.y;//g_camera.m_vecPos.y-g_camera.GetHeight();
		mtrWorld._43=g_toy.m_pos.z;//g_camera.m_vecPos.z-0.3;
		mtrWorld=mtrR*mtrWorldT*mtrWorld;
		mtrWorld=mtrWorld;
	
		g_pDevice->SetTransform(D3DTS_WORLD,&mtrWorld);
		g_toy.m_thirdhand->DrawSubset(0);
		//g_toy.m_fAngelThirdHand+=3.1415926f/180*10;
	}
	if(g_nActionState>0)
	{
		//�������ֲ���ת--�ֲ�ƽ��--������ת--����ƽ��
		//g_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
		g_camera.GetRotateMatrix(&mtrWorld);
		D3DXMatrixRotationX(&mtrR,g_toy.m_fAngelWeapon);
		D3DXMatrixIdentity(&mtrWorldT);
		mtrWorldT._41=g_toy.m_posWeapon.x;
		mtrWorldT._42=g_toy.m_posWeapon.y;
		mtrWorldT._43=g_toy.m_posWeapon.z;
		mtrWorld._41=g_toy.m_pos.x;//g_camera.m_vecPos.x;
		mtrWorld._42=g_toy.m_pos.y;//g_camera.m_vecPos.y-g_camera.GetHeight();
		mtrWorld._43=g_toy.m_pos.z;//g_camera.m_vecPos.z-0.3;

		D3DXMATRIX mtrScale;
		D3DXMatrixScaling(&mtrScale,g_toy.m_fWeaponScale,g_toy.m_fWeaponScale,g_toy.m_fWeaponScale);
		mtrWorld=mtrScale*mtrR*mtrWorldT*mtrWorld;
		g_pDevice->SetTransform(D3DTS_WORLD,&mtrWorld);
		
		if(g_pTextureA)
			g_pDevice->SetTexture(0,g_pTextureA);

		g_toy.m_weapon->DrawSubset(0);
		g_toy.m_fAngelWeapon+=3.1415926f/180*10;
		D3DXVec3TransformCoord(&g_posWeapon,&g_toy.m_posWeapon,&mtrWorld);
		g_pDevice->SetTexture(0,NULL);
		g_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	}
	D3DXMatrixIdentity(&mtrWorld);
	g_pDevice->SetTransform(D3DTS_WORLD,&mtrWorld);
	
	if(g_bRenderBlast)
	{
		g_blast.Display();
		g_blast.Update();
		g_bRenderBlast++;
		if(g_bRenderBlast>=10)
			g_bRenderBlast=0;
	}
	
	g_text.Display();

	g_pDevice->EndScene();
	g_pDevice->Present(0,0,0,0);
}


void KeyDown(UINT vk)
{
	const float MOVEUNIT=0.3f;
	const float MOVEANGEL=(3.1415926f/180*1);
	switch(vk)
	{
		//ǰ������
	case 'W':
		g_camera.MoveFB(MOVEUNIT);//ǰ��
		if(!ActorXXX(&g_camera.m_vecLook)) g_camera.MoveFB(-MOVEUNIT);//����
		else g_nToyMovedTimes++;
		g_camera.SetPosY(g_mainland.GetHeight(g_camera.m_vecPos.x,g_camera.m_vecPos.z));
		break;
	case 'S':
		g_camera.MoveFB(-MOVEUNIT);//����
		g_nToyMovedTimes++;
		g_camera.SetPosY(g_mainland.GetHeight(g_camera.m_vecPos.x,g_camera.m_vecPos.z));
		break;

		//����
	case 'A':
		g_camera.MoveRL(-MOVEUNIT);//��
		g_nToyMovedTimes++;
		g_camera.SetPosY(g_mainland.GetHeight(g_camera.m_vecPos.x,g_camera.m_vecPos.z));
		break;
	case 'D':
		g_camera.MoveRL(MOVEUNIT);//��
		g_nToyMovedTimes++;
		g_camera.SetPosY(g_mainland.GetHeight(g_camera.m_vecPos.x,g_camera.m_vecPos.z));
		break;

		//����
	case 'R':
		g_camera.MoveUD(MOVEUNIT);//��
		break;

	case 'F':
		g_camera.MoveUD(-MOVEUNIT);//��
		break;

		//����
	case VK_UP:
		g_camera.Pitch(MOVEANGEL);//��
		break;
	case VK_DOWN:
		g_camera.Pitch(-MOVEANGEL);//��
		break;

		//ɨ��
	case VK_LEFT:
		g_camera.Yaw(-MOVEANGEL);//��ɨ
		break;
	case VK_RIGHT:
		g_camera.Yaw(MOVEANGEL);//��ɨ
		break;
		
	case 'C'://�л��ӽ�
		if(g_camera.m_viewtype==ROSECamera::ViewType_First)
			g_camera.m_viewtype=ROSECamera::ViewType_Third;
		else if(g_camera.m_viewtype==ROSECamera::ViewType_Third)
			g_camera.m_viewtype=ROSECamera::ViewType_First;
		break;
	case VK_SPACE://׼������
		if(g_nActionState==0)//�ո���ʱִ��һ�ξͿ�����
		{
			g_toy.m_fWeaponScale=0.25f;
			g_toy.m_fAngelThirdHand=0.0f;
			g_toy.m_posWeapon=g_toy.m_posWeaponOrg;
			g_toy.m_vecWeaponVel=D3DXVECTOR3(0.0f,3.0f,3.0f);
			g_nActionState=1;
		}
		break;
		

	}
	g_toy.UpdatePos();
}
void KeyUp(UINT vk)
{
	g_nToyMovedTimes=0;
	if(vk==VK_SPACE)
		g_nActionState=2;
}

//����
void GameInput()
{
	POINT pos;
	GetCursorPos(&pos);
	g_camera.MouseMove(pos.x,pos.y);
	g_toy.UpdatePos();
}

void GameRun_AI()
{
	//actor
	//����û������룬��Ƶ���л�֡
	//���200ms��û�����룬����Ϊֹͣ�ƶ���
	static clock_t t1=clock();
	clock_t t2;
	if(g_nToyMovedTimes==0)
	{
		g_toy.m_nCurFrame=0;
	}
	else if(g_nToyMovedTimes==1)
	{
		g_toy.m_nCurFrame=1;
	}
	else
	{
		t2=clock();
		if(t2-t1>CLOCKS_PER_SEC/5)
		{
			t1=t2;
			if(g_toy.m_nCurFrame==1)
				g_toy.m_nCurFrame=2;
			else
				g_toy.m_nCurFrame=1;
		}
	}
	
	if(g_nActionState>=2)
	{
		if(g_toy.m_fAngelThirdHand>ROSE_ONE_DEGREE*180)//��������ֻ�ֵĶ���
		{
			g_nActionState=3;
		}
		else g_toy.m_fAngelThirdHand+=ROSE_ONE_DEGREE*70;
		
		//�����ƶ�����
		g_toy.m_posWeapon.x+=g_toy.m_vecWeaponVel.x;
		g_toy.m_posWeapon.y+=g_toy.m_vecWeaponVel.y;
		g_toy.m_posWeapon.z+=g_toy.m_vecWeaponVel.z;
		g_toy.m_vecWeaponVel+=g_toy.m_vecWeaponAcc;
		g_toy.m_fWeaponScale+=0.4;
		if(g_toy.m_posWeapon.y<0)
		{
			g_nActionState=0;
		}
		if(!WeaponXXX())
		{
			g_nActionState=0;
			g_bRenderBlast=TRUE;
			//��WeaponXXX������������blast����Ϊ��������Ի�ȡ����ɫ
		}
		
	}
}

BOOL ActorXXX(const D3DXVECTOR3 *pMoveVec)
{
	//�ж�actor���������ײ״̬
	for(int i=0;i<ROSE_COPYS_NUM;i++)
	{
		//���ж������Ƿ���ǰ��������
		D3DXVECTOR3 vecActorObject;//actor��Object������
		vecActorObject=g_copys.m_arPos[i]-g_toy.m_pos;
		float f=D3DXVec3Dot(pMoveVec,&vecActorObject);
		if(f<0)
			continue;
		
		float w,h,d;
		w=g_copys.m_arScales[i]*1;
		h=g_copys.m_arScales[i]*6;
		d=g_copys.m_arScales[i]*1;
		float d1=g_toy.m_pos.x-g_copys.m_arPos[i].x;if(d1<0) d1=-d1; d1*=d1;
		float d2=g_toy.m_pos.y-g_copys.m_arPos[i].y;if(d2<0) d2=-d2; d2*=d2;
		float d3=g_toy.m_pos.z-g_copys.m_arPos[i].z;if(d3<0) d3=-d3; d3*=d3;
		float d4=d1+d2+d3;
		if(d4<w*w||d4<d*d)
		{
			ROSEDump_OutputWindow("XXX!\n");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL WeaponXXX()
{
	//�ж�Weapon���������ײ״̬
	
	for(int i=0;i<ROSE_COPYS_NUM;i++)
	{
		//���ж������Ƿ���ǰ��������
		D3DXVECTOR3 vecActorObject;//weapon��Object������
		vecActorObject=g_copys.m_arPos[i]-g_posWeapon;
		float f=D3DXVec3Dot(&g_toy.m_vecWeaponVel,&vecActorObject);
		//if(f<0)
		//	continue;

		float w,h,d;
		w=g_copys.m_arScales[i]*1;
		h=g_copys.m_arScales[i]*6;
		d=g_copys.m_arScales[i]*1;
		float d1=g_posWeapon.x-g_copys.m_arPos[i].x;if(d1<0) d1=-d1; d1*=d1;
		//float d2=posWeapon.y-g_copys.m_arPos[i].y;if(d2<0) d2=-d2; d2*=d2;
		float d2=0;
		float d3=g_posWeapon.z-g_copys.m_arPos[i].z;if(d3<0) d3=-d3; d3*=d3;
		float d4=d1+d2+d3;
		d4=sqrt(d4)-g_toy.m_fWeaponScale;
		if(d4<w||d4<d)
		{
			ROSEDump_OutputWindow("YYY!\n");
			g_blast.SetParticleOrg(&g_posWeapon);
			g_blast.SetColor(&g_copys.m_arDiffuse[i]);
			g_blast.ResetParticleBuffer();
			return FALSE;
		}
	}
	return TRUE;
}