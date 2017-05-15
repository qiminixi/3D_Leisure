#include <d3dx9.h>
#include "../ROSE/d3dUtilities.h"
#include "sky.h"

void ROSECSky::Init(IDirect3DDevice9 *pDevice)
{
	ID3DXMesh *pMesh;
	IDirect3DTexture9 *pTexture;
	int nStride;
	float r=400.0f;
	D3DXCreateSphere(pDevice,r,30,30,&pMesh,NULL);
	nStride=pMesh->GetNumBytesPerVertex();
	m_nNumVertices=pMesh->GetNumVertices();
	m_nNumFaces=pMesh->GetNumFaces();
	pDevice->CreateVertexBuffer(m_nNumVertices*sizeof(ROSEVertTexture),0,ROSEVertTexture::FVF,
		D3DPOOL_MANAGED,&m_pVB,NULL);
	pDevice->CreateIndexBuffer(m_nNumFaces*3*sizeof(WORD),D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIB,NULL);

	/*
	ROSEVertTexture *vertices;
	m_pVB->Lock(0,0,(void**)&vertices,0);
	vertices[0]=ROSEVertTexture(-1.0f, 1.0f,1.0f,0,0);
	vertices[1]=ROSEVertTexture( 1.0f, 1.0f,1.0f,1,0);
	vertices[2]=ROSEVertTexture(-1.0f,-1.0f,1.0f,0,1);
	m_pVB->Unlock();
	*/


	//提取顶点坐标并设置纹理坐标
	IDirect3DVertexBuffer9 *pMeshVB;
	byte *buffer=NULL;
	ROSEVertTexture *pVertices=NULL;
	pMesh->GetVertexBuffer(&pMeshVB);
	pMeshVB->Lock(0,0,(void**)&buffer,0);
	m_pVB->Lock(0,0,(void**)&pVertices,0);
	for(int i=0;i<m_nNumVertices;i++)
	{
		D3DXVECTOR3 *pVert;
		pVert=(D3DXVECTOR3*)buffer;
		pVertices[i]._x=pVert->x;
		pVertices[i]._y=pVert->y;
		pVertices[i]._z=pVert->z;
		pVertices[i]._u=pVert->x/r/2+0.5;
		pVertices[i]._v=pVert->z/r/2+0.5;
		buffer+=nStride;
	}
	pMeshVB->Unlock();
	m_pVB->Unlock();


	//复制索引
	IDirect3DIndexBuffer9 *pMeshIB=NULL;
	byte *pIndicesMesh=NULL;
	byte *pIndices=NULL;
	pMesh->GetIndexBuffer(&pMeshIB);
	pMeshIB->Lock(0,0,(void**)&pIndicesMesh,0);
	m_pIB->Lock(0,0,(void**)&pIndices,0);
	memcpy(pIndices,pIndicesMesh,m_nNumFaces*3*sizeof(WORD));
	pMeshIB->Unlock();
	m_pIB->Unlock();

	pTexture=NULL;
	D3DXCreateTextureFromFile(pDevice,TEXT("res\\sky.jpg"),&pTexture);
	m_pTexture=pTexture;
	m_pos=D3DXVECTOR3(0,0,0);
	pMesh->Release();
}

void ROSECSky::Render(IDirect3DDevice9 *pDevice)
{
	//if(m_pTexture) 
	pDevice->SetTexture(0,m_pTexture);
	pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	pDevice->SetStreamSource(0,m_pVB,0,sizeof(ROSEVertTexture));
	pDevice->SetFVF(ROSEVertTexture::FVF);
	pDevice->SetIndices(m_pIB);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nNumVertices,0,m_nNumFaces);
	//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);
	if(m_pTexture) pDevice->SetTexture(0,NULL);
}
