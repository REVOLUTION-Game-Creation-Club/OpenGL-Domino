#include "stdafx.h"
#include "Tile.h"

Tile::Tile(void)
{
	m_position[0] = 0.0f;
	m_position[1] = 0.0f;
	m_type = NORMAL;
	m_power = TL_POWER; // 만약 시작 타일이라면 시작 파워를 저장한다.
}

Tile::~Tile(void)
{
}

// 타일을 그린다.
void Tile::draw(void)
{
	glPopMatrix();
	glPushMatrix();
	glTranslatef(m_position[0],m_position[1],0.0f);

	glBegin(GL_POLYGON);
		glColor3f(0.1f,0.1f,0.8f);
		glNormal3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,TILE_SIZE_Y,0.0f);
		glVertex3f(TILE_SIZE_X,TILE_SIZE_Y,0.0f);
		glVertex3f(TILE_SIZE_X,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
	glEnd();
	
	glLineWidth(2.0f);
	glColor3f(0.3f,0.3f,0.9f);
	glBegin(GL_LINE_LOOP);
		glNormal3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,TILE_SIZE_Y,0.0f);
		glVertex3f(TILE_SIZE_X,TILE_SIZE_Y,0.0f);
		glVertex3f(TILE_SIZE_X,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
	glEnd();


	if(m_type == START)	
		drawStartTile();
}

// 타일의 좌표를 설정한다
void Tile::setPosition(GLfloat x, GLfloat y)
{
	m_position[0] = x;
	m_position[1] = y;
}

// 타일의 타입을 설정한다.
void Tile::setType(TILE_TYPE type)
{
	m_type = type;
}

void Tile::setPower(BLOCK_POWER power)
{
	m_power = power;
}

void Tile::drawArrow()
{
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,TILE_SIZE_Y/3, 0.1f);
		glVertex3f(TILE_SIZE_X*2/6,0.0f, 0.1f);
		glVertex3f(TILE_SIZE_X*1/6,0.0f, 0.1f);
		glVertex3f(TILE_SIZE_X*1/6,-TILE_SIZE_Y/3, 0.1f);
		glVertex3f(-TILE_SIZE_X*1/6,-TILE_SIZE_Y/3, 0.1f);
		glVertex3f(-TILE_SIZE_X*1/6,0.0f, 0.1f);
		glVertex3f(-TILE_SIZE_X*2/6,0.0f, 0.1f);
	glEnd();
}

void Tile::drawStartTile()
{
	// 타일 타입에 따라 다른 색으로 보여준다.
	if(m_power == NO_POWER)
		return;

	glPopMatrix();
	glPushMatrix();
	glTranslatef(m_position[0]+TILE_SIZE_X/2,m_position[1]+TILE_SIZE_Y/2,0.0f);

	switch(m_power)
	{
	case LEFT_POWER:
		glRotatef(90.0f,0.0f,0.0f,1.0f);
		break;
	case TL_POWER:
		glRotatef(45.0f,0.0f,0.0f,1.0f);
		break;
	case TOP_POWER:
		break;
	case TR_POWER:
		glRotatef(-45.0f,0.0f,0.0f,1.0f);
		break;
	case RIGHT_POWER:
		glRotatef(-90.0f,0.0f,0.0f,1.0f);
		break; 
	case BR_POWER:
		glRotatef(-135.0f,0.0f,0.0f,1.0f);
		break;
	case BOTTOM_POWER:
		glRotatef(180.0f,0.0f,0.0f,1.0f);
		break;
	case BL_POWER:
		glRotatef(135.0f,0.0f,0.0f,1.0f);
		break;
	}

	drawArrow();
}