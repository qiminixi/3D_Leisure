//�Ȳ���װ�����ˣ�����һ�����ϰ�����̣��Ժ�������װ��
//����Ļ��������Ƕ�ȡ�ļ��ˣ��������ﻹ���Լ��ô��봴������
struct object
{
	//λ��
	//
};

void GameInit(HWND hWnd);//��ʼ����Ϸ��������Ϸ��Դ
void GameDistroy();//�ر���Ϸ
void GameReset();//���細�ڴ�С�����仯ʱҪ�����ߴ�
void GameRender();//��Ⱦһ֡
void GameInput();//����

void GameRun();//��Ϸ���У��������������Ϸ�������������
void GameRun_AI();//�����������Ϊ
void GameRun_Pyscs();//����ϵͳ
void GameRun_P();//��ײ���

void KeyDown(UINT vk);
void KeyUp(UINT vk);
void MouseMove();
void MouseClick();


