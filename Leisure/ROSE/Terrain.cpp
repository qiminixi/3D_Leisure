#include "PreHeader.h"
#include "terrain.h"
#include "DebugHelper.h"
#include <fstream>

#define GetVertexIndex(i,j) ((i)*m_nNumVerticesPerRow+j) //根据顶点的行列坐标计算顶点在缓存中的索引

ROSECTerrain::ROSECTerrain()
{
}

ROSECTerrain::~ROSECTerrain()
{
	if(m_pIB) m_pIB->Release();
	if(m_pVB) m_pVB->Release();
	if(m_pTex) m_pTex->Release();
	if(m_pVBView) m_pVBView->Release();
	if(m_pIBView) m_pIBView->Release();
}
const DWORD ROSECTerrain::TerrainVertex::FVF=D3DFVF_XYZ|D3DFVF_TEX1;

//初始化
//nNumVerticesInRow：一行的顶点数
//nNumVerticesInCol：一列的顶点数
//fCellSpacing：单元格的边长
void ROSECTerrain::Init(
				   IDirect3DDevice9 *pDevice,
				   ROSEPOINTF ptPos,
				   int nNumVerticesInRow,
				   int nNumVerticesInCol,
				   float fCellSpacing,
				   float fHeightScale,
				   LPCTSTR lpszFileName)
{
	m_pDevice=pDevice;
	m_ptPos=ptPos;
	m_nNumVerticesPerRow=nNumVerticesInRow;
	m_nNumVerticesPerCol=nNumVerticesInCol;
	m_fCellSpacing=fCellSpacing;
	m_fHeightScale=fHeightScale;
	
	m_nNumVertices=m_nNumVerticesPerRow*m_nNumVerticesPerCol;
	m_nNumTriangles=(m_nNumVertices-m_nNumVerticesPerCol-m_nNumVerticesPerRow+1)*2;
	m_pVBView=NULL;
	m_pIBView=NULL;
	
	ReadRawFile(lpszFileName);//读取高度图
	GenerateVertices();//生成网格顶点
	GenerateIndices();//生成三角形索引
}

//读取高度图
void ROSECTerrain::ReadRawFile(LPCTSTR lpszFileName)
{
	std::ifstream infile;
	infile.open(lpszFileName,std::ios::in|std::ios::binary);
	if(!infile) return;
	infile.seekg(0,SEEK_END);
	UINT nFileSize=infile.tellg();//文件大小
	unsigned char *buffer=new unsigned char[nFileSize];
	infile.seekg(0,SEEK_SET);
	infile.read((char*)buffer,nFileSize);//读取文件
	infile.close();
	m_heightmap.resize(nFileSize);
	for(UINT i=0;i<nFileSize;i++)
	{
		m_heightmap[i]=(int)buffer[i]-93;//128;
	}
	
	delete buffer;
}

//生成网格顶点
void ROSECTerrain::GenerateVertices()
{
	FILE *fp;
	fp=fopen("uv.txt","w");
	//纹理的(0,0)与地形网格的左上角重合，(1,1)与右下角重合
	m_pDevice->CreateVertexBuffer(
		m_nNumVertices*sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,NULL);
	float fDu=1.0f/m_nNumVerticesPerRow,fDv=1.0f/m_nNumVerticesPerCol;//以地形网格左上角的顶点作为原点，相邻格子的纹理坐标在x和y方向上的坐标差值
	TerrainVertex *vertices=NULL;
	TerrainVertex *pVert=NULL;
	m_pVB->Lock(0,0,(void**)&vertices,0);
	for(int i=0;i<m_nNumVerticesPerCol;i++)//逐行
	{
		int nn=0;
		for(int j=0;j<m_nNumVerticesPerRow;j++)//逐列
		{
			int index=i*m_nNumVerticesPerRow+j;
			pVert=vertices+index;
			pVert->m_x=m_ptPos._x+j*m_fCellSpacing;//X方向是加
			pVert->m_y=(m_heightmap[index])*m_fHeightScale;
			pVert->m_z=m_ptPos._y-i*m_fCellSpacing;//Z方向是减
			pVert->m_u=(float)i;//fDu*i;
			pVert->m_v=(float)j;//fDv*j;
			fprintf(fp,"%3d,%3d|%3.0f,%3.0f  ",i,j,pVert->m_u,pVert->m_v);
			//fprintf(fp,"(%3.0f,%3.0f)  ",pVert->m_x,pVert->m_z);
			nn++;
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	m_pVB->Unlock();
}

//生成三角形单元
void ROSECTerrain::GenerateIndices()
{
	m_pDevice->CreateIndexBuffer(
		m_nNumTriangles*3*sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_pIB,NULL);
	DWORD *indices=NULL;
	m_pIB->Lock(0,0,(void**)&indices,0);
	int baseIndex=0;

	//格子数只有(m_nNumVerticesPerCol-1)*(m_nNumVerticesPerRow-1)
	#define HELPER(i,j) ((i)*m_nNumVerticesPerRow+j) //根据顶点所在行和列坐标计算顶点在数组中的索引
	for(int i=0;i<m_nNumVerticesPerCol-1;i++)//逐行
	{
		for(int j=0;j<m_nNumVerticesPerRow-1;j++)//逐列
		{
			indices[baseIndex]=  HELPER(i,j);
			indices[baseIndex+1]=HELPER(i,j+1);
			indices[baseIndex+2]=HELPER(i+1,j);

			indices[baseIndex+3]=HELPER(i+1,j);
			indices[baseIndex+4]=HELPER(i,j+1);
			indices[baseIndex+5]=HELPER(i+1,j+1);

			baseIndex+=6;
		}
	}
	m_pIB->Unlock();
}

//生成纹理
void ROSECTerrain::GenerateTexture()
{
	FILE *fp;
	fp=fopen("uv_tex.txt","w");
	D3DXCreateTexture(
		m_pDevice,
		m_nNumVerticesPerRow,
		m_nNumVerticesPerCol,
		0,
		0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pTex);
	D3DLOCKED_RECT lockedRect;
	m_pTex->LockRect(0,&lockedRect,NULL,0);
	DWORD *imgData=(DWORD*)lockedRect.pBits;
	
	//用高度255代表0xFFFF FFFF
	for(int i=0;i<m_nNumVerticesPerCol;i++)//逐行
	{
		int h;
		for(int j=0;j<m_nNumVerticesPerRow;j++)//逐列
		{
			//计算颜色
			h=m_heightmap[i*m_nNumVerticesPerRow+j];
			imgData[i*lockedRect.Pitch/4+j]=D3DCOLOR_XRGB(h,h,h);;
			fprintf(fp,"%3d",h);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	m_pTex->UnlockRect(0);
}

void ROSECTerrain::ComputeShade(D3DXVECTOR3 *directionToLight)
{
	if(!m_pTex) return;
	D3DXVec3Normalize(directionToLight,directionToLight);
	
	D3DLOCKED_RECT lockedRect;
	m_pTex->LockRect(0,&lockedRect,NULL,0);
	DWORD *imgData=(DWORD*)lockedRect.pBits;

	for(int i=0;i<m_nNumVerticesPerCol;i++)//逐行
	{
		D3DCOLOR clr;
		UINT r,g,b;
		float f;
		for(int j=0;j<m_nNumVerticesPerRow;j++)//逐列
		{
			//计算颜色
			f=ComputeShadeOfCell(i,j,directionToLight);
			clr=imgData[i*lockedRect.Pitch/4+j];
			r=(clr&0x00ff0000)>>16;r=(UINT)((float)r*f);
			g=(clr&0x0000ff00)>>8;g=(UINT)((float)g*f);
			b=clr&0x000000ff;b=(UINT)((float)b*f);
			clr=D3DCOLOR_XRGB(r,g,b);
			imgData[i*lockedRect.Pitch/4+j]=clr;
		}
	}
	m_pTex->UnlockRect(0);
}

//计算一个格子的光照
//传入的是顶点的行坐标和列坐标
float ROSECTerrain::ComputeShadeOfCell(int row,int col,D3DXVECTOR3 *directionToLight)
{
	//右边和下边上的顶点认为是最后列或者一行的顶点
	if(row==m_nNumVerticesPerCol-1)
		row--;
	if(col==m_nNumVerticesPerRow-1)
		col--;
	//  A   B
	//  * --*
	//  | /
	//  *
	//  C
	float heightA=(float)m_heightmap[GetVertexIndex(row,  col)];
	float heightB=(float)m_heightmap[GetVertexIndex(row,  col+1)];
	float heightC=(float)m_heightmap[GetVertexIndex(row+1,col)];
	
	D3DXVECTOR3 u(m_fCellSpacing,heightB-heightA,0);
	D3DXVECTOR3 v(0,heightC-heightA,-m_fCellSpacing);
	
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n,&u,&v);
	D3DXVec3Normalize(&n,&n);
	
	float cosine=D3DXVec3Dot(&n,directionToLight);
	if(cosine<0) cosine=0;
	return cosine;
}

void ROSECTerrain::Display()
{
	/*m_nViewRadius=(int)m_fViewRadius/m_fCellSpacing;
	
	int l,t,r,b;
	int i=(m_ptPos._y-m_pCamera->m_vecPos.y)/m_fCellSpacing;//相机坐标
	float j=(m_pCamera->m_vecPos.x-m_ptPos._x)/m_fCellSpacing;//相机坐标
	l=i-m_nViewRadius;t=j-m_nViewRadius;r=i+m_nViewRadius;b=j+m_nViewRadius;
	if(l<0) l=0;
	if(t<0) t=0;
	if(r>m_nNumVerticesPerRow-1) r=m_nNumVerticesPerRow-1;
	if(b>m_nNumVerticesPerCol-1) b=m_nNumVerticesPerCol-1;
	
	//if(m_pVBView) m_pDevice->CreateVertexBuffer((m_nViewRadius+2)*
	if(!m_pIBView) 
		m_pDevice->CreateIndexBuffer((m_nViewRadius*2+1)*(m_nViewRadius*2+1)*2*3*4,D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_MANAGED,&m_pIBView,NULL);
	DWORD *verticesDes=NULL;
	DWORD *verticesOrg=NULL;
	UINT nNumIndices=0;
	m_pIBView->Lock(0,0,(void**)&verticesDes,NULL);
	m_pIB->Lock(0,0,(void**)&verticesOrg,NULL);
	for(int m=t;m<=b;m++)//b
	{
		for(int n=l;n<=r;n++)
		{
			memcpy(verticesDes+nNumIndices,verticesOrg+(m*(m_nNumVerticesPerRow-1)+n)*6,4*6);
			nNumIndices+=6;
		}
		//break;
	}
	//memcpy(verticesDes,verticesOrg,4*6*1000);
	//nNumIndices=6000;
	for(int i=0;i<nNumIndices;i+=6)
	{
		ROSEDump_OutputWindow("(%d,%d,%d)(%d,%d,%d)\n",
			verticesDes[i],
			verticesDes[i+1],
			verticesDes[i+2],
			verticesDes[i+3],
			verticesDes[i+4],
			verticesDes[i+5]);
	}
	m_pIBView->Unlock();
	m_pIB->Unlock();
	
	if(m_pTex) m_pDevice->SetTexture(0,m_pTex);
	m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(TerrainVertex));
	m_pDevice->SetFVF(TerrainVertex::FVF);
	m_pDevice->SetIndices(m_pIBView);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nNumVertices,0,nNumIndices/3);
	*/
	if(m_pTex) m_pDevice->SetTexture(0,m_pTex);
	m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(TerrainVertex));
	m_pDevice->SetFVF(TerrainVertex::FVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nNumVertices,0,m_nNumTriangles);
	
	if(m_pTex) m_pDevice->SetTexture(0,NULL);
}

float ROSECTerrain::GetHeight(float x,float z)
{
	//浮点数的小数代表了在格子中的百分比位置
	float r=(m_ptPos._y-z)/m_fCellSpacing;//格子行坐标，即A点行坐标
	float c=(x-m_ptPos._x)/m_fCellSpacing;//格子列坐标，即A点列坐标
	int i=(int)r,j=(int)c;
	//TRACE(_T("%d,%d\n"),i,j);
	if(i>=m_nNumVerticesPerCol-1 || j>=m_nNumVerticesPerRow-1 || i<0 || j<0) return 0.0f;
	
	//  A   B
	//  *---*
	//  | / |
	//  *---*
	//  C   D
	
	float A=(float)m_heightmap[GetVertexIndex(i,  j  )];
	float B=(float)m_heightmap[GetVertexIndex(i,  j+1)];
	float C=(float)m_heightmap[GetVertexIndex(i+1,j  )];
	float D=(float)m_heightmap[GetVertexIndex(i+1,j+1)];
	
	float dx=c-j;
	float dz=r-i;
	float height=0;
	if(dz<1-dx)
	{
		float uy=B-A;
		float vy=C-A;
		height=A+uy*dx+uy*dx+vy*dz;
	}
	else
	{
		float uy=C-D;
		float vy=B-D;
		height=D+uy*(1-dx)+vy*(1-dz);
	}
	return height;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//CFlatTerrain

void CFlatTerrain::Init(
	IDirect3DDevice9 *pDevice,
	ROSEPOINTF ptPos,
	int nNumVerticesInRow,
	int nNumVerticesInCol,
	float fCellSpacing,
	float fHeightScale,
	LPCTSTR lpszFileName)
{
	m_pDevice=pDevice;
	m_ptPos=ptPos;
	m_nNumVerticesPerRow=nNumVerticesInRow;
	m_nNumVerticesPerCol=nNumVerticesInCol;
	m_fCellSpacing=fCellSpacing;
	m_fHeightScale=fHeightScale;

	m_nNumVertices=m_nNumVerticesPerRow*m_nNumVerticesPerCol;
	m_nNumTriangles=(m_nNumVertices-m_nNumVerticesPerCol-m_nNumVerticesPerRow+1)*2;
	
	UINT nHeightmapSize=m_nNumVerticesPerRow*m_nNumVerticesPerCol;
	m_heightmap.resize(nHeightmapSize);
	for(UINT i=0;i<nHeightmapSize;i++)
	{
		m_heightmap[i]=0;
	}
	
	GenerateVertices();//生成网格顶点
	GenerateIndices();//生成三角形索引
	//GenerateTexture();//生成纹理
}

//生成网格顶点
void CFlatTerrain::GenerateVertices()
{
	//纹理的(0,0)与地形网格的左上角重合，(1,1)与右下角重合
	m_pDevice->CreateVertexBuffer(
		m_nNumVertices*sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,NULL);
	float fDu=1.0f/m_nNumVerticesPerCol,fDv=1.0f/m_nNumVerticesPerRow;//以地形网格左上角的顶点作为原点，相邻格子的纹理坐标在x和y方向上的坐标差值
	TerrainVertex *vertices=NULL;
	TerrainVertex *pVert=NULL;
	m_pVB->Lock(0,0,(void**)&vertices,0);
	for(int i=0;i<m_nNumVerticesPerCol;i++)//逐行
	{
		for(int j=0;j<m_nNumVerticesPerRow;j++)//逐列
		{
			int index=i*m_nNumVerticesPerRow+j;
			pVert=vertices+index;
			pVert->m_x=m_ptPos._x+j*m_fCellSpacing;//X方向是加
			pVert->m_y=m_heightmap[index]*m_fHeightScale;
			pVert->m_z=m_ptPos._y-i*m_fCellSpacing;//Z方向是减
			pVert->m_u=(float)i;//fDu*i;
			pVert->m_v=(float)j;//fDv*j;
		}
	}
	m_pVB->Unlock();
}