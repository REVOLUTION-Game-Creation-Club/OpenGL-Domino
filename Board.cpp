#include "stdafx.h"
#include "Board.h"
//#include "../RevSound/SoundManager.h"

extern HWND g_hDlg;

//using namespace rev;

Board::Board(void)
{
	srand((unsigned int)time(NULL));

	for(int w = 0; w <TILE_WIDTH; w++)
	{
		for(int h = 0; h <TILE_HEIGHT; h++)
		{
			GLfloat x = -TILE_SIZE_X*TILE_WIDTH/2 + w*TILE_SIZE_X;
			GLfloat y = -TILE_SIZE_Y*TILE_HEIGHT/2 + h*TILE_SIZE_Y;
			m_tile[w][h].setPosition(x, y);
			m_block[w][h].setPosition(x+(GLfloat)((TILE_SIZE_X-BLOCK_SIZE_X)/2),y+(GLfloat)((TILE_SIZE_Y-BLOCK_SIZE_Y)/2));
		}
	}

	m_startTile.x = 0;
	m_startTile.y = 0;
	m_startPower = TL_POWER;

	setStartTile(m_startTile.x,m_startTile.x);
}

Board::~Board(void)
{
}

// 타일에 블록을 놓는다
void Board::setBlockOnTile(POINT pos, BLOCK_TYPE type)
{
	BLOCK_COLOR color = (BLOCK_COLOR)(rand()%MAX_COLOR);
	m_block[pos.x][pos.y].setColor(color);
	m_block[pos.x][pos.y].setType(type);
	m_block[pos.x][pos.y].setPower(NO_POWER);
}

// 다음 블록이 졵재하는지 확인한다.
bool Board::isNextBlock(POINT index, BLOCK_POWER power)
{
	switch(power)
	{
	case NO_POWER:
		return false;
	case LEFT_POWER:
		if(index.x == 0)
			return false;
		else
			return true;
	case TL_POWER:
		if((index.x == 0)||(index.y == (TILE_HEIGHT -1)))
			return false;
		else
			return true;
	case TOP_POWER:
		if(index.y == (TILE_HEIGHT -1))
			return false;
		else
			return true;
	case TR_POWER:
		if((index.x == (TILE_WIDTH - 1))||(index.y == (TILE_HEIGHT -1)))
			return false;
		else
			return true;
	case RIGHT_POWER:
		if(index.x == (TILE_WIDTH - 1))
			return false;
		else
			return true;
	case BR_POWER:
		if(index.x == (TILE_WIDTH - 1)||(index.y == 0))
			return false;
		else
			return true;
	case BOTTOM_POWER:
		if(index.y == 0)
			return false;
		else
			return true;
	case BL_POWER:
		if((index.y == 0)||(index.x == 0))
			return false;
		else
			return true;
	}
	return false; // 에러
}

// 다음 블럭의 인덱스를 구한다.
POINT Board::getNextIndex(POINT index, BLOCK_POWER power)
{
	POINT nextIndex;		// 다음 블록 인덱스
	switch(power)
	{
	case NO_POWER:		// 다음 블록이 없다면 ... 에러...
		nextIndex.x = index.x;
		nextIndex.y = index.y;
		break;
	case LEFT_POWER:
		nextIndex.x = index.x - 1;
		nextIndex.y = index.y;
		break;
	case TL_POWER:
		nextIndex.x = index.x - 1;
		nextIndex.y = index.y + 1;
		break;
	case TOP_POWER:
		nextIndex.x = index.x;
		nextIndex.y = index.y + 1;
		break;
	case TR_POWER:
		nextIndex.x = index.x + 1;
		nextIndex.y = index.y + 1;
		break;
	case RIGHT_POWER:
		nextIndex.x = index.x + 1;
		nextIndex.y = index.y;
		break;
	case BR_POWER:
		nextIndex.x = index.x + 1;
		nextIndex.y = index.y - 1;
		break;
	case BOTTOM_POWER:
		nextIndex.x = index.x;
		nextIndex.y = index.y - 1;
		break;
	case BL_POWER:
		nextIndex.x = index.x - 1;
		nextIndex.y = index.y - 1;
		break;
	}

	return nextIndex;
}

// 도미노 쓰러지는 한번의 처리 ...처리가 끝났는지 여부를 리턴한다.
bool Board::startUpdate()
{
	gluLookAt(-TILE_SIZE_X*TILE_WIDTH/2,-TILE_SIZE_Y*TILE_HEIGHT/2,TILE_SIZE_Y*TILE_HEIGHT/2, 
				TILE_SIZE_X*TILE_WIDTH/2,TILE_SIZE_Y*TILE_HEIGHT/2,0.0f,
			0.33f,0.33f,0.33f);

	glTranslatef(-tile.x*TILE_SIZE_X,-tile.y*TILE_SIZE_Y,10.0f);

	// 외부의 힘이 블록에 미치는 실제 힘을 구한다.
	power = m_block[tile.x][tile.y].externalPowerToBlock(power);

	// 블록에 힘을 설정한다. --- 블록이 넘어진다.
	m_block[tile.x][tile.y].setPower(power);

	// 다음 위치에 타일 존재할 수 없으면 끝낸다.
	if(!isNextBlock(tile, m_block[tile.x][tile.y].getPower()))
		return true;

	// 다음 타일의 인덱스를 구한다.
	POINT next = getNextIndex(tile, power);

	// 다음 타일 위치에 타일이 없으면 끝낸다.
	if(m_block[next.x][next.y].getType() == TYPE0)
		return true;

	// 다음 타일이 이미 힘을 받았으면(쓰려졌으면)...끝낸다.
	if(m_block[next.x][next.y].getPower() != NO_POWER)
		return true;

	// 그 외의 경우 이면... 세워진 블록이 있는 경우이다.
	tile = next;
	// 다음 타일로 진행하기전에 정지 시간을 둔다.
	Sleep(DELAY_TIME);

//	getSoundManager()->play("block");
	return false;
}

// 도미노를 쓰러뜨린다.
void Board::start(void)
{

	tile.x = m_startTile.x;
	tile.y = m_startTile.y;
	power = m_startPower;
}

// 타일들을 그린다.
void Board::draw(void)
{
	for(int w = 0; w <TILE_WIDTH; w++)
	{
		for(int h = 0; h <TILE_HEIGHT; h++)
		{
			m_tile[w][h].draw();
			m_block[w][h].draw();
		}
	}

	//glPopMatrix();
	//// 중심점의 축을 그린다.
	//glLineWidth(3.0f);
	//glBegin(GL_LINES);
	////glColor3f(1.0f,0.0f,0.0f);
	////glVertex3f(0.0f,0.0f,0.0f);
	////glVertex3f(TILE_SIZE_X*TILE_WIDTH,0.0f,0.0f);
	////glColor3f(0.0f,1.0f,0.0f);
	////glVertex3f(0.0f,0.0f,0.0f);
	////glVertex3f(0.0f,TILE_SIZE_Y*TILE_HEIGHT,0.0f);
	//glColor3f(0.0f,1.0f,0.0f);
	//glVertex3f(0.0f,0.0f,0.0f);
	//glVertex3f(0.0f,0.0f,30.0f);
	//glEnd();

//	m_tile[0][0].draw();
}

// 해당하는 인덱스의 블록을 리턴한다.
Block& Board::getBlock(int x, int y)
{
	if((x < 0)||(x > TILE_WIDTH ) || (y < 0)||(y > TILE_HEIGHT ))
		MessageBox(g_hDlg,"잘못된 블럭 인덱스 입니다.",NULL,NULL);
	
	return m_block[x][y];
}

// 시작 타일 인덱스 좌표를 설정한다
void Board::setStartTile(int x, int y)
{
	// 이전 시작 타일을 일반 타일로 바꾼다.
	m_tile[m_startTile.x][m_startTile.y].setType(NORMAL);
	m_tile[m_startTile.x][m_startTile.y].setPower(NO_POWER);

	m_startTile.x = x;
	m_startTile.y = y;
	m_tile[m_startTile.x][m_startTile.y].setType(START);

	switch(m_startPower)
	{
	case LEFT_POWER:
		m_startPower = TL_POWER;
		break;
	case TL_POWER:
		m_startPower = TOP_POWER;
		break;
	case TOP_POWER:
		m_startPower = TR_POWER;
		break;
	case TR_POWER:
		m_startPower = RIGHT_POWER;
		break;
	case RIGHT_POWER:
		m_startPower = BR_POWER;
		break;
	case BR_POWER:
		m_startPower = BOTTOM_POWER;
		break;
	case BOTTOM_POWER:
		m_startPower = BL_POWER;
		break;
	case BL_POWER:
		m_startPower = LEFT_POWER;
		break;
	default:
		m_startPower = TOP_POWER;
		break;
	}
	m_tile[m_startTile.x][m_startTile.y].setPower(m_startPower);
}

void Board::reset(void)
{
	for(int w = 0; w <TILE_WIDTH; w++)
	{
		for(int h = 0; h <TILE_HEIGHT; h++)
		{
			m_block[w][h].setType(TYPE0);
			m_block[w][h].setPower(NO_POWER);
		}
	}
}

void Board::stand(void)
{
	for(int w = 0; w <TILE_WIDTH; w++)
	{
		for(int h = 0; h <TILE_HEIGHT; h++)
		{
			m_block[w][h].setPower(NO_POWER);
		}
	}
}
