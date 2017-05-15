#ifndef D3DUTILITIES_CAMERA_H
#define D3DUTILITIES_CAMERA_H


//第三人称视角是在第一人称视角的基础上，向look反方向移动得到相机的最终位置
class ROSECamera
{
public:
	enum CameraType{LandObject,Aircraft};
	enum ViewType{ViewType_First,ViewType_Third};
	
	ROSECamera();//默认为LandObject，坐标系与世界坐标系重合
	~ROSECamera();
	void Init(CameraType ctype,
		ViewType vtype, 
		D3DXVECTOR3 *pVectPos,
		D3DXVECTOR3 *pVectLook,
		D3DXVECTOR3 *pVectUp);

	//沿某一轴移动
	void MoveRL(float units);//左右移动(RightLeft)
	void MoveUD(float units);//上下移动(UpDown)
	void MoveFB(float units);//前后移动(ForwardBackward)
	
	//绕某一轴运动
	void Pitch(float angle);//俯仰(Pitch)，绕right轴
	void Yaw(float angle);  //扫视(Yaw)，  绕up轴
	void Roll(float angle); //翻滚(Roll)， 绕look轴
	
	//任意方向移动
	//任意轴旋转
	
	//访问
	void GetViewMatrix(D3DXMATRIX *pOut);//获取观察变换矩阵
	CameraType GetCameraType();
	void SetCameraType(CameraType type);
	void GetPosition(D3DXVECTOR3 *pOut);
	void SetPosition(D3DXVECTOR3 *pIn);
	void SetRight(D3DXVECTOR3 *pIn);
	void SetPosY(float y);//更改垂直位置
	void SetHeight(float fHeight){m_fHeight=fHeight;}
	void SetCenter(D3DXVECTOR3 *pIn){m_vecCenter=*pIn;}
	
	void GetRight(D3DXVECTOR3 *pOut){*pOut=m_vecRight;}
	void GetUp(D3DXVECTOR3 *pOut)   {*pOut=m_vecUp;}
	void GetLook(D3DXVECTOR3 *pOut) {*pOut=m_vecLook;}
	void GetRotateMatrix(D3DXMATRIX *pOut){*pOut=m_mtrRotate;}
	float GetHeight(){return m_fHeight;}
	void GetCenter(D3DXVECTOR3 *pOut){*pOut=m_vecCenter;}
	
	void MouseMove(int x,int y);//鼠标移动，视角跟着动
	void SetInitMousePos(int x,int y);//初始鼠标位置
	
public:
	CameraType m_cameratype;
	D3DXVECTOR3 m_vecPos;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecCenter;//第三人称视角时，相机的围绕中心
	float m_fHeight;//距离地面的高度，LandObject用
	ViewType m_viewtype;//第一人还是第三人称视角
	float m_fRadius;//围绕中心的半径
	float m_fAngelR;//绕right轴旋转的角度
	float m_fAngelU;//绕up轴旋转的角度
	float m_fAngelL;//绕look轴旋转的角度

public:
	POINT m_ptPreMousePos;//鼠标的上个位置
	D3DXMATRIX m_mtrRotate;//记录旋转情况
	D3DXVECTOR3 m_posThirdView;//第三人称视角时的相机位置
	
};


#endif