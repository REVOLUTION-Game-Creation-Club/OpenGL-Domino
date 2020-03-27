#pragma once


HWND g_hwnd;
HWND g_hDlg;
HINSTANCE g_hInstance;
HDC g_hDC;
HGLRC g_hRC;			// 렌더링 컨텍스트

// OpenGL 설정
void SetupRC();

void Resize(RECT rect);

// 초기화
void Initialize(HDC hdc,RECT rect);

// 파과자
void Destroy();

void ProcessKey(DWORD vkcode);

// 렌더링 함수
void Render();

// 픽셀포멧을 설정한다.
void SetupPixelFormat(HDC hDC);

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// 다이얼로그 프로시저
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// 윈도우를 생성한다.
HWND CreateMyWindow(HINSTANCE hInstance, int width, int height, bool fullScreen);

// 메인루프
WPARAM MainLoop();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);