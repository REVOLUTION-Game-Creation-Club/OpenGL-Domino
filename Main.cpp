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
int g_gameMode = 0;			// ���� ���

//CCamera g_camera;
Domino g_domino;

void SetupRC()
{
	GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	
	GLfloat lightPos[] = { TILE_SIZE_X*TILE_WIDTH/2.0f, TILE_SIZE_Y*TILE_HEIGHT/2.0f, 50.0f, 1.0f };
	GLfloat specularRef[] = { 0.6f, 0.6f, 0.6f, 1.0f };

	glEnable(GL_DEPTH_TEST);	// ��������
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);		// �ָ��� ��ü ���ο� ���ؼ��� ������� �ʴ´�.

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
	// ����Ʈ ũ�⸦ �ٽ� ����
	glViewport(0,0,width,height);
	
	glMatrixMode(GL_PROJECTION);	// ���� ��ķ� ����
	glLoadIdentity();				// ���� ����� �ʱ�ȭ
	//g_camera.PositionCamera(0.0f, 0.0f , 1.0f,	0.0f,0.0f, 0.0f,	0, 1, 0);

	if(width <= height)
		glOrtho(-TILE_SIZE_X*TILE_WIDTH/2, TILE_SIZE_X*TILE_WIDTH/2, -TILE_SIZE_Y*TILE_HEIGHT/2*((float)height/(float)width),TILE_SIZE_Y*TILE_HEIGHT/2*((float)height/(float)width),-100.0f,100.0f);
	else
		glOrtho(-TILE_SIZE_X*TILE_WIDTH/2*((float)width/(float)height), TILE_SIZE_X*TILE_WIDTH/2*((float)width/(float)height), -TILE_SIZE_Y*TILE_HEIGHT/2,TILE_SIZE_Y*TILE_HEIGHT/2,-100.0f,100.0f);

	glMatrixMode(GL_MODELVIEW);		// �𵨺� ��ķ� ����
	glLoadIdentity();				// �𵨺� ����� �ʱ�ȭ
}

void ProcessKey(DWORD vkcode)
{
}

void Render()
{
	g_domino.drawBoard();	
	glFlush();
	SwapBuffers(g_hDC);			// �ĸ� ���۸� ���� ���۷� ��ü
}

void SetupPixelFormat(HDC hDC)
{
	int nPixelFormat;	// �ȼ� ���� ����

	static PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR),
	1,							// ���� �׻� 1
	PFD_DRAW_TO_WINDOW|			// â ��� ����
	PFD_SUPPORT_OPENGL|			// OpenGL ����
	PFD_DOUBLEBUFFER|			// ���� ���۸� ����
	PFD_TYPE_RGBA,				// RGBA ���� ���
	32,							// 32 ��Ʈ ���� ���
	0,0,0,0,0,0,
	0,
	0,
	0,
	0,0,0,0,
	16,
	0,
	0,
	PFD_MAIN_PLANE,				// ���� ����� ���
	0,
	0,0,0 };

	// ���� ������ �ȼ� ������ �����ϰ� �ȴ�. ������ ��ȯ�ȴ�.
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// �ȼ� ������ ��ġ ���ƿ� ����
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

// ������ ���ν���
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

// ���̾�α� ���ν���
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

			//// ������� = ������ ���� - Ŭ���̾�Ʈ ����) /2
			//int borderWidth = ((rtDlgWinodw.right - rtDlgWinodw.left) - rtDlgClient.right)/2;
			//// Ÿ��Ʋ�� ���� = ������ ���� - Ŭ���̾�Ʈ ���� - �Ʒ����� ����
			//int titleHeight = (rtDlgWinodw.bottom - rtDlgWinodw.top) - rtDlgClient.bottom - borderWidth;
			//
			//// ����� ���̾�α� �۾�â�� ���� = Ÿ�̾�α� ��ǥ - ��������ǥ - (Ÿ��Ʋ�� ����/ ���� ����)
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

	// ������ ���ؽ�Ʈ�� �����ϰ� ���� ������ ���ؽ�Ʈ�� �����.
	g_hRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, g_hRC);
	Resize(rect);
	SetupRC();
}
void Destroy()
{
	// ������ ���ؽ�Ʈ�� �����ϰ� �����Ѵ�.
	wglMakeCurrent(g_hDC, NULL);
	wglDeleteContext(g_hRC);

	if (g_hDC)
		ReleaseDC(g_hDlg, g_hDC);						// Release our HDC from memory

	if(g_fullScreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}
	// �޼��� ť�� WM_QUIT�� ������.
	PostQuitMessage(0);
}

HWND CreateMyWindow(HINSTANCE hInstance, int width, int height, bool fullScreen)
{
	WNDCLASSEX	windowClass;	// ������ Ŭ����

	windowClass.cbSize				= sizeof(WNDCLASSEX);
	windowClass.style				= CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc			= WndProc;
	windowClass.cbClsExtra			= 0;
	windowClass.cbWndExtra			= 0;
	windowClass.hInstance			= hInstance;
	windowClass.hIcon				= LoadIcon(NULL, IDI_APPLICATION);		// �⺻ ������
	windowClass.hIconSm				= LoadIcon(NULL, IDI_WINLOGO);			// ���� ������
	windowClass.hCursor				= LoadCursor(NULL, IDC_ARROW);			// �⺻ ȭ��ǥ
	windowClass.hbrBackground		= NULL;									// �⺻ ��� ����
	windowClass.lpszMenuName		= NULL;									// �޴� ����
	windowClass.lpszClassName		= "DominoClass";

	// ������ Ŭ���� ���
	if(!RegisterClassEx(&windowClass))
		return 0;

	// ��üȭ�� ���� ������

	DWORD dwExStyle;
	DWORD dwStyle;
	int bits = 32;
	RECT windowRect;

	//  ��ü ȭ�� ����̸�
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

	// �����츦 ����
	HWND hwnd = CreateWindowEx(NULL, "DominoClass","Domino",	// Ŭ����, ����
		dwStyle,
		0, 0,											// x,y ��ǥ
		width, height,										// ����, ����
		NULL,												// �θ��� �ڵ�
		NULL,												// �޴� �ڵ�
		hInstance,											// �������α׷� �ν��Ͻ�
		NULL);												// �߰� ���� ����

	// hwnd�� NULL�̸� �����찡 �������� �ʴ� ���̴�.
	if(!hwnd)
		return NULL;

	ShowWindow(hwnd, SW_HIDE);			// ������ ǥ��
	UpdateWindow(hwnd);					// �����츦 ����

	return hwnd;
}

WPARAM MainLoop()
{
	MSG			msg;			// �޽���

	// ���� �޼��� ����
	while(!g_done)
	{
		if(PeekMessage(&msg, g_hDlg, NULL, NULL, PM_REMOVE))
		{
			//if(!IsDialogMessage(g_hDlg,&msg))
			{
				TranslateMessage(&msg);		// �޽����� �ؼ�
				DispatchMessage(&msg);		// �̺�Ʈ ť�� ����
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
