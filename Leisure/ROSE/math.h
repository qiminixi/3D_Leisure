#ifndef ROSE_MATH_H
#define ROSE_MATH_H

//void ROSERotatePointX(D3DXVECTOR3 *pVec,float angel);
//void ROSERotatePointX(D3DXVECTOR3 *pVecDes,D3DXVECTOR3 *pVecSrc,float angel);

inline void ROSERotatePointX(D3DXVECTOR3 *pVec,float angel)
{
	pVec->y=pVec->y*cos(angel)-pVec->z*sin(angel);//第二帧顶点的法向量
	pVec->z=pVec->z*cos(angel)+pVec->y*sin(angel);//第二帧顶点的法向量
}

inline void ROSERotatePointX(D3DXVECTOR3 *pVecDes,D3DXVECTOR3 *pVecSrc,float angel)
{
	pVecDes->x=pVecSrc->x;
	pVecDes->y=pVecSrc->y*cos(angel)-pVecSrc->z*sin(angel);//第二帧顶点的法向量
	pVecDes->z=pVecSrc->z*cos(angel)+pVecSrc->y*sin(angel);//第二帧顶点的法向量
}

#endif