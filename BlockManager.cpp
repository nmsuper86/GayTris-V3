#include "BlockManager.h"


#pragma region 系统调用

BlockManager::BlockManager()
{

} //BlockManager::BlockManager()

BlockManager::~BlockManager()
{

} //BlockManager::~BlockManager()

BlockManager* BlockManager::create(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerToBind)
{
	BlockManager* pRet = new BlockManager();
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
} //BlockManager* BlockManager::create(Vec2 position)

bool BlockManager::init(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerToBind)
{
	bool bRet = true;
	do
	{
		this->setPosition(p_pointToDisplayOnScreen);

		this->m_boundDisplayManager = p_displayManagerToBind;
		if (m_boundDisplayManager == NULL)
		{
			bRet = false;
			break;
		}

		Sprite* backgroundImage = Sprite::create("Background.png");
		if (!backgroundImage)
		{
			bRet = false;
			break;
		}
		this->addChild(backgroundImage);

		//初始化单元状态矩阵
		for (int x = 0; x < BlockManager::CellMatrixWidth; x++)
		{
			for (int y = 0; y < BlockManager::CellMatrixHeight; y++)
			{
				this->m_matrixCellState[x][y] = BlockManager::CellState::Empty;
				this->m_matrixDeadBlockSprites[x][y] = NULL;
			}
		}

		//设定默认更新频率
		this->m_periodDropping = BlockManager::DefaultRefreshTime;

		//创建第一个块
		this->m_blockWaiting = _createNewBlock();
		this->m_blockWaiting->retain();

		//放入第一个块并创建第二个块
		this->_pushNewBlock();

		if (this->m_blockWaiting == NULL || this->m_blockDropping == NULL)
		{
			bRet = false;
			break;
		}

		//初始化已死方块批量精灵
		this->m_spriteBatchNodeDeadBlocks = SpriteBatchNode::create("BlockDead.png", BlockManager::CellMatrixWidth * BlockManager::CellMatrixHeight);
		// 		this->m_spriteBatchNodeDeadBlocks->setAnchorPoint(p(0.5, 0.5));
		// 		this->m_spriteBatchNodeDeadBlocks->setPosition(this->getAnchorPoint());
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

void BlockManager::update(float delta)
{
	if (!this->_blockShouldTryPeriodicalDrop(this->m_blockDropping, 
		this->m_isDropReqiured ? BlockManager::RequiredUpdateTime : this->m_periodDropping))
	{
		this->_blockDonotTryDrop(this->m_blockDropping);
	}
	else
	{
		this->_blockDoTryMove(this->m_blockDropping, Block::Direction::Down);
	}
} //void BlockManager::update(float delta)

void BlockManager::triggerKeyboard(EventKeyboard::KeyCode p_valueKey)
{
	switch (p_valueKey)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		this->_blockDoTryMove(this->m_blockDropping, Block::Direction::Left);
		break; //case VK_LEFT

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		this->_blockDoTryMove(this->m_blockDropping, Block::Direction::Right);
		break; //case VK_RIGHT

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		this->_blockDoTryRequiredDrop(this->m_blockDropping);
		this->m_isDropReqiured = true;
		break; //case VK_DOWN

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		this->_blockDoTryTurn90Degrees(this->m_blockDropping);
		break; //case VK_UP

	default:
		break;
	}
} //void BlockManager::triggerKeyboard(int p_valueKey)

#pragma endregion

#pragma region 公有自定义函数

int BlockManager::getUpdateTime()
{
	return this->m_periodDropping;
} //int BlockManager::getUpdateTime()

void BlockManager::setUpdateTime(int p_updateTime)
{
	this->m_periodDropping = p_updateTime;
} //void BlockManager::setUpdateTime(int time)

Vec2 BlockManager::convertBlockPositionToPixelPosition(Vec2 p_pointBlock)
{
	Vec2 pointResult;
	pointResult.x = -BlockManager::CellMatrixWidth / 2 * Block::CellSize + p_pointBlock.x * Block::CellSize;
	pointResult.y = BlockManager::CellMatrixHeight / 2 * Block::CellSize - (p_pointBlock.y + 1) * Block::CellSize;
	return pointResult;
} //Vec2 BlockManager::convertBlockToPixel(Vec2 blockPoint)

void BlockManager::setDownKeyState(bool p_isKeyPressed)
{
	this->m_isDropReqiured = p_isKeyPressed;
} //void BlockManager::setKeyDownState(bool keyDownState)

Vec2 BlockManager::getBlockStartPosition0_0InMatrix(int p_indexOfBlock)
{
	Vec2 result;
	result.y = 0;
	result.x = BlockManager::CellMatrixWidth / 2 - Block::BlockWidthCount / 2 + p_indexOfBlock * Block::BlockWidthCount;
	return result;
} //Vec2 BlockManager::getBlockStartPositionInMatrix(int p_indexOfBlock)

#pragma endregion

#pragma region 私有自定义函数

Block* BlockManager::_createNewBlock()
{
	Block* block = Block::generateNewBlock(this, 0);
	return block;
}

void BlockManager::_pushNewBlock()
{
	this->m_blockDropping = this->m_blockWaiting;
	this->m_blockDropping->setPosition(this->convertBlockPositionToPixelPosition(Vec2(BlockManager::CellMatrixWidth / 2, Block::BlockWidthCount / 2 - 1)));

	Block::CellPosition position = this->m_blockDropping->getCellPosition();
	for (int i = 0; i < 4; i++)
	{
		this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y] = BlockManager::CellState::Dropping;
	}
	this->addChild(this->m_blockDropping);

	this->m_blockWaiting = this->_createNewBlock();
	this->m_blockWaiting->retain();
	this->m_boundDisplayManager->nextBlockChanged(this->m_blockWaiting);
} //void BlockManager::_pushNewBlock()

  /*******************************************************/

bool BlockManager::_blockShouldTryPeriodicalDrop(Block* p_block, int p_updateTime)
{
	return p_block->increaseTimeCounter(p_updateTime);
}

void BlockManager::_blockDonotTryDrop(Block* p_block)
{
	//Nothing
} //void BlockManager::_donotTryDrop()

void BlockManager::_blockDoTryMove(Block* p_block, Block::Direction p_direction)
{
	if (this->_blockCanMove(p_block, p_direction))
	{
		this->_blockDoMove(p_block, p_direction);
	}
	else if (p_direction == Block::Direction::Down)
	{
		this->_blockStopDrop(p_block);
	}
} //void BlockManager::_doTryMove(Block::Direction direction)

void BlockManager::_blockDoTryRequiredDrop(Block* p_block)
{
	if (!this->_blockShouldTryPeriodicalDrop(p_block, BlockManager::RequiredUpdateTime))
	{
		this->_blockDonotTryDrop(p_block);
	}
	else
	{
		this->_blockDoTryMove(p_block, Block::Direction::Down);
	}
} //void BlockManager::_doTryRequiredDrop()

void BlockManager::_blockDoTryTurn90Degrees(Block* p_block)
{
	Block::BlockType blockType = p_block->getBlockType();
	if (blockType != Block::O)
	{
		if (this->_blockCanTurn90Degrees(p_block))
		{
			this->_blockDoTurn90Degrees(p_block);
		}
	}
} //void BlockManager::_doTryTurn90Degrees()

  /********************************************************/

bool BlockManager::_blockCanMove(Block* p_block, Block::Direction p_direction)
{
	Block::CellPosition position = p_block->getCellPosition();

	switch (p_direction)
	{
	case Block::Direction::Down:
		for (int i = 0; i < 4; i++)
		{
			if ((position.points[i].y >= BlockManager::CellMatrixHeight - 1) ||
				(this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y + 1] == CellState::Dead))
			{
				return false;
			}
		}
		break;

	case Block::Direction::Left:
		for (int i = 0; i < 4; i++)
		{
			if ((position.points[i].x <= 0) ||
				(this->m_matrixCellState[(int)position.points[i].x - 1][(int)position.points[i].y] == CellState::Dead))
			{
				return false;
			}
		}
		break;

	case Block::Direction::Right:
		for (int i = 0; i < 4; i++)
		{
			if ((position.points[i].x >= BlockManager::CellMatrixWidth - 1) ||
				(this->m_matrixCellState[(int)position.points[i].x + 1][(int)position.points[i].y] == CellState::Dead))
			{
				return false;
			}
		}
		break;

	case Block::Direction::Up:
		for (int i = 0; i < 4; i++)
		{
			if ((position.points[i].y <= 0) ||
				(this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y - 1] == CellState::Dead))
			{
				return false;
			}
		}

	default:
		break;
	}

	return true;
} //bool BlockManager::_currentBlockCanDrop()

void BlockManager::_blockDoMove(Block* p_block, Block::Direction p_direction)
{
	this->_updateCellMatrixForBlockBeforeMove(p_block, p_direction);
	p_block->doMove(p_direction);

	if (p_direction == Block::Direction::Down)
	{
		float y = this->m_blockDropping->getPositionY();
		p_block->setPositionY((int)y - Block::CellSize);
	}
	else if (p_direction == Block::Direction::Left)
	{
		float x = this->m_blockDropping->getPositionX();
		p_block->setPositionX((int)x - Block::CellSize);
	}
	else if (p_direction == Block::Direction::Right)
	{
		float x = this->m_blockDropping->getPositionX();
		p_block->setPositionX((int)x + Block::CellSize);
	}
	else
	{
		float y = this->m_blockDropping->getPositionY();
		p_block->setPositionY((int)y + Block::CellSize);
	}
} //void BlockManager::_currentBlockDoDrop()

void BlockManager::_blockStopDrop(Block* p_block)
{
	this->m_isDropReqiured = false;

	Block::CellPosition blockPrePosition = p_block->getCellPosition();

	this->_updateCellMatrixForBlockBeforeDie(p_block);

	this->_rePaintDeadBlocks();
	this->_eliminateLines();

	this->removeChild(p_block, false);

	if (p_block)
	{
		p_block->autorelease();
	}

//	this->m_blockDropping = NULL;

	this->_pushNewBlock();

	Block::CellPosition blockCurrentPosition = this->m_blockDropping->getCellPosition();

	if (this->_blockOverlayed(blockPrePosition, blockCurrentPosition))
	{
		this->_endGame();
	}
}

bool BlockManager::_blockCanTurn90Degrees(Block* p_block)
{
	if (p_block->getBlockType() == Block::BlockType::O)
	{
		return false;
	}

	Block::CellPosition turnedPosition = p_block->doTurn90Degrees(false);

	for (int i = 0; i < 4; i++)
	{
		if (turnedPosition.points[i].x < 0 ||
			turnedPosition.points[i].y < 0 ||
			turnedPosition.points[i].x > BlockManager::CellMatrixWidth - 1 ||
			turnedPosition.points[i].y > BlockManager::CellMatrixHeight - 1 ||
			this->m_matrixCellState[(int)turnedPosition.points[i].x][(int)turnedPosition.points[i].y] == BlockManager::CellState::Dead)
		{
			return false;
		}
	}

	return true;

} //bool BlockManager::_currentBlockCanTurn90Degrees()

void BlockManager::_blockDoTurn90Degrees(Block* p_block)
{
	Block::CellPosition originalPosition = p_block->getCellPosition();

	p_block->doTurn90Degrees(true);

	Block::CellPosition newPosition = p_block->getCellPosition();

	this->_updateCellMatrixAfterChanged(originalPosition, newPosition);

	p_block->getSprite()->setRotation(p_block->getDegree());
} //void BlockManager::_currentBlockDoTurn90Degrees()

  /********************************************************/

void BlockManager::_updateCellMatrixForBlockBeforeMove(Block* p_block, Block::Direction p_direction)
{
	Block::CellPosition position = p_block->getCellPosition();

	for (int i = 0; i < 4; i++)
	{
		this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y] = BlockManager::CellState::Empty;

		if (p_direction == Block::Direction::Down)
		{
			this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y + 1] = BlockManager::CellState::Dropping;
		}
		else if (p_direction == Block::Direction::Left)
		{
			this->m_matrixCellState[(int)position.points[i].x - 1][(int)position.points[i].y] = BlockManager::CellState::Dropping;
		}
		else if (p_direction == Block::Direction::Right)
		{
			this->m_matrixCellState[(int)position.points[i].x + 1][(int)position.points[i].y] = BlockManager::CellState::Dropping;
		}
		else
		{
			this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y - 1] = BlockManager::CellState::Dropping;
		}
	}

} //void BlockManager::_updateCellMatrixForMove()

void BlockManager::_updateCellMatrixForBlockBeforeDie(Block* p_block)
{
	Block::CellPosition position = p_block->getCellPosition();

	for (int i = 0; i < 4; i++)
	{
		this->m_matrixCellState[(int)position.points[i].x][(int)position.points[i].y] = BlockManager::CellState::Dead;
	}
} //void BlockManager::_updateCellMatrixForDie()

void BlockManager::_updateCellMatrixAfterChanged(Block::CellPosition originalPosition, Block::CellPosition newPosition)
{
	for (int i = 0; i < 4; i++)
	{
		this->m_matrixCellState[(int)originalPosition.points[i].x][(int)originalPosition.points[i].y] = CellState::Empty;
	}
	for (int i = 0; i < 4; i++)
	{
		this->m_matrixCellState[(int)newPosition.points[i].x][(int)newPosition.points[i].y] = CellState::Dropping;
	}
}

int BlockManager::_eliminateLines()
{
	int lineCounter = 0;
	int lineNums[Block::BlockWidthCount];

	for (int i = 0; i < BlockManager::CellMatrixHeight; i++)
	{
		if (this->_isLineFilled(i))
		{
			lineNums[lineCounter] = i;
			this->_eliminateSingleLine(i);
			lineCounter++;
		}
	}

	if (lineCounter > 0)
	{
		this->m_boundDisplayManager->lineEliminated(lineCounter, this);
		this->_rePaintDeadBlocks();
	}

	return lineCounter;
} //void BlockManager::_eliminateLines()

bool BlockManager::_blockOverlayed(Block::CellPosition p_block1, Block::CellPosition p_block2)
{
	for (int i1 = 0; i1 < 4; i1++)
	{
		for (int i2 = 0; i2 < 4; i2++)
		{
			if (p_block1.points[i1].x == p_block2.points[i2].x && p_block1.points[i1].y == p_block2.points[i2].y)
			{
				return true;
			}
		}
	}
	return false;
} //bool BlockManager::_blockOverlayed(Block::CellPosition p_block1, Block::CellPosition p_block2)

  //Unfinished
void BlockManager::_endGame()
{
	this->unscheduleUpdate();
	cocos2d::log("END");
} //void BlockManager::_endGame()


  /********************************************************/

  //Unfinished
void BlockManager::_eliminateSingleLine(int lineNum)
{
	for (int x = 0; x < BlockManager::CellMatrixWidth; x++)
	{
		for (int y = lineNum; y >= 0; y--)
		{
			if (y == 0)
			{
				this->m_matrixCellState[x][y] = BlockManager::CellState::Empty;
				this->m_matrixDeadBlockSprites[x][y] = NULL;
			}
			else
			{
				this->m_matrixCellState[x][y] = this->m_matrixCellState[x][y - 1];
				this->m_matrixDeadBlockSprites[x][y] = this->m_matrixDeadBlockSprites[x][y - 1];
			}
		}
	}
} //void BlockManager::_eliminateSingleLine(int lineNum)

  //Unfinished
void BlockManager::_isTetris()
{

} //void BlockManager::_isTetris(int startLine)

void BlockManager::_rePaintDeadBlocks()
{
	this->m_spriteBatchNodeDeadBlocks->removeAllChildren();

	for (int x = 0; x < BlockManager::CellMatrixWidth; x++)
	{
		for (int y = 0; y < BlockManager::CellMatrixHeight; y++)
		{
			if (this->m_matrixCellState[x][y] == CellState::Dead)
			{
				Sprite* temp = Sprite::createWithTexture(this->m_spriteBatchNodeDeadBlocks->getTexture());
				temp->setPosition(this->convertPositionToCenterPosition(convertBlockPositionToPixelPosition(Vec2(x, y))));
				this->m_spriteBatchNodeDeadBlocks->addChild(temp);
				this->m_matrixDeadBlockSprites[x][y] = temp;
			}
		}
	}
} //void BlockManager::_rePaintDeadBlocks()

#pragma endregion