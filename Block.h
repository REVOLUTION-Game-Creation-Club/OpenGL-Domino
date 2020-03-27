#pragma once

#include "define.h"

class Block
{
private:
	BLOCK_TYPE m_type;
	BLOCK_COLOR m_color;
	GLfloat m_position[2];
	BLOCK_POWER m_power;
//	BLOCK_AREA m_area;
	// 일반 블록을 그린다
	void drawNormalBlock(void);
public:
	Block(void);
	virtual ~Block(void);

	// 블록을 그린다.
	void draw(void);

	// 블록의 좌표를 설정한다.
	void setPosition(GLfloat x, GLfloat y);

	// 블록의 색을 설정한다.
	void setColor(BLOCK_COLOR color);

	// 블록이 받는 힘을 설정한다.
	void setPower(BLOCK_POWER power);

	// 블록이 받는 힘을 반환한다.
	BLOCK_POWER getPower();

	// 블록의 종류를 설정한다
	void setType(BLOCK_TYPE type);

	// 블록의 종류를 반환한다.
	BLOCK_TYPE getType(void);

	// 외부힘이 블록에 미치는 파워를 구한다.
	BLOCK_POWER externalPowerToBlock(BLOCK_POWER power);
};
