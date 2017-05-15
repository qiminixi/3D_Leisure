#include <windows.h>
#include <stdio.h>
#include "Game.h"
#include <dinput.h>
#include "../ROSE/DebugHelper.h"
#include "time.h"
#include "resource.h"

static IDirectInputDevice8 *g_pMouse=NULL;
LPDIRECTINPUT8  g_pDI;
static HANDLE g_hMouseEvent;

//核心代号，ROSE
//地形分割以后再做

struct ROSECRect
{
	int _x,_y,_w,_h;
	void SetRect(int x,int y,int w,int h)
	{
		_x=x;_y=y;_w=w;_h=h;
	}
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateProgramWindow(HINSTANCE hInstance);
void Run();//消息循环
void FullScreen();//全屏
void NormalWindow();//普通窗口

DWORD CALLBACK Thread_Render(LPVOID param);//渲染线程
DWORD CALLBACK Thread_AI(LPVOID param);//AI线程
DWORD CALLBACK Thread_Input(LPVOID param);//获取用户输入

int g_ThreadCommand=0;//1=exit
ROSECRect g_rectWindow;//记录当前的窗口矩形
HWND g_hWnd=0;
HINSTANCE g_hInstance;
UINT g_vk;
BOOL g_bKeyDown=FALSE;

int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	g_hInstance=hInstance;
	HWND hWnd=CreateProgramWindow(hInstance);
	g_hWnd=hWnd;
	
	Run();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	UINT keybord;
	switch(uMsg)
	{
	case WM_CREATE:
		GameInit(hWnd);
		CreateThread(NULL,0,Thread_Render,0,0,0);
		CreateThread(NULL,0,Thread_AI,0,0,0);
		CreateThread(NULL,0,Thread_Input,0,0,0);
		g_hWnd=hWnd;
		/*HRESULT hr;
		hr=DirectInput8Create(g_hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&g_pDI,NULL);
		hr=g_pDI->CreateDevice(GUID_SysMouse,&g_pMouse,NULL);
		hr=g_pMouse->SetDataFormat(&c_dfDIMouse);
		hr=g_pMouse->SetCooperativeLevel(g_hWnd,DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		g_hMouseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_pMouse->SetEventNotification(g_hMouseEvent);
		DIPROPDWORD dipdw;
		// the header
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		// the data
		dipdw.dwData            = 16;
		g_pMouse->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph);
		*/
		//CreateThread(NULL,0,Thread_Input,0,0,0);
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,NULL);
		EndPaint(hWnd,NULL);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if(wParam==VK_F1)
		{
			FullScreen();break;
		}
		if(wParam==VK_ESCAPE)
		{
			NormalWindow();break;
		}
		g_bKeyDown=TRUE;
		g_vk=wParam;      
		break;//KeyDown(wParam);	
	case WM_KEYUP:
		g_bKeyDown=FALSE;
		KeyUp(wParam);
		break;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

HWND CreateProgramWindow(HINSTANCE hInstance)
{
	WNDCLASS wc={0};
	wc.hbrBackground=(HBRUSH)::GetStockObject(BLACK_BRUSH);
	wc.hCursor=NULL;
	//wc.hIcon=(HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hIcon = NULL;
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndProc;
	wc.lpszClassName=TEXT("LeisureClass");
	wc.lpszMenuName=NULL;
	wc.style=CS_HREDRAW | CS_VREDRAW; 
	if(!RegisterClass(&wc))
		return FALSE;
	
	//根据窗口尺寸计算位置，使窗口保持居中
	int w=640,h=480;
	int x=(GetSystemMetrics(SM_CXSCREEN)-w)/2;
	int y=(GetSystemMetrics(SM_CYSCREEN)-h)/2;
	RECT rect={x,y,x+w,y+h};
	
	//根据客户区大小和位置计算窗口位置
	AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,FALSE);
	x=rect.left;
	y=rect.top;
	w=rect.right-rect.left;
	h=rect.bottom-rect.top;
	g_rectWindow.SetRect(x,y,w,h);
	
	return CreateWindow(
		TEXT("LeisureClass"),
		TEXT("Leisure"),
		WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		x,y,w,h,
		NULL,NULL,hInstance,NULL);
}

//消息循环
void Run()
{
	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT) break;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		Sleep(1);
	}
	
}


//渲染线程
DWORD CALLBACK Thread_Render(LPVOID param)
{
	while(g_ThreadCommand!=1)
	{
		Sleep(100);
		clock_t t1=clock();
		GameRender();
		
		GameRun_AI();
		clock_t t2=clock();
		ROSEDump_OutputWindow("render+t1=%d,t2=%d,t2-t1=%d\n",t1,t2,t2-t1);
	}
	return 0;
}

//AI线程
DWORD CALLBACK Thread_AI(LPVOID param)
{
	while(g_ThreadCommand!=1)
	{
		Sleep(100);
		
	}
	return 0;
}

//输入线程
DWORD CALLBACK Thread_Input(LPVOID param)
{
	while(g_ThreadCommand!=1)
	{
		while(!g_bKeyDown) Sleep(10);//等待键盘按下
		
		if(g_vk!='A' && g_vk!='S' && g_vk!='D' && g_vk!='W' && 
			g_vk!=VK_UP && g_vk!=VK_DOWN && g_vk!=VK_LEFT && g_vk!=VK_RIGHT)
		{
			KeyDown(g_vk);
			Sleep(500);
		}
		
		ROSEDump_OutputWindow("KeyDown\n");
		int i=0;
		clock_t t1,t2;
		while(g_bKeyDown)//如果一直没弹起，就一直发送事件通知
		{
			t1=clock();
			KeyDown(g_vk);
			
			Sleep(100);
			t2=clock();
			ROSEDump_OutputWindow("t1=%d,t2=%d,t2-t1=%d,i=%d\n",t1,t2,t2-t1,i);
		}
		ROSEDump_OutputWindow("KeyUp\n");
	}
	return 0;
	
	
	
	BOOL g_bSwapMouseButtons;
	while(g_ThreadCommand!=1)
	{
		//Sleep(100);
		//GameInput();
		HRESULT hr;
		BOOL bDone=0;
		int nButton;
		int x,y;
		WaitForSingleObject(g_hMouseEvent,0xffffffff);
		while (!bDone) {
			DIDEVICEOBJECTDATA od;
			DWORD dwElements = 1;   // number of items to be retrieved
			hr = g_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
				&od, &dwElements, 0);
			switch (od.dwOfs) 
			{
				// Mouse horizontal motion
			case DIMOFS_X: 
				x=od.dwData;
				break;

				// Mouse vertical motion
			case DIMOFS_Y: 
				y=od.dwData;
				break; 

				// DIMOFS_BUTTON1: Right button pressed or released 
			case DIMOFS_BUTTON1:

				// DIMOFS_BUTTON0: Left button pressed or released 
			case DIMOFS_BUTTON0:
				// Is the right button or a swapped left button down?
				if((g_bSwapMouseButtons && 
					DIMOFS_BUTTON1 == od.dwOfs) ||
					(!g_bSwapMouseButtons && 
					DIMOFS_BUTTON0 == od.dwOfs))
				{
					if (od.dwData & 0x80)  // Left button pressed, so
						// go into button-down mode
					{
						bDone = TRUE;
						//OnLeftButtonDown(hWnd);
					}
					// Is the left button or a swapped right button down?
					if((g_bSwapMouseButtons && 
						DIMOFS_BUTTON0 == od.dwOfs) ||
						(!g_bSwapMouseButtons && 
						DIMOFS_BUTTON1 == od.dwOfs))
					{
						if(!(od.dwData & 0x80))  // button release, so
							// check shortcut menu
						{  
							bDone = TRUE;
							//OnRightButtonUp(hWnd); 
						}
					}
					break;        
				}
			}
		}
	}
	return 0;
}

//全屏
void FullScreen()
{
	int screenW=GetSystemMetrics(SM_CXSCREEN);
	int screenH=GetSystemMetrics(SM_CYSCREEN);
	DWORD style=GetWindowLong(g_hWnd,GWL_STYLE);
	style&=~WS_OVERLAPPEDWINDOW;
	style|=WS_VISIBLE;
	SetWindowLong(g_hWnd,GWL_STYLE,style);
	MoveWindow(g_hWnd,0,0,screenW,screenH,TRUE);
}

//窗口模式
void NormalWindow()
{
	DWORD style=GetWindowLong(g_hWnd,GWL_STYLE);
	style&=~WS_POPUPWINDOW;
	style|=WS_OVERLAPPEDWINDOW;
	SetWindowLong(g_hWnd,GWL_STYLE,style);
	MoveWindow(
		g_hWnd,
		g_rectWindow._x,
		g_rectWindow._y,
		g_rectWindow._w,
		g_rectWindow._h,
		TRUE);
}