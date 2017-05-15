#ifndef D3DUTILITIES_CAMERA_H
#define D3DUTILITIES_CAMERA_H


//�����˳��ӽ����ڵ�һ�˳��ӽǵĻ����ϣ���look�������ƶ��õ����������λ��
class ROSECamera
{
public:
	enum CameraType{LandObject,Aircraft};
	enum ViewType{ViewType_First,ViewType_Third};
	
	ROSECamera();//Ĭ��ΪLandObject������ϵ����������ϵ�غ�
	~ROSECamera();
	void Init(CameraType ctype,
		ViewType vtype, 
		D3DXVECTOR3 *pVectPos,
		D3DXVECTOR3 *pVectLook,
		D3DXVECTOR3 *pVectUp);

	//��ĳһ���ƶ�
	void MoveRL(float units);//�����ƶ�(RightLeft)
	void MoveUD(float units);//�����ƶ�(UpDown)
	void MoveFB(float units);//ǰ���ƶ�(ForwardBackward)
	
	//��ĳһ���˶�
	void Pitch(float angle);//����(Pitch)����right��
	void Yaw(float angle);  //ɨ��(Yaw)��  ��up��
	void Roll(float angle); //����(Roll)�� ��look��
	
	//���ⷽ���ƶ�
	//��������ת
	
	//����
	void GetViewMatrix(D3DXMATRIX *pOut);//��ȡ�۲�任����
	CameraType GetCameraType();
	void SetCameraType(CameraType type);
	void GetPosition(D3DXVECTOR3 *pOut);
	void SetPosition(D3DXVECTOR3 *pIn);
	void SetRight(D3DXVECTOR3 *pIn);
	void SetPosY(float y);//���Ĵ�ֱλ��
	void SetHeight(float fHeight){m_fHeight=fHeight;}
	void SetCenter(D3DXVECTOR3 *pIn){m_vecCenter=*pIn;}
	
	void GetRight(D3DXVECTOR3 *pOut){*pOut=m_vecRight;}
	void GetUp(D3DXVECTOR3 *pOut)   {*pOut=m_vecUp;}
	void GetLook(D3DXVECTOR3 *pOut) {*pOut=m_vecLook;}
	void GetRotateMatrix(D3DXMATRIX *pOut){*pOut=m_mtrRotate;}
	float GetHeight(){return m_fHeight;}
	void GetCenter(D3DXVECTOR3 *pOut){*pOut=m_vecCenter;}
	
	void MouseMove(int x,int y);//����ƶ����ӽǸ��Ŷ�
	void SetInitMousePos(int x,int y);//��ʼ���λ��
	
public:
	CameraType m_cameratype;
	D3DXVECTOR3 m_vecPos;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecCenter;//�����˳��ӽ�ʱ�������Χ������
	float m_fHeight;//�������ĸ߶ȣ�LandObject��
	ViewType m_viewtype;//��һ�˻��ǵ����˳��ӽ�
	float m_fRadius;//Χ�����ĵİ뾶
	float m_fAngelR;//��right����ת�ĽǶ�
	float m_fAngelU;//��up����ת�ĽǶ�
	float m_fAngelL;//��look����ת�ĽǶ�

public:
	POINT m_ptPreMousePos;//�����ϸ�λ��
	D3DXMATRIX m_mtrRotate;//��¼��ת���
	D3DXVECTOR3 m_posThirdView;//�����˳��ӽ�ʱ�����λ��
	
};


#endif