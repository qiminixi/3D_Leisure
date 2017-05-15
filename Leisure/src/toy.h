#ifndef ROSE_TOY_H
#define ROSE_TOY_H

#include "../ROSE/Camera.h"//actor��Ҫ�����������Ϊλ��������camera

/*
//�պ�����
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
	IDirect3DVertexBuffer9 *m_pVB;//��6��������
	IDirect3DIndexBuffer9 *m_pIB;//��6��������
	UINT m_nNumVertices;
	UINT m_nNumFaces;
	DWORD m_FVF;
};

class CToy
{
public:
	SToy_Frame m_move[3];//0=վ����1/2=�ƶ�
	D3DXVECTOR3 m_pos;//����λ��
	UINT m_nCurFrame;
	
	ID3DXMesh *m_thirdhand;//����ֻ�֣��ײ�����λ��0,0,0��
	ID3DXMesh *m_weapon;//����
	D3DXVECTOR3 m_posThirdHand;//����ֻ�ֵ�λ��
	float m_fAngelThirdHand;//����ֻ�ֵ��˶��Ƕȣ�y+��z+Ϊ����y+Ϊ0��
	
	D3DXVECTOR3 m_posWeapon;//����λ��
	D3DXVECTOR3 m_posWeaponOrg;//����λ��
	D3DXVECTOR3 m_vecWeaponAcc;//�������ٶ�
	D3DXVECTOR3 m_vecWeaponVel;//�����ٶ�
	float m_fWeaponScale;//������С
	float m_fAngelWeapon;//�������˶��Ƕ�
	
	ROSECamera *pCamera;//���
	IDirect3DTexture9 *m_pTexture;
	
	
	CToy();
	~CToy();
	void Init(IDirect3DDevice9 *pDevice);
	void Render(IDirect3DDevice9 *pDevice);
	void UpdatePos();
};

#endif