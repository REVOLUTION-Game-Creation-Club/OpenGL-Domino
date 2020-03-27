#pragma once


HWND g_hwnd;
HWND g_hDlg;
HINSTANCE g_hInstance;
HDC g_hDC;
HGLRC g_hRC;			// ������ ���ؽ�Ʈ

// OpenGL ����
void SetupRC();

void Resize(RECT rect);

// �ʱ�ȭ
void Initialize(HDC hdc,RECT rect);

// �İ���
void Destroy();

void ProcessKey(DWORD vkcode);

// ������ �Լ�
void Render();

// �ȼ������� �����Ѵ�.
void SetupPixelFormat(HDC hDC);

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// ���̾�α� ���ν���
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// �����츦 �����Ѵ�.
HWND CreateMyWindow(HINSTANCE hInstance, int width, int height, bool fullScreen);

// ���η���
WPARAM MainLoop();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);