#include "BlockManagerDoubleBlock.h"

BlockManagerDoubleBlock::BlockManagerDoubleBlock()
{

} //BlockManagerDoubleBlock::BlockManagerDoubleBlock()

BlockManagerDoubleBlock::~BlockManagerDoubleBlock()
{

} //BlockManagerDoubleBlock::~BlockManagerDoubleBlock()

BlockManagerDoubleBlock* BlockManagerDoubleBlock::create(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerToBind)
{
	BlockManagerDoubleBlock* pRet = new BlockManagerDoubleBlock();
	if (pRet && pRet->init(p_pointToDisplayOnScreen, p_displayManagerToBind))
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
} //BlockManagerDoubleBlock* BlockManagerDoubleBlock::create(Vec2 position)

bool BlockManagerDoubleBlock::init(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerToBind)
{
	bool bRet = true;
	do
	{
		this->setPosition(p_pointToDisplayOnScreen);

// 		this->m_boundDisplayManager = p_displayManagerToBind;
// 		if (m_boundDisplayManager == NULL)
// 		{
// 			bRet = false;
// 			break;
// 		}

		Sprite* backgroundImage = Sprite::create("Background.png");
		if (!backgroundImage)
		{
			bRet = false;
			break;
		}
		this->addChild(backgroundImage);

		//初始化单元状态矩阵
		for (int x = 0; x < BlockManagerDoubleBlock::CellMatrixWidth; x++)
		{
			for (int y = 0; y < BlockManagerDoubleBlock::CellMatrixHeight; y++)
			{
				this->m_matrixCellState[x][y] = BlockManagerDoubleBlock::CellState::Empty;
				this->m_matrixDeadBlockSprites[x][y] = NULL;
			}
		}

		//设定默认更新频率
		this->m_periodDropping = BlockManagerDoubleBlock::DefaultRefreshTime;

		//创建第一个块
		this->m_blockWaitingD[0] = _createNewBlock();
		this->m_blockWaitingD[0]->retain();
		this->m_blockWaitingD[1] = _createNewBlock();
		this->m_blockWaitingD[1]->retain();
		this->m_blockDroppingD[0] = nullptr;
		this->m_blockDroppingD[1] = nullptr;
		this->m_blockWaitingToAdd = nullptr;
		this->m_indexOfCurrentBlock = 0;

		//放入第一个块并创建第二个块
		this->_pushNewBlock(0);
		this->_pushNewBlock(1);

		if (this->m_blockWaitingD[0] == NULL ||
			this->m_blockWaitingD[1] == NULL ||
			this->m_blockDroppingD[0] == NULL ||
			this->m_blockDroppingD[1] == NULL)
		{
			bRet = false;
			break;
		}

		//初始化已死方块批量精灵
		this->m_spriteBatchNodeDeadBlocks = SpriteBatchNode::create("BlockDead.png", BlockManagerDoubleBlock::CellMatrixWidth * BlockManagerDoubleBlock::CellMatrixHeight);
		if (this->m_spriteBatchNodeDeadBlocks == NULL)
		{
			bRet = false;
			break;
		}
		this->addChild(this->m_spriteBatchNodeDeadBlocks);

		//初始化键盘控制
		this->m_isDropReqiured = false;

	} while (0);

	if (bRet)
	{
		this->scheduleUpdate();
		this->_rePaintDeadBlocks();
	}
	else
	{
		if (this->m_boundDisplayManager)
		{
			this->m_boundDisplayManager->autorelease();
			this->m_boundDisplayManager = NULL;
		}

		if (this->m_spriteBatchNodeDeadBlocks)
		{
			this->m_spriteBatchNodeDeadBlocks->autorelease();
			this->m_spriteBatchNodeDeadBlocks = NULL;
		}
	}

	return bRet;

}

void BlockManagerDoubleBlock::update(float delta)
{
	//To judge which one is on a lower height
	int y0_1 = this->m_blockDroppingD[0]->getCenterPointInMatrix().y -
		this->m_blockDroppingD[1]->getCenterPointInMatrix().y;  

	//If block_0 is lower
	int i = (y0_1 >= 0) ? 0 : 1;
	int stepper = (i > 0) ? -1 : 1;

	//Iterate both blocks from the lower one
	for (; i >= 0 && i <= 1 ; i += stepper)
	{
		if (!this->_blockShouldTryPeriodicalDrop(this->m_blockDroppingD[i],
			(this->m_isDropReqiured) && (i == this->m_indexOfCurrentBlock)
			? BlockManagerDoubleBlock::RequiredUpdateTime 
			: this->m_periodDropping))
		{
			this->_blockDonotTryDrop(this->m_blockDroppingD[i]);
		}
		else
		{
			this->_blockDoTryMove(this->m_blockDroppingD[i], Block::Direction::Down);

			if (this->m_blockWaitingToAdd != nullptr)
			{
				Block::CellPosition position = this->m_blockWaitingToAdd->getCellPosition();
				bool isAbleToAdd = true;
				for (int i = 0; i < 4; i++)
				{
					if (this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y] != BlockManagerDoubleBlock::CellState::Empty)
					{
						isAbleToAdd = false;
						break;
					}
				}

				if (isAbleToAdd)
				{
					this->addChild(m_blockWaitingToAdd);
					this->m_blockWaitingToAdd = nullptr;
				}
			}
		}
	}

	

} //void BlockManagerDoubleBlock::update(float delta)

void BlockManagerDoubleBlock::triggerKeyboard(EventKeyboard::KeyCode p_valueKey)
{
	switch (p_valueKey)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		this->_blockDoTryMove(this->m_blockDroppingD[this->m_indexOfCurrentBlock], Block::Direction::Left);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		this->_blockDoTryMove(this->m_blockDroppingD[this->m_indexOfCurrentBlock], Block::Direction::Right);
		break; 

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		this->m_isDropReqiured = true;
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		this->_blockDoTryTurn90Degrees(this->m_blockDroppingD[this->m_indexOfCurrentBlock]);
		break; 

	case EventKeyboard::KeyCode::KEY_TAB:
		this->m_indexOfCurrentBlock = 1 - this->m_indexOfCurrentBlock;
		break; 

	default:
		break;
	}
} //void BlockManagerDoubleBlock::triggerKeyboard(EventKeyboard::KeyCode p_valueKey)

Vec2 BlockManagerDoubleBlock::getBlockStartPosition0_0InMatrix(int p_indexOfBlock)
{
	Vec2 result;
	result.y = 0;
	result.x = (p_indexOfBlock == 1)
		? this->CellMatrixWidth / 2 - Block::BlockWidthCount / 2 - 2
		: this->CellMatrixWidth / 2 - Block::BlockWidthCount / 2 + 2;
	return result;

} //Vec2 BlockManagerDoubleBlock::getBlockStartPosition0_0InMatrix(int p_indexOfBlock)

void BlockManagerDoubleBlock::_pushNewBlock(int p_index)
{
	this->m_blockDroppingD[p_index] = this->m_blockWaitingD[0];
	this->m_blockDroppingD[p_index]->setIndex(p_index);

	this->m_blockDroppingD[p_index]->setPosition(
		this->convertBlockPositionToPixelPosition(
			this->m_blockDroppingD[p_index]->getCenterPointInMatrix()));

	log(this->m_blockDroppingD[1 - p_index] == NULL ? "True" : "False");

	if (this->m_blockDroppingD[1 - p_index] == NULL ||
		!this->_blockOverlayed(
			this->m_blockDroppingD[0]->getCellPosition(),
			this->m_blockDroppingD[1]->getCellPosition()))
	{
		this->addChild(this->m_blockDroppingD[p_index]);
		Block::CellPosition position = this->m_blockDroppingD[p_index]->getCellPosition();

		for (int i = 0; i < 4; i++)
		{
			this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y] = BlockManager::CellState::Dropping;
		}
	}
	else
	{
		this->m_blockWaitingToAdd = this->m_blockDroppingD[p_index];
	}

	this->m_blockWaitingD[0] = this->m_blockWaitingD[1];
	this->m_blockWaitingD[1] = this->_createNewBlock();
	this->m_blockWaitingD[1]->retain();

	//notice display manager


} //void BlockManagerDoubleBlock::_pushNewBlock(int p_index)

void BlockManagerDoubleBlock::_blockStopDrop(Block* p_block)
{
	this->m_isDropReqiured = false;
	int blockIndex = p_block->getIndex();

	Block::CellPosition blockPrePosition = p_block->getCellPosition();

	this->_updateCellMatrixForBlockBeforeDie(p_block);

	this->_rePaintDeadBlocks();
	this->_eliminateLines();

	this->removeChild(p_block, true);

	if (p_block)
	{
		p_block->autorelease();
	}

	this->_pushNewBlock(blockIndex);
	
	Block::CellPosition blockCurrentPosition = this->m_blockDroppingD[blockIndex]->getCellPosition();

	if (this->_blockOverlayed(blockPrePosition, blockCurrentPosition))
	{
		this->_endGame();
	}

} //void BlockManagerDoubleBlock::_blockStopDrop(Block* p_block)