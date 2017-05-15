#ifndef ROSE_TOY_H
#define ROSE_TOY_H

#include "../ROSE/Camera.h"//actor需要关联相机，因为位置来自于camera

/*
//日后增加
class LOSTCObject
{
	//mesh
	//iframe
	//....
};

class LOSTActor
{
	//object
	//camera
};
*/

struct SToy_Frame
{
	IDirect3DVertexBuffer9 *m_pVB;//共6个长方体
	IDirect3DIndexBuffer9 *m_pIB;//共6个长方体
	UINT m_nNumVertices;
	UINT m_nNumFaces;
	DWORD m_FVF;
};

class CToy
{
public:
	SToy_Frame m_move[3];//0=站立，1/2=移动
	D3DXVECTOR3 m_pos;//所处位置
	UINT m_nCurFrame;
	
	ID3DXMesh *m_thirdhand;//第三只手，底部中心位于0,0,0处
	ID3DXMesh *m_weapon;//武器
	D3DXVECTOR3 m_posThirdHand;//第三只手的位置
	float m_fAngelThirdHand;//第三只手的运动角度，y+向z+为正，y+为0度
	
	D3DXVECTOR3 m_posWeapon;//武器位置
	D3DXVECTOR3 m_posWeaponOrg;//武器位置
	D3DXVECTOR3 m_vecWeaponAcc;//武器加速度
	D3DXVECTOR3 m_vecWeaponVel;//武器速度
	float m_fWeaponScale;//武器大小
	float m_fAngelWeapon;//武器的运动角度
	
	ROSECamera *pCamera;//相机
	IDirect3DTexture9 *m_pTexture;
	
	
	CToy();
	~CToy();
	void Init(IDirect3DDevice9 *pDevice);
	void Render(IDirect3DDevice9 *pDevice);
	void UpdatePos();
};

#endif