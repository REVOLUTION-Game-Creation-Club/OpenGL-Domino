#pragma once
#include "Board.h"
#include "Block.h"

class Domino
{
private:
	Board		m_board;
	BLOCK_TYPE	m_selectBlockType;		// 선택한 블럭 타입
	bool		m_start;				// 게임의 시작여부

	// 좌표가 보드영역에 속하는지 확인한다.
	bool InBoard(int x, int y);

	// 주어진 마우스 좌표값의 타일이 어느 것인지 타일 X, Y값을 돌려준다. 
	POINT pickTile(int x, int y);
	
	// 다이얼로그 좌표를 보드의 상대 좌표로 변환한다.
	POINT getBoardPoint(int ox, int oy);
public:
	Domino(void);
	virtual ~Domino(void);

	// 선택한 블록 타입을 저장한다.
	void selectBlockType(BLOCK_TYPE blockType);

	// 도미노 보드를 그린다.
	void drawBoard(void);

	// 타일에 블록을 설정한다.
	bool putBlockOnTile(int x, int y);

	// 타일에 시작 블록을 설정한다.
	bool putStartOnTile(int x, int y);
public:
	// 도미노를 시작한다.
	void start(void);
public:
	// 블록정보를 초기화
	void reset(void);

	// 블록을 세운다.
	void stand(void);
};