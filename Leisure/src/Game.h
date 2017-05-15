//先不封装引擎了，先走一遍整合版的流程，以后再做封装。
//引擎的话就是总是读取文件了，但是这里还会自己用代码创建物体
struct object
{
	//位置
	//
};

void GameInit(HWND hWnd);//初始化游戏，加载游戏资源
void GameDistroy();//关闭游戏
void GameReset();//比如窗口大小发生变化时要调整尺寸
void GameRender();//渲染一帧
void GameInput();//输入

void GameRun();//游戏运行，将在这里进行游戏世界的自身运作
void GameRun_AI();//物体的自身行为
void GameRun_Pyscs();//物理系统
void GameRun_P();//碰撞检测

void KeyDown(UINT vk);
void KeyUp(UINT vk);
void MouseMove();
void MouseClick();


