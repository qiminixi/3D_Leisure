#ifndef D3DU_SCENES_H
#define D3DU_SCENES_H

namespace d3du
{

class CylinderScene
{
public:
	CylinderScene();
	CylinderScene(IDirect3DDevice9 *pDevice);
	~CylinderScene();

	void Init(
		IDirect3DDevice9 *pDevice=NULL,
		float fRadius1=0.3f,
		float fRadius2=0.3f,
		float fLength=2.0f,
		UINT nSlices=10,
		UINT nStacks=10);
	void Display();
	void SetTexture(IDirect3DTexture9 *pTex);

private:
	IDirect3DDevice9 *m_pDevice;
	IDirect3DTexture9 *m_pTex;
	ID3DXMesh *m_pMesh;
	float m_fRadius1;//z-
	float m_fRadius2;//z+
	float m_fLength;
	UINT m_nSlices;
	UINT m_nStacks;
};


class AxesScene
{
public:
	AxesScene();
	~AxesScene();
	
	void Init(IDirect3DDevice9 *pDevice);
	void Display();
	
private:
	IDirect3DDevice9 *m_pDevice;
	UINT m_nXLength;
	UINT m_nYLength;
	UINT m_nZLength;
	
	IDirect3DVertexBuffer9 *m_pVB;
};


}
#endif

