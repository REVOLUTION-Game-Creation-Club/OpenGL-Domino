#pragma once

#include "define.h"
#include "Tile.h"
#include "Block.h"


class Board
{
private:
	Tile			m_tile[TILE_WIDTH][TILE_HEIGHT];
	Block			m_block[TILE_WIDTH][TILE_HEIGHT];

	POINT			m_startTile;			// 시작 타일 인덱스 값
	BLOCK_POWER		m_startPower;			// 시작 타일 힘

	POINT			tile;					// 진행중인 타일의 정보를 임시 저장
	BLOCK_POWER		power;

	// 다음 블록이 졵재하는지 확인한다.
	bool isNextBlock(POINT index, BLOCK_POWER power);

	// 다음 블럭의 인덱스를 구한다.
	POINT getNextIndex(POINT index, BLOCK_POWER power);
public:
	Board(void);
	virtual ~Board(void);

	// 타일에 블록을 놓는다
	void setBlockOnTile(POINT pos, BLOCK_TYPE type);

	// 도미노를 쓰러뜨린다.
	void start(void);

	// 타일들을 그린다.
	void draw(void);

	// 도미노 쓰러지는 한번의 처리
	bool startUpdate();

	// 해당하는 인덱스의 블록을 리턴한다.
	Block& getBlock(int x, int y);

	// 시작 타일 인덱스 좌표를 설정한다
	void setStartTile(int x, int y);

	// 블록 정보들을 초기화한다.
	void reset(void);
public:
	void stand(void);
};