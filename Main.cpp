#include "stdafx.h"
#include "main.h"
//#include "Camera.h"
#include "Domino.h"
//#include "../RevSound/SoundManager.h"

//using namespace rev;

extern HWND g_hwnd;
extern HWND g_hDlg;
extern HINSTANCE g_hInstance;
extern HDC g_hDC;
extern HGLRC g_hRC;

bool g_fullScreen = false;
bool g_done = false;
int g_gameMode = 0;			// 게임 모드

//CCamera g_camera;
Domino g_domino;

void SetupRC()
{
	GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	
	GLfloat lightPos[] = { TILE_SIZE_X*TILE_WIDTH/2.0f, TILE_SIZE_Y*TILE_HEIGHT/2.0f, 50.0f, 1.0f };
	GLfloat specularRef[] = { 0.6f, 0.6f, 0.6f, 1.0f };

	glEnable(GL_DEPTH_TEST);	// 은면제거
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);		// 솔리드 객체 내부에 대해서는 계산하지 않는다.

	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularRef);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}

void Resize(RECT rect)
{
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	//g_camera.PositionCamera(TILE_SIZE_X*TILE_WIDTH/2, TILE_SIZE_Y*TILE_HEIGHT/2, 1.0f,	TILE_SIZE_X*TILE_WIDTH/2, TILE_SIZE_Y*TILE_HEIGHT/2, 0.0f,	0, 1, 0);
	// 뷰포트 크기를 다시 설정
	glViewport(0,0,width,height);
	
	glMatrixMode(GL_PROJECTION);	// 투영 행렬로 설정
	glLoadIdentity();				// 투영 행렬을 초기화
	//g_camera.PositionCamera(0.0f, 0.0f , 1.0f,	0.0f,0.0f, 0.0f,	0, 1, 0);

	if(width <= height)
		glOrtho(-TILE_SIZE_X*TILE_WIDTH/2, TILE_SIZE_X*TILE_WIDTH/2, -TILE_SIZE_Y*TILE_HEIGHT/2*((float)height/(float)width),TILE_SIZE_Y*TILE_HEIGHT/2*((float)height/(float)width),-100.0f,100.0f);
	else
		glOrtho(-TILE_SIZE_X*TILE_WIDTH/2*((float)width/(float)height), TILE_SIZE_X*TILE_WIDTH/2*((float)width/(float)height), -TILE_SIZE_Y*TILE_HEIGHT/2,TILE_SIZE_Y*TILE_HEIGHT/2,-100.0f,100.0f);

	glMatrixMode(GL_MODELVIEW);		// 모델뷰 행렬로 설정
	glLoadIdentity();				// 모델뷰 행렬을 초기화
}

void ProcessKey(DWORD vkcode)
{
}

void Render()
{
	g_domino.drawBoard();	
	glFlush();
	SwapBuffers(g_hDC);			// 후면 버퍼를 전면 버퍼로 교체
}

void SetupPixelFormat(HDC hDC)
{
	int nPixelFormat;	// 픽셀 형식 색인

	static PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR),
	1,							// 버전 항상 1
	PFD_DRAW_TO_WINDOW|			// 창 모드 지원
	PFD_SUPPORT_OPENGL|			// OpenGL 지원
	PFD_DOUBLEBUFFER|			// 더블 버퍼링 지원
	PFD_TYPE_RGBA,				// RGBA 색상 모드
	32,							// 32 비트 색상 모드
	0,0,0,0,0,0,
	0,
	0,
	0,
	0,0,0,0,
	16,
	0,
	0,
	PFD_MAIN_PLANE,				// 메인 드로잉 평면
	0,
	0,0,0 };

	// 가장 근접한 픽셀 형식을 선택하게 된다. 색인이 반환된다.
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// 픽셀 형식을 장치 문맥에 설정
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	RECT rect;
	switch(message)
	{
	case WM_CREATE:
		g_hDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, DlgProc);
		ShowWindow(g_hDlg,SW_SHOW);
		break;
	case WM_DESTROY:
		g_done = true;
		break;
	//case WM_KEYDOWN:
	//	ProcessKey((DWORD)wParam);
	//	break;
	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

// 다이얼로그 프로시저
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hBoard;
	RECT rc;
	switch(message)
	{
	case WM_INITDIALOG:
		{
			hBoard = GetDlgItem( hDlg, IDC_BOARD );
			GetWindowRect( hBoard, &rc );
			g_hDC = GetDC(hBoard);
			Initialize(g_hDC, rc);
		}
		return TRUE;
	case WM_CLOSE:
		Destroy();
		DestroyWindow(g_hDlg);
		g_hDlg = NULL;
		return TRUE;
	case WM_DESTROY:
//		getSoundManager()->release();
		g_done = true;
		return TRUE;
	case WM_KEYDOWN:
		ProcessKey((DWORD)wParam);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BLOCK0:
			g_domino.selectBlockType(TYPE0);
			break;
		case IDC_BLOCK1:
			g_domino.selectBlockType(TYPE1);
			break;
		case IDC_BLOCK2:
			g_domino.selectBlockType(TYPE2);
			break;
		case IDC_BLOCK3:
			g_domino.selectBlockType(TYPE3);
			break;
		case IDC_BLOCK4:
			g_domino.selectBlockType(TYPE4);
			break;
		case IDC_START:
			g_domino.start();
			break;
		case IDC_RESET:
			g_domino.reset();
			break;
		case IDC_STAND:
			g_domino.stand();
			break;
		case IDC_EXIT:
			SendMessage(g_hDlg,WM_CLOSE,NULL,NULL);
			break;
		}
		return TRUE;
	case WM_LBUTTONDOWN:
		{
			int ox = LOWORD(lParam);
			int oy = HIWORD(lParam);
			//RECT rtDlgWinodw;
			//RECT rtDlgClient;
			//RECT rtBoard;

			//GetWindowRect( g_hDlg, &rtDlgWinodw );
			//GetClientRect( g_hDlg, &rtDlgClient );

			//hBoard = GetDlgItem( hDlg, IDC_BOARD );
			//GetWindowRect( hBoard, &rtBoard );

			//// 보드넓이 = 윈도우 넓이 - 클라이언트 넓이) /2
			//int borderWidth = ((rtDlgWinodw.right - rtDlgWinodw.left) - rtDlgClient.right)/2;
			//// 타이틀바 높이 = 윈도우 높이 - 클라이언트 높이 - 아래보드 높이
			//int titleHeight = (rtDlgWinodw.bottom - rtDlgWinodw.top) - rtDlgClient.bottom - borderWidth;
			//
			//// 보드와 다이얼로그 작업창의 간격 = 타이얼로그 좌표 - 윈도우좌표 - (타이틀바 높이/ 보드 넓이)
			//int x = ox - (rtBoard.left - rtDlgWinodw.left - borderWidth);
			//int y = oy - (rtBoard.top - rtDlgWinodw.top - titleHeight);

			g_domino.putBlockOnTile(ox,oy);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			int ox = LOWORD(lParam);
			int oy = HIWORD(lParam);
			g_domino.putStartOnTile(ox,oy);
		}
		break;
	}

	return FALSE;
}
void Initialize(HDC hdc, RECT rect)
{
	SetupPixelFormat(hdc);

	// 랜더링 컨텍스트를 생성하고 현재 렌더링 컨텍스트를 만든다.
	g_hRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, g_hRC);
	Resize(rect);
	SetupRC();
}
void Destroy()
{
	// 렌더링 컨텍스트를 해제하고 삭제한다.
	wglMakeCurrent(g_hDC, NULL);
	wglDeleteContext(g_hRC);

	if (g_hDC)
		ReleaseDC(g_hDlg, g_hDC);						// Release our HDC from memory

	if(g_fullScreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}
	// 메세지 큐에 WM_QUIT를 보낸다.
	PostQuitMessage(0);
}

HWND CreateMyWindow(HINSTANCE hInstance, int width, int height, bool fullScreen)
{
	WNDCLASSEX	windowClass;	// 윈도우 클래스

	windowClass.cbSize				= sizeof(WNDCLASSEX);
	windowClass.style				= CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc			= WndProc;
	windowClass.cbClsExtra			= 0;
	windowClass.cbWndExtra			= 0;
	windowClass.hInstance			= hInstance;
	windowClass.hIcon				= LoadIcon(NULL, IDI_APPLICATION);		// 기본 아이콘
	windowClass.hIconSm				= LoadIcon(NULL, IDI_WINLOGO);			// 작은 아이콘
	windowClass.hCursor				= LoadCursor(NULL, IDC_ARROW);			// 기본 화살표
	windowClass.hbrBackground		= NULL;									// 기본 배경 없음
	windowClass.lpszMenuName		= NULL;									// 메뉴 없음
	windowClass.lpszClassName		= "DominoClass";

	// 윈도우 클래스 등록
	if(!RegisterClassEx(&windowClass))
		return 0;

	// 전체화면 설정 변수들

	DWORD dwExStyle;
	DWORD dwStyle;
	int bits = 32;
	RECT windowRect;

	//  전체 화면 모드이면
	if(fullScreen)			
	{
		windowRect.left = 0;
		windowRect.right = width;
		windowRect.top = 0;
		windowRect.bottom = height;

		DEVMODE devMode;
		memset(&devMode, 0, sizeof(devMode));
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = width;
		devMode.dmPelsHeight = height;
		devMode.dmBitsPerPel = bits;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL,"Display mode failed", NULL, MB_OK);
			fullScreen = false;
		}
	}

	if(fullScreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	// 윈도우를 생성
	HWND hwnd = CreateWindowEx(NULL, "DominoClass","Domino",	// 클래스, 제목
		dwStyle,
		0, 0,											// x,y 좌표
		width, height,										// 넓이, 높이
		NULL,												// 부모의 핸들
		NULL,												// 메뉴 핸들
		hInstance,											// 응용프로그램 인스턴스
		NULL);												// 추가 인자 없음

	// hwnd가 NULL이면 윈도우가 생성되지 않는 것이다.
	if(!hwnd)
		return NULL;

	ShowWindow(hwnd, SW_HIDE);			// 윈도우 표시
	UpdateWindow(hwnd);					// 윈도우를 갱신

	return hwnd;
}

WPARAM MainLoop()
{
	MSG			msg;			// 메시지

	// 메인 메세지 루프
	while(!g_done)
	{
		if(PeekMessage(&msg, g_hDlg, NULL, NULL, PM_REMOVE))
		{
			//if(!IsDialogMessage(g_hDlg,&msg))
			{
				TranslateMessage(&msg);		// 메시지를 해석
				DispatchMessage(&msg);		// 이벤트 큐로 전달
			}
		}
		else
		{
			//g_camera.Update();			// Update the camera data
			Render();
		}
	}
	
	return msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_hInstance = hInstance;
	g_hwnd = CreateMyWindow(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT,g_fullScreen);

	return (int)MainLoop();
}
