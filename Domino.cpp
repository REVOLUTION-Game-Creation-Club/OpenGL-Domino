#include "StdAfx.h"
#include "Domino.h"
//#include "../RevSound/SoundManager.h"

extern HWND g_hDlg;

//using namespace rev;

Domino::Domino(void)
{
	m_selectBlockType = TYPE0;
	m_start = false;
}

Domino::~Domino(void)
{
}

void Domino::selectBlockType(BLOCK_TYPE blockType)
{
	// 선택한 타일 종류를 표시한다.
	switch(blockType)
	{
	case TYPE0:
		SetDlgItemText(g_hDlg,IDC_BLOCKTYPE,"블록없음");
		break;
	case TYPE1:
		SetDlgItemText(g_hDlg,IDC_BLOCKTYPE,"종류1");
		break;
	case TYPE2:
		SetDlgItemText(g_hDlg,IDC_BLOCKTYPE,"종류2");
		break;
	case TYPE3:
		SetDlgItemText(g_hDlg,IDC_BLOCKTYPE,"종류3");
		break;
	case TYPE4:
		SetDlgItemText(g_hDlg,IDC_BLOCKTYPE,"종류4");
		break;
	}

	m_selectBlockType = blockType;
}

void Domino::drawBoard(void)
{
	// 렌더링
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	// 화면과 깊이 버퍼를 지운다.
	glClearColor(1.0f,1.0f,1.0f,0.0f);						// 배경을 횐색으로
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// 색상과 깊이 버퍼를 지운다.
	glLoadIdentity();										// 모델뷰 행렬 초기화
	if(m_start)
	{
		m_start = !m_board.startUpdate();
	}
	glPushMatrix();
	m_board.draw();
	glPopMatrix();
}
// 좌표가 보드영역에 속하는지 확인한다. 
bool Domino::InBoard(int x, int y)
{
	RECT rtBoardClient;
	HWND hBoard = GetDlgItem( g_hDlg, IDC_BOARD );
	GetClientRect( hBoard, &rtBoardClient );

	// 보드상의 상대적인 마우스 좌표 값
	POINT boardPt = getBoardPoint(x,y);

	if((0 <= boardPt.x)&&( boardPt.x <= rtBoardClient.right)&&(0 < boardPt.y - 1)&&( boardPt.y < rtBoardClient.bottom - 1))
		return true;
	return false;
}

// 주어진 마우스 좌표값의 타일이 어느 것인지 타일 X, Y값을 돌려준다. 
POINT Domino::pickTile(int x, int y)
{
	RECT rtBoardClient;
	HWND hBoard = GetDlgItem( g_hDlg, IDC_BOARD );
	GetClientRect( hBoard, &rtBoardClient );
	
	// 타일간의 실제 간격을 구한다.
	float cw = rtBoardClient.right / TILE_WIDTH;
	float ch = rtBoardClient.bottom / TILE_HEIGHT;

	// 보드상의 상대적인 마우스 좌표 값
	POINT boardPt = getBoardPoint(x,y);

	// pt에 타일 인덱스 값을 저장한다.
	POINT TileIndex;
	TileIndex.x = boardPt.x / cw;

	TileIndex.y = TILE_HEIGHT - (boardPt.y / ch);

	return TileIndex;
}

// 타일에 블록을 설정한다.
bool Domino::putBlockOnTile(int x, int y)
{
	// 클릭한 마우스 좌표가 보드 위에 있는지 확인한다.
	if(!InBoard(x,y))
		return false;
	
	// 마우스 좌표 위치의 타일의 인덱스 값을 구한다.
	POINT pos = pickTile(x,y);
	
	m_board.setBlockOnTile(pos, m_selectBlockType);

//	getSoundManager()->play("click");

	return true;
}


// 타일에 블록을 설정한다.
bool Domino::putStartOnTile(int x, int y)
{
	// 클릭한 마우스 좌표가 보드 위에 있는지 확인한다.
	if(!InBoard(x,y))
		return false;

	// 마우스 좌표 위치의 타일의 인덱스 값을 구한다.
	POINT pos = pickTile(x,y);
	
	// 시작 타일 좌표를 설정한다.
	m_board.setStartTile(pos.x,pos.y);

	return true;
}

// 보드의 상대적 좌표를 구한다.
POINT Domino::getBoardPoint(int ox, int oy)
{
	RECT rtDlgWinodw;
	RECT rtDlgClient;
	RECT rtBoardWindow;
	RECT rtBoardClient;

	GetWindowRect( g_hDlg, &rtDlgWinodw );
	GetClientRect( g_hDlg, &rtDlgClient );

	HWND hBoard = GetDlgItem( g_hDlg, IDC_BOARD );
	GetWindowRect( hBoard, &rtBoardWindow );
	GetClientRect( hBoard, &rtBoardClient );

	// 보드넓이 = 윈도우 넓이 - 클라이언트 넓이) /2
	int borderWidth = ((rtDlgWinodw.right - rtDlgWinodw.left) - rtDlgClient.right)/2;
	// 타이틀바 높이 = 윈도우 높이 - 클라이언트 높이 - 아래보드 높이
	int titleHeight = (rtDlgWinodw.bottom - rtDlgWinodw.top) - rtDlgClient.bottom - borderWidth;

	// 보드와 다이얼로그 작업창의 간격 = 타이얼로그 좌표 - 윈도우좌표 - (타이틀바 높이/ 보드 넓이)
	POINT pt;
	pt.x = ox - (rtBoardWindow.left - rtDlgWinodw.left - borderWidth);
	pt.y = oy - (rtBoardWindow.top - rtDlgWinodw.top - titleHeight);

	return pt;
}

// 도미노를 시작한다.
void Domino::start(void)
{
	m_start = true;
	m_board.start();
}

// 블록정보를 초기화
void Domino::reset(void)
{
	m_board.reset();
}

// 블록을 세운다
void Domino::stand(void)
{
	m_board.stand();
}
