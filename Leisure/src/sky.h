#ifndef ROSE_SKY_H
#define ROSE_SKY_H

class ROSECSky
{
public:
	//ID3DXMesh *m_pMeshSky;
	IDirect3DVertexBuffer9 *m_pVB;
	IDirect3DIndexBuffer9 *m_pIB;
	IDirect3DTexture9 *m_pTexture;
	D3DXVECTOR3 m_pos;
	UINT m_nNumVertices;
	UINT m_nNumFaces;
	void Init(IDirect3DDevice9 *pDevice);
	void Render(IDirect3DDevice9 *pDevice);


};

#endif