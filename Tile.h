#pragma once

#include "define.h"

class Tile
{
private:
	GLfloat		m_position[2];
	TILE_TYPE	m_type;
	BLOCK_POWER m_power;

	// 시작 타일의 화살표를 그린다.
	void drawArrow();

	void drawStartTile();
public:
	Tile(void);
	virtual ~Tile(void);
	
	// 타일을 그린다.
	void draw(void);

	// 타일의 좌표를 설정한다
	void setPosition(GLfloat x, GLfloat y);

	// 타일의 타입을 설정한다.
	void setType(TILE_TYPE type);

	// 시작 타일의 파워를 설정한다.
	void setPower(BLOCK_POWER power);

};
