#ifndef D3DU_TERRAIN_H
#define D3DU_TERRAIN_H

#include <vector>
#include "Camera.h"

//暂时没有使用法向量
class ROSECTerrain
{
protected:
	//地形网格顶点
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
	//构造函数，析构函数
	ROSECTerrain();
	~ROSECTerrain();
	virtual void Init(IDirect3DDevice9 *pDevice,
		ROSEPOINTF ptPos,
		int nNumVerticesPerRow,
		int nNumVerticesPerCol,
		float fCellSpacing,
		float fHeightScale,
		LPCTSTR lpszFileName);//初始化
	
public:
	void ReadRawFile(LPCTSTR lpszFileName);//读取高度图
	virtual void GenerateVertices();//生成网格顶点
	void GenerateIndices();//生成三角形索引
	void GenerateTexture();//生成纹理
public:
	void Display();
	float GetHeight(float x,float z);
	void ComputeShade(D3DXVECTOR3 *directionToLight);//光照计算
	void SetTexture(IDirect3DTexture9 *pTex){m_pTex=pTex;}
protected:
	float ComputeShadeOfCell(int row,int col,D3DXVECTOR3 *directionToLight);
	
public:
	ROSECamera *m_pCamera;//追踪相机位置
	float m_fViewRadius;//视野半径，实际上它应该随高度变化而变化
	UINT m_nViewRadius;
	IDirect3DVertexBuffer9 *m_pVBView;
	IDirect3DIndexBuffer9 *m_pIBView;
	
protected:
	//对外参数
	IDirect3DDevice9 *m_pDevice;
	int m_nNumVerticesPerRow;//一行的顶点数，则网格的列数为m_nNumVerticesPerRow-1
	int m_nNumVerticesPerCol;//一列的顶点数，则网格的行数为m_nNumVerticesPerCol-1
	float m_fCellSpacing;//单元格宽度
	float m_fHeightScale;//高度比例
	ROSEPOINTF m_ptPos;//位置
	
	//内部工作用
	IDirect3DVertexBuffer9 *m_pVB;
	IDirect3DIndexBuffer9 *m_pIB;
	IDirect3DTexture9 *m_pTex;
	std::vector<int> m_heightmap;
	UINT m_nNumVertices;//总的顶点数，省得diplay的时候算
	UINT m_nNumTriangles;//总的三角形单元数，省得diplay的时候算
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
	virtual void GenerateVertices();//生成网格顶点
};



#endif