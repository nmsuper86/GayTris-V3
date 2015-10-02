#pragma once

#ifndef __BLOCK_MANAGER_DOUBLE_BLOCK_H__
#define __BLOCK_MANAGER_DOUBLE_BLOCK_H__

#include "cocos2d.h"
#include "BlockManager.h"

class BlockManagerDoubleBlock : public BlockManager
{
public:
// 	static const int CellMatrixWidth = 16;
// 	static const int CellMatrixHeight = 20;
// 	static const int DefaultRefreshTime = 80;
// 	static const int RequiredUpdateTime = 2;

protected:
	int m_indexOfCurrentBlock;
	Block* m_blockDroppingD[2];
	Block* m_blockWaitingD[2];
	Block* m_blockWaitingToAdd;

	CellState m_matrixCellState[BlockManagerDoubleBlock::CellMatrixWidth][BlockManagerDoubleBlock::CellMatrixHeight];
	Sprite* m_matrixDeadBlockSprites[BlockManagerDoubleBlock::CellMatrixWidth][BlockManagerDoubleBlock::CellMatrixHeight];



public:
	BlockManagerDoubleBlock();
	~BlockManagerDoubleBlock();
	
	static BlockManagerDoubleBlock* create(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerTobind);
	virtual bool init(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerTobind);
	virtual void update(float delta);
	virtual void triggerKeyboard(EventKeyboard::KeyCode p_valueKey);

public:
	virtual Vec2 getBlockStartPosition0_0InMatrix(int p_indexOfBlock);

protected:
	virtual void _pushNewBlock(int p_index);
	virtual void _blockStopDrop(Block* p_block);


}; //class BlockManagerDoubleBlock : public BlockManager

#endif //!__BLOCK_MANAGER_DOUBLE_BLOCK_H__
