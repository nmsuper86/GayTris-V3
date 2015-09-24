#pragma once

#ifndef __BLOCK_MANAGER_H__
#define __BLOCK_MANAGER_H__

#include "cocos2d.h"
#include "Block.h"
#include "DisplayManager.h"

using namespace cocos2d;

class BlockManager :public Node
{
public:
	typedef enum
	{
		Empty,
		Dropping,
		Dead
	}CellState;
	static const int CellMatrixWidth = 10;
	static const int CellMatrixHeight = 20;
	static const int DefaultRefreshTime = 80;
	static const int RequiredUpdateTime = 2;

protected:
	int m_periodDropping;
	bool m_isDropReqiured;

	Block* m_blockDropping;
	Block* m_blockWaiting;

	CellState m_matrixCellState[BlockManager::CellMatrixWidth][BlockManager::CellMatrixHeight];
	Sprite* m_matrixDeadBlockSprites[BlockManager::CellMatrixWidth][BlockManager::CellMatrixHeight];

	DisplayManger* m_boundDisplayManager;
	SpriteBatchNode* m_spriteBatchNodeDeadBlocks;


public: //系统调用

	BlockManager();
	~BlockManager();

	static BlockManager* create(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerTobind);
	virtual bool init(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerTobind);

	virtual void update(float p_delta); //定时刷新函数

	virtual void triggerKeyboard(EventKeyboard::KeyCode p_valueKey);

public: //公有自定义函数
	int getUpdateTime(); //获取更新时间
	void setUpdateTime(int p_updateTime);
	virtual Vec2 convertBlockPositionToPixelPosition(Vec2 p_pointBlock); //把网格坐标转化为实际坐标（右下角）
	virtual inline Vec2 convertPositionToCenterPosition(Vec2 p_pointRightBottom)
	{
		Vec2 pointResult = Vec2(p_pointRightBottom.x + Block::CellSize / 2, p_pointRightBottom.y + Block::CellSize / 2);
		return pointResult;
	}
	void setDownKeyState(bool p_isKeyPressed); //设定按键按下状态 
	virtual Vec2 getBlockStartPosition0_0InMatrix(int p_indexOfBlock);

protected: //私有自定义函数
	virtual Block* _createNewBlock(); //生成新的方块
	virtual void _pushNewBlock(); //生成新的nextBlock 修改Stage矩阵 并把原nextBlock推上舞台

	virtual bool _blockShouldTryPeriodicalDrop(Block* p_block, int p_updateTime); //判断方块是否应该下落一次
	virtual void _blockDonotTryDrop(Block* p_block); //不尝试下落
	virtual void _blockDoTryMove(Block* p_block, Block::Direction p_direction); //尝试移动
	virtual void _blockDoTryRequiredDrop(Block* p_block); //尝试触发下落
	virtual void _blockDoTryTurn90Degrees(Block* p_block); //尝试旋转90度

	virtual bool _blockCanMove(Block* p_block, Block::Direction p_direction); //判断方块是否可以向特定方向移动
	virtual void _blockDoMove(Block* p_block, Block::Direction p_direction); //使当前方块移动
	virtual void _blockStopDrop(Block* p_block); //停止当前方块下落
	virtual bool _blockCanTurn90Degrees(Block* p_block); //判断方块是否可以旋转90度
	virtual void _blockDoTurn90Degrees(Block* p_block); //使当前方块旋转

	virtual void _updateCellMatrixForBlockBeforeMove(Block* p_block, Block::Direction p_direction); //为Block移动更新单元矩阵
	virtual void _updateCellMatrixForBlockBeforeDie(Block* p_block); //为Block死亡更新单元矩阵
	virtual void _updateCellMatrixAfterChanged(Block::CellPosition originalPosition, Block::CellPosition newPosition); //用position更新矩阵
	virtual int _eliminateLines(); //尝试消除
	bool _blockOverlayed(Block::CellPosition p_block1, Block::CellPosition p_block2);
	virtual void _endGame(); //结束游戏


	virtual void _eliminateSingleLine(int lineNum); //消除单行（动画效果,清除数据等）
	virtual inline bool _isLineFilled(int lineNum)
	{
		for (int i = 0; i < BlockManager::CellMatrixWidth; i++)
		{
			if (this->m_matrixCellState[i][lineNum] != BlockManager::CellState::Dead)
			{
				return false;
			}
		}
		return true;
	} //判断该行是否已满
	virtual void _isTetris(); //Tetris动画
	virtual void _rePaintDeadBlocks(); //重绘dead blocks


}; //class BlockManager

#endif //!__BLOCK_MANAGER_H__