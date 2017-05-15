#ifndef D3DU_TERRAIN_H
#define D3DU_TERRAIN_H

#include <vector>
#include "Camera.h"

//��ʱû��ʹ�÷�����
class ROSECTerrain
{
protected:
	//�������񶥵�
	struct TerrainVertex
	{
		float m_x,m_y,m_z;
		float m_u,m_v;
		static const DWORD FVF;
		TerrainVertex();
		TerrainVertex(float x,float y,float z,float u,float v)
		{
			m_x=x;m_y=y;m_z=z;m_u=u;m_v=v;
		}
	};

public:
	//���캯������������
	ROSECTerrain();
	~ROSECTerrain();
	virtual void Init(IDirect3DDevice9 *pDevice,
		ROSEPOINTF ptPos,
		int nNumVerticesPerRow,
		int nNumVerticesPerCol,
		float fCellSpacing,
		float fHeightScale,
		LPCTSTR lpszFileName);//��ʼ��
	
public:
	void ReadRawFile(LPCTSTR lpszFileName);//��ȡ�߶�ͼ
	virtual void GenerateVertices();//�������񶥵�
	void GenerateIndices();//��������������
	void GenerateTexture();//��������
public:
	void Display();
	float GetHeight(float x,float z);
	void ComputeShade(D3DXVECTOR3 *directionToLight);//���ռ���
	void SetTexture(IDirect3DTexture9 *pTex){m_pTex=pTex;}
protected:
	float ComputeShadeOfCell(int row,int col,D3DXVECTOR3 *directionToLight);
	
public:
	ROSECamera *m_pCamera;//׷�����λ��
	float m_fViewRadius;//��Ұ�뾶��ʵ������Ӧ����߶ȱ仯���仯
	UINT m_nViewRadius;
	IDirect3DVertexBuffer9 *m_pVBView;
	IDirect3DIndexBuffer9 *m_pIBView;
	
protected:
	//�������
	IDirect3DDevice9 *m_pDevice;
	int m_nNumVerticesPerRow;//һ�еĶ������������������Ϊm_nNumVerticesPerRow-1
	int m_nNumVerticesPerCol;//һ�еĶ������������������Ϊm_nNumVerticesPerCol-1
	float m_fCellSpacing;//��Ԫ����
	float m_fHeightScale;//�߶ȱ���
	ROSEPOINTF m_ptPos;//λ��
	
	//�ڲ�������
	IDirect3DVertexBuffer9 *m_pVB;
	IDirect3DIndexBuffer9 *m_pIB;
	IDirect3DTexture9 *m_pTex;
	std::vector<int> m_heightmap;
	UINT m_nNumVertices;//�ܵĶ�������ʡ��diplay��ʱ����
	UINT m_nNumTriangles;//�ܵ������ε�Ԫ����ʡ��diplay��ʱ����
};

class CFlatTerrain :public ROSECTerrain
{
public:
	virtual void Init(
		IDirect3DDevice9 *pDevice, 
		ROSEPOINTF ptPos, 
		int nNumVerticesPerRow, 
		int nNumVerticesPerCol, 
		float fCellSpacing, 
		float fHeightScale, 
		LPCTSTR lpszFileName);
	virtual void GenerateVertices();//�������񶥵�
};



#endif