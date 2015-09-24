#include "Block.h"
#include "BlockManager.h"
#include "BlockI.h"
#include "BlockL.h"
#include "BlockLR.h"
#include "BlockO.h"
#include "BlockT.h"
#include "BlockZ.h"
#include "BlockZR.h"

Block::Block()
{
	for (int x = 0; x < Block::BlockWidthCount; x++)
	{
		for (int y = 0; y < Block::BlockWidthCount; y++)
		{
			this->m_blockData[x][y] = Vec2(-1, -1);
		}
	}

	this->m_timeCounter = 0;
	this->m_degree = 0;
	//	this->m_position = p(BlockManager::CellMatrixWidth / 2, 2);
} //Block::Block()

Block::~Block()
{

} //Block::~Block()

Block* Block::create(BlockManager* p_blockManagerToBind, int p_index)
{
	Block* pRet = new Block();
	if (pRet && pRet->init(p_blockManagerToBind))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Block::init(BlockManager* p_blockManagerToBind)
{
	this->m_manager = p_blockManagerToBind;
	return true;
} //bool Block::init(BlockManager* p_blockManagerToBind)

Block::CellPosition Block::doTurn90Degrees(bool p_saveState)
{
	CellPosition position; //位置合法则返回位置 非法则把所有位置变为（-1， -1）
	int positionPointer = 0;

// 	Vec2 point00; //block左上角的方块坐标
// #pragma region 获取左上角的block
// 	for (int x = 0; x < Block::BlockWidthCount; x++)
// 	{
// 		for (int y = 0; y < Block::BlockWidthCount; y++)
// 		{
// 			if (this->m_blockData[x][y].x != -1)
// 			{
// 				point00.x = m_blockData[x][y].x - x;
// 				point00.y = m_blockData[x][y].y - y;
// 				break;
// 			}
// 		}
// 	}
// #pragma endregion

	Vec2 tempCellMatrix[Block::BlockWidthCount][Block::BlockWidthCount];
#pragma region 旋转blockData矩阵
	if (this->m_type == BlockType::I && this->m_degree == 90)
	{
		for (int x = 0; x < Block::BlockWidthCount; x++)
		{
			for (int y = 0; y < Block::BlockWidthCount; y++)
			{
				tempCellMatrix[y][Block::BlockWidthCount - x - 1] = this->m_blockData[x][y];
			}
		}
	}
	else
	{
		for (int x = 0; x < Block::BlockWidthCount; x++)
		{
			for (int y = 0; y < Block::BlockWidthCount; y++)
			{
				tempCellMatrix[Block::BlockWidthCount - y - 1][x] = this->m_blockData[x][y];
			}
		}
	}
#pragma endregion

#pragma region 修改矩阵中的数据并保存CellPosition位置 
	for (int x = 0; x < Block::BlockWidthCount; x++)
	{
		for (int y = 0; y < Block::BlockWidthCount; y++)
		{
			if (tempCellMatrix[x][y].x != -1)
			{
				tempCellMatrix[x][y].x = position.points[positionPointer].x = this->m_position0_0.x + x;
				tempCellMatrix[x][y].y = position.points[positionPointer].y = this->m_position0_0.y + y;
				positionPointer++;
			}
		}
	}
#pragma endregion

	if (p_saveState)
	{
		if (this->m_type == BlockType::I && this->m_degree == 90)
		{
			this->m_degree = 0;
		}
		else
		{
			this->m_degree = (this->m_degree + 90) % 360;
		}

		for (int x = 0; x < Block::BlockWidthCount; x++)
		{
			for (int y = 0; y < Block::BlockWidthCount; y++)
			{
				this->m_blockData[x][y] = tempCellMatrix[x][y];
			}
		}
	}

	return position;
} //bool Block::doTurn90Degrees()

bool Block::doMove(Direction direction)
{
	if (direction == Direction::Down)
	{
		for (int x = 0; x < Block::BlockWidthCount; x++)
		{
			for (int y = 0; y < Block::BlockWidthCount; y++)
			{
				if (this->m_blockData[x][y].x != -1)
				{
					this->m_blockData[x][y].y++;
				}
			}
		}
		this->m_position0_0.y++;
	}
	else if (direction == Direction::Left)
	{
		for (int x = 0; x < Block::BlockWidthCount; x++)
		{
			for (int y = 0; y < Block::BlockWidthCount; y++)
			{
				if (this->m_blockData[x][y].x != -1)
				{
					this->m_blockData[x][y].x--;
				}
			}
		}
		this->m_position0_0.x--;
	}
	else if (direction == Direction::Right)
	{
		for (int x = 0; x < Block::BlockWidthCount; x++)
		{
			for (int y = 0; y < Block::BlockWidthCount; y++)
			{
				if (this->m_blockData[x][y].x != -1)
				{
					this->m_blockData[x][y].x++;
				}
			}
		}
		this->m_position0_0.x++;
	}
	else
	{
		for (int x = 0; x < Block::BlockWidthCount; x++)
		{
			for (int y = 0; y < Block::BlockWidthCount; y++)
			{
				if (this->m_blockData[x][y].x != -1)
				{
					this->m_blockData[x][y].y--;
				}
			}
		}
		this->m_position0_0.y--;
	}
	return true;
} //bool Block::doMove(Direction direction)

void Block::bindManager(BlockManager* manager)
{
	this->m_manager = manager;
} //void Block::bindManager(BlockManager* manager)

bool Block::increaseTimeCounter(int updateTime)
{
	this->m_timeCounter++;
	if (this->m_timeCounter >= updateTime)
	{
		this->m_timeCounter = 0;
		return true;
	}
	else
	{
		return false;
	}
}// bool Block::increaseTimeCounter()

Block* Block::generateNewBlock(BlockManager* p_blockManagerToBind, int p_index)
{
	Block* block = NULL;

	
	int blockType = RandomHelper::random_int(0, 6);

	switch (blockType)
	{
	case 0:
		block = BlockI::create(p_blockManagerToBind, p_index);
		break;

	case 1:
		block = BlockL::create(p_blockManagerToBind, p_index);
		break;

	case 2:
		block = BlockLR::create(p_blockManagerToBind, p_index);
		break;

	case 3:
		block = BlockO::create(p_blockManagerToBind, p_index);
		break;

	case 4:
		block = BlockT::create(p_blockManagerToBind, p_index);
		break;

	case 5:
		block = BlockZ::create(p_blockManagerToBind, p_index);
		break;

	case 6:
		block = BlockZR::create(p_blockManagerToBind, p_index);
		break;

	default:
		break;
	}
	return block;
}

Block::CellPosition Block::getCellPosition()
{
	CellPosition position;
	int counter = 0;
	for (int x = 0; x < Block::BlockWidthCount; x++)
	{
		for (int y = 0; y < Block::BlockWidthCount; y++)
		{
			if (this->m_blockData[x][y].x != -1)
			{
				position.points[counter++] = this->m_blockData[x][y];
			}
		}
	}
	return position;
} //CellPosition Block::getCellPosition()

void Block::setBlockData(Vec2 blockPoint, Vec2 stagePoint)
{
	this->m_blockData[(int)blockPoint.x][(int)blockPoint.y] = stagePoint;
} //void Block::setBlockData(Vec2 blockPoint, Vec2 stagePoint)

Block::BlockType Block::getBlockType()
{
	return this->m_type;
}

int Block::getDegree()
{
	return this->m_degree;
}


