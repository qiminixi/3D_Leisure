#include <d3dx9.h>
#include "../ROSE/d3dUtilities.h"
#include "toy.h"

struct VertexToy
{
	D3DXVECTOR3 _pos;
	D3DXVECTOR3 _norm;
	float _u,_v;
	static const DWORD FVF;
	
	VertexToy(D3DXVECTOR3 *pPos,D3DXVECTOR3 *pNorm,float u,float v)
	{
		_pos=*pPos;
		_norm=*pNorm;
		_u=u;
		_v=v;                                                                                                                                                    
	}
};
const DWORD VertexToy::FVF=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;

CToy::CToy()
{
	m_thirdhand=NULL;//第三只手
	m_weapon=NULL;//武器
	m_nCurFrame=0;
	ZeroMemory(&m_move,sizeof(m_move));
	
	m_weapon=NULL;
	m_fAngelThirdHand=0.0f;
	m_fAngelWeapon=0.0f;
}

CToy::~CToy()
{
	//释放内存
}

void CToy::Init(IDirect3DDevice9 *pDevice)
{
	VertexToy toyvertices[24]={
		VertexToy(&D3DXVECTOR3(-1.000000,-1.000000,-1.000000),&D3DXVECTOR3(-1.000000,0.000000,0.000000),1.0,1.0),
		VertexToy(&D3DXVECTOR3(-1.000000,-1.000000,1.000000),&D3DXVECTOR3(-1.000000,0.000000,0.000000),0.0,1.0),
		VertexToy(&D3DXVECTOR3(-1.000000,1.000000,1.000000),&D3DXVECTOR3(-1.000000,0.000000,0.000000),0.0,0.0),
		VertexToy(&D3DXVECTOR3(-1.000000,1.000000,-1.000000),&D3DXVECTOR3(-1.000000,0.000000,0.000000),1.0,0.0),
		
		VertexToy(&D3DXVECTOR3(-1.000000,1.000000,-1.000000),&D3DXVECTOR3(0.000000,1.000000,0.000000),0.0,1.0),
		VertexToy(&D3DXVECTOR3(-1.000000,1.000000,1.000000),&D3DXVECTOR3(0.000000,1.000000,0.000000),0.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,1.000000,1.000000),&D3DXVECTOR3(0.000000,1.000000,0.000000),1.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,1.000000,-1.000000),&D3DXVECTOR3(0.000000,1.000000,0.000000),1.0,1.0),
		
		VertexToy(&D3DXVECTOR3(1.000000,1.000000,-1.000000),&D3DXVECTOR3(1.000000,0.000000,0.000000),0.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,1.000000,1.000000),&D3DXVECTOR3(1.000000,0.000000,0.000000),1.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,-1.000000,1.000000),&D3DXVECTOR3(1.000000,0.000000,0.000000),1.0,1.0),
		VertexToy(&D3DXVECTOR3(1.000000,-1.000000,-1.000000),&D3DXVECTOR3(1.000000,0.000000,0.000000),0.0,1.0),
		
		VertexToy(&D3DXVECTOR3(-1.000000,-1.000000,1.000000),&D3DXVECTOR3(0.000000,-1.000000,0.000000),0.0,1.0),
		VertexToy(&D3DXVECTOR3(-1.000000,-1.000000,-1.000000),&D3DXVECTOR3(0.000000,-1.000000,0.000000),0.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,-1.000000,-1.000000),&D3DXVECTOR3(0.000000,-1.000000,0.000000),1.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,-1.000000,1.000000),&D3DXVECTOR3(0.000000,-1.000000,0.000000),1.0,1.0),
		
		VertexToy(&D3DXVECTOR3(-1.000000,-1.000000,1.000000),&D3DXVECTOR3(0.000000,0.000000,1.000000),1.0,1.0),
		VertexToy(&D3DXVECTOR3(1.000000,-1.000000,1.000000),&D3DXVECTOR3(0.000000,0.000000,1.000000),0.0,1.0),
		VertexToy(&D3DXVECTOR3(1.000000,1.000000,1.000000),&D3DXVECTOR3(0.000000,0.000000,1.000000),0.0,0.0),
		VertexToy(&D3DXVECTOR3(-1.000000,1.000000,1.000000),&D3DXVECTOR3(0.000000,0.000000,1.000000),1.0,0.0),
		
		VertexToy(&D3DXVECTOR3(-1.000000,-1.000000,-1.000000),&D3DXVECTOR3(0.000000,0.000000,-1.000000),0.0,1.0),
		VertexToy(&D3DXVECTOR3(-1.000000,1.000000,-1.000000),&D3DXVECTOR3(0.000000,0.000000,-1.000000),0.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,1.000000,-1.000000),&D3DXVECTOR3(0.000000,0.000000,-1.000000),1.0,0.0),
		VertexToy(&D3DXVECTOR3(1.000000,-1.000000,-1.000000),&D3DXVECTOR3(0.000000,0.000000,-1.000000),1.0,1.0)
	};
	WORD toyindices[36]={
		0,1,2,
		2,3,0,
		4,5,6,
		6,7,4,
		8,9,10,
		10,11,8,
		12,13,14,
		14,15,12,
		16,17,18,
		18,19,16,
		20,21,22,
		22,23,20
	};
	
	//创建，24，18
	//每一帧都是6*24的顶点数
	
	pDevice->CreateVertexBuffer(6*24*32,D3DUSAGE_WRITEONLY,0x112,D3DPOOL_MANAGED,&m_move[0].m_pVB,NULL);
	pDevice->CreateVertexBuffer(6*24*32,D3DUSAGE_WRITEONLY,0x112,D3DPOOL_MANAGED,&m_move[1].m_pVB,NULL);
	pDevice->CreateVertexBuffer(6*24*32,D3DUSAGE_WRITEONLY,0x112,D3DPOOL_MANAGED,&m_move[2].m_pVB,NULL);
	pDevice->CreateIndexBuffer(6*36*2,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_move[0].m_pIB,NULL);
	pDevice->CreateIndexBuffer(6*36*2,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_move[1].m_pIB,NULL);
	pDevice->CreateIndexBuffer(6*36*2,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_move[2].m_pIB,NULL);
	m_move[0].m_FVF=m_move[1].m_FVF=m_move[2].m_FVF=18;
	m_move[0].m_nNumVertices=m_move[1].m_nNumVertices=m_move[2].m_nNumVertices=6*24;
	m_move[0].m_nNumFaces=m_move[1].m_nNumFaces=m_move[2].m_nNumFaces=6*12;
	
	ID3DXMesh *pMesh=NULL;
	IDirect3DVertexBuffer9 *pVB_Mesh=NULL;
	D3DXCreateBox(pDevice,2,2,2,&pMesh,NULL);
	byte *buffer_mesh=NULL;
	byte *buffer_mesh_temp=NULL;
	UINT nStride=32;//pMesh->GetNumBytesPerVertex();
	UINT FVF=pMesh->GetFVF();
	UINT nNumVertices=pMesh->GetNumVertices();
	D3DXVECTOR3 *pVec=NULL;
	
	
	//先复制索引，三个帧都使用相同的索引
	UINT nNumFaces=pMesh->GetNumFaces();
	WORD *indices_mesh=toyindices;
	WORD *indices_toy0=NULL;
	WORD *indices_toy1=NULL;
	WORD *indices_toy2=NULL;
	m_move[0].m_pIB->Lock(0,0,(void**)&indices_toy0,0);
	m_move[1].m_pIB->Lock(0,0,(void**)&indices_toy1,0);
	m_move[2].m_pIB->Lock(0,0,(void**)&indices_toy2,0);
	int t=0;
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<nNumFaces*3;j++)//
		{
			*indices_toy0=*indices_toy1=*indices_toy2=indices_mesh[j]+i*nNumVertices;//顶点在数组中的位置不一样了，为0~6*24
			indices_toy0++;
			indices_toy1++;
			indices_toy2++;
			t++;
		}
	}
	m_move[0].m_pIB->Unlock();
	m_move[1].m_pIB->Unlock();
	m_move[2].m_pIB->Unlock();
	
	//生成各个部位
	const float OY=1.54f;//将物体上移使脚在y=0平面上
	VertexToy *pVert_toyM=NULL;
	VertexToy *pVert_toy0=NULL;//第一帧
	VertexToy *pVert_toy1=NULL;//第二帧，在一帧数据的基础上变换得到
	VertexToy *pVert_toy2=NULL;//第三帧，在一帧数据的基础上变换得到
	m_move[0].m_pVB->Lock(0,0,(void**)&pVert_toy0,0);
	m_move[1].m_pVB->Lock(0,0,(void**)&pVert_toy1,0);
	m_move[2].m_pVB->Lock(0,0,(void**)&pVert_toy2,0);
	
	//head
	pVert_toyM=toyvertices;
	for(int i=0;i<nNumVertices;i++)
	{
		*pVert_toy0=*pVert_toyM;
		pVert_toy0->_pos.x*=0.2;
		pVert_toy0->_pos.y*=0.2;
		pVert_toy0->_pos.z*=0.1;
		pVert_toy0->_pos+=D3DXVECTOR3(0,1.5+OY,0);
		*pVert_toy1=*pVert_toy2=*pVert_toy0;//另外两帧直接复制第一帧的数据即可
		
		pVert_toyM++;
		pVert_toy0++;
		pVert_toy1++;
		pVert_toy2++;
	}
	
	//body
	pVert_toyM=toyvertices;
	for(int i=0;i<nNumVertices;i++)
	{
		*pVert_toy0=*pVert_toyM;
		pVert_toy0->_pos.x*=0.35;
		pVert_toy0->_pos.y*=0.55;
		pVert_toy0->_pos.z*=0.20;
		pVert_toy0->_pos+=D3DXVECTOR3(0,0.68+OY,0);
		*pVert_toy1=*pVert_toy2=*pVert_toy0;//另外两帧直接复制第一帧的数据即可

		pVert_toyM++;
		pVert_toy0++;
		pVert_toy1++;
		pVert_toy2++;
	}
	
	//handle_left
	pVert_toyM=toyvertices;
	for(int i=0;i<nNumVertices;i++)
	{
		const float angel=-3.1415926f/180*30;//绕x轴的旋转角度
		
		*pVert_toy0=*pVert_toyM;
		pVert_toy0->_pos.x*=0.1;
		pVert_toy0->_pos.y*=0.6;
		pVert_toy0->_pos.z*=0.1;
		*pVert_toy1=*pVert_toy2=*pVert_toy0;//另外两帧直接复制第一帧的数据即可
		
		ROSERotatePointX(&pVert_toy1->_norm, angel);//第二帧顶点的法向量
		ROSERotatePointX(&pVert_toy2->_norm,-angel);//第三帧顶点的法向量
		ROSERotatePointX(&pVert_toy1->_pos, angel);//第二帧左手伸出
		ROSERotatePointX(&pVert_toy2->_pos,-angel);//第三帧左手后撤
		
		pVert_toy0->_pos+=D3DXVECTOR3(-0.51,0.5+OY,0);
		pVert_toy1->_pos+=D3DXVECTOR3(-0.51,0.65+OY,0.28f);
		pVert_toy2->_pos+=D3DXVECTOR3(-0.51,0.65+OY,-0.28f);

		pVert_toyM++;
		pVert_toy0++;
		pVert_toy1++;
		pVert_toy2++;
	}
	
	//handle_right
	pVert_toyM=toyvertices;
	for(int i=0;i<nNumVertices;i++)
	{
		const float angel=-3.1415926f/180*30;//绕x轴的旋转角度
		
		*pVert_toy0=*pVert_toyM;
		pVert_toy0->_pos.x*=0.1;
		pVert_toy0->_pos.y*=0.6;
		pVert_toy0->_pos.z*=0.1;
		*pVert_toy1=*pVert_toy2=*pVert_toy0;//另外两帧直接复制第一帧的数据即可

		ROSERotatePointX(&pVert_toy1->_norm,-angel);//第二帧顶点的法向量
		ROSERotatePointX(&pVert_toy2->_norm, angel);//第三帧顶点的法向量
		ROSERotatePointX(&pVert_toy1->_pos,-angel);//第二帧右手后撤
		ROSERotatePointX(&pVert_toy2->_pos, angel);//第三帧右手伸出

		pVert_toy0->_pos+=D3DXVECTOR3(0.51,0.5+OY,0);
		pVert_toy1->_pos+=D3DXVECTOR3(0.51,0.65+OY,-0.28f);
		pVert_toy2->_pos+=D3DXVECTOR3(0.51,0.65+OY, 0.28f);

		pVert_toyM++;
		pVert_toy0++;
		pVert_toy1++;
		pVert_toy2++;
	}
	
	//leg_left
	pVert_toyM=toyvertices;
	for(int i=0;i<nNumVertices;i++)
	{
		const float angel=-3.1415926f/180*20;//绕x轴的旋转角度

		*pVert_toy0=*pVert_toyM;
		pVert_toy0->_pos.x*=0.14;
		pVert_toy0->_pos.y*=0.80;
		pVert_toy0->_pos.z*=0.14;
		*pVert_toy1=*pVert_toy2=*pVert_toy0;//另外两帧直接复制第一帧的数据即可

		ROSERotatePointX(&pVert_toy1->_norm,-angel);//第二帧顶点的法向量
		ROSERotatePointX(&pVert_toy2->_norm, angel);//第三帧顶点的法向量
		ROSERotatePointX(&pVert_toy1->_pos,-angel);//第二帧左腿后撤
		ROSERotatePointX(&pVert_toy2->_pos, angel);//第三帧左腿迈出

		pVert_toy0->_pos+=D3DXVECTOR3(-0.21,-0.74+OY,0);
		pVert_toy1->_pos+=D3DXVECTOR3(-0.21,-0.74+OY,-0.40f);
		pVert_toy2->_pos+=D3DXVECTOR3(-0.21,-0.74+OY, 0.40f);

		pVert_toyM++;
		pVert_toy0++;
		pVert_toy1++;
		pVert_toy2++;
	}
	
	//leg_right
	pVert_toyM=toyvertices;
	for(int i=0;i<nNumVertices;i++)
	{
		const float angel=-3.1415926f/180*20;//绕x轴的旋转角度

		*pVert_toy0=*pVert_toyM;
		pVert_toy0->_pos.x*=0.14;
		pVert_toy0->_pos.y*=0.80;
		pVert_toy0->_pos.z*=0.14;
		*pVert_toy1=*pVert_toy2=*pVert_toy0;//另外两帧直接复制第一帧的数据即可

		ROSERotatePointX(&pVert_toy1->_norm, angel);//第二帧顶点的法向量
		ROSERotatePointX(&pVert_toy2->_norm,-angel);//第三帧顶点的法向量
		ROSERotatePointX(&pVert_toy1->_pos, angel);//第二帧右腿迈出
		ROSERotatePointX(&pVert_toy2->_pos,-angel);//第三帧右腿后撤

		pVert_toy0->_pos+=D3DXVECTOR3(0.21,-0.74+OY,0);
		pVert_toy1->_pos+=D3DXVECTOR3(0.21,-0.74+OY, 0.40f);
		pVert_toy2->_pos+=D3DXVECTOR3(0.21,-0.74+OY,-0.40f);

		pVert_toyM++;
		pVert_toy0++;
		pVert_toy1++;
		pVert_toy2++;
	}
	
	m_move[0].m_pVB->Unlock();
	m_move[1].m_pVB->Unlock();
	m_move[2].m_pVB->Unlock();
	
	//第三只手
	VertexToy *pVertMesh=NULL;
	D3DXCreateMeshFVF(12,24,D3DXMESH_WRITEONLY|D3DXMESH_MANAGED,VertexToy::FVF,pDevice,&m_thirdhand);
	pVB_Mesh=NULL;
	m_thirdhand->GetVertexBuffer(&pVB_Mesh);
	pVB_Mesh->Lock(0,0,(void**)&pVertMesh,0);
	
	pVert_toyM=toyvertices;
	for(int i=0;i<nNumVertices;i++)
	{
		*pVertMesh=*pVert_toyM;
		pVertMesh->_pos.y+=1.0f;
		pVertMesh->_pos.x*=0.1;
		pVertMesh->_pos.y*=0.6;
		pVertMesh->_pos.z*=0.1;
		
		pVertMesh++;
		pVert_toyM++;
	}
	pVB_Mesh->Unlock();
	
	IDirect3DIndexBuffer9 *pIB_Mesh=NULL;
	WORD *indices_thirdhand=NULL;
	m_thirdhand->GetIndexBuffer(&pIB_Mesh);
	pIB_Mesh->Lock(0,0,(void**)&indices_thirdhand,0);
	memcpy(indices_thirdhand,toyindices,sizeof(toyindices));
	pIB_Mesh->Unlock();
	
	//武器
	pVertMesh=NULL;
	D3DXCreateMeshFVF(12,24,D3DXMESH_WRITEONLY|D3DXMESH_MANAGED,VertexToy::FVF,pDevice,&m_weapon);
	pVB_Mesh=NULL;
	m_weapon->GetVertexBuffer(&pVB_Mesh);
	pVB_Mesh->Lock(0,0,(void**)&pVertMesh,0);
	//memcpy(pVertMesh,toyindices,sizeof(toyvertices));
	pVert_toyM=toyvertices;
	for(int i=0;i<24.0;i++)
	{
		*pVertMesh=*pVert_toyM;

		pVertMesh++;
		pVert_toyM++;
	}
	pVB_Mesh->Unlock();

	pIB_Mesh=NULL;
	indices_thirdhand=NULL;
	m_weapon->GetIndexBuffer(&pIB_Mesh);
	pIB_Mesh->Lock(0,0,(void**)&indices_thirdhand,0);
	memcpy(indices_thirdhand,toyindices,sizeof(toyindices));
	pIB_Mesh->Unlock();
	
	m_posWeaponOrg=D3DXVECTOR3(0.51f,4.40f,0.0f);
	m_posWeapon=D3DXVECTOR3(0.51f,4.40f,0.0f);
	m_vecWeaponAcc=D3DXVECTOR3(0.0f,-1.0f,0.0f);
	m_vecWeaponVel=D3DXVECTOR3(0.0f,3.0f,3.0f);
	m_fWeaponScale=0.4f;
	
	m_pTexture=NULL;
	D3DXCreateTextureFromFile(pDevice,"res\\1.jpg",&m_pTexture);
}

void CToy::Render(IDirect3DDevice9 *pDevice)
{
	if(m_pTexture) pDevice->SetTexture(0,m_pTexture);
	pDevice->SetStreamSource(0,m_move[m_nCurFrame].m_pVB,0,sizeof(VertexToy));
	pDevice->SetFVF(VertexToy::FVF);
	pDevice->SetIndices(m_move[m_nCurFrame].m_pIB);
	
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		m_move[m_nCurFrame].m_nNumVertices,
		0,
		m_move[m_nCurFrame].m_nNumFaces);
	if(m_pTexture) pDevice->SetTexture(0,NULL);
}

void CToy::UpdatePos()
{
	m_pos=pCamera->m_vecPos;
	m_pos.z-=0.3*pCamera->m_vecLook.z;
	m_pos.x-=0.3*pCamera->m_vecLook.x;
	m_pos.y-=pCamera->GetHeight();
}