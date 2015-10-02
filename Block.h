#pragma once

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

class BlockManager;

class BlockI;

class Block :public Entity
{
public:
	typedef enum
	{
		Up,
		Down,
		Left,
		Right
	}Direction;
	typedef enum
	{
		I,
		L,
		L_Reverse,
		T,
		Z,
		Z_Reverse,
		O
	}BlockType;
	typedef struct
	{
		Vec2 points[4];
	}CellPosition;

	static const int CellSize = 25;
	static const int BlockWidthCount = 4;

protected:
	Vec2 m_blockData[Block::BlockWidthCount][Block::BlockWidthCount];
	Vec2 m_position0_0;
	BlockManager* m_manager;
	BlockType m_type;
	int m_timeCounter;
	int m_degree;
	int m_index;

public: //System functions
	Block();
	~Block();
	static Block* create(BlockManager* p_blockManagerToBind, int p_index = -1);
	virtual bool init(BlockManager* p_blockManagerToBind);

public: //Custom functions
	virtual CellPosition doTurn90Degrees(bool p_saveState); //旋转方块并返回旋转后的位置信息 saveState代表是否保存旋转后的结果
	virtual bool doMove(Direction p_direction);
	void bindManager(BlockManager* p_manager);
	bool increaseTimeCounter(int p_updateTime); //时间自增后返回是否需要更新位置
	static Block* generateNewBlock(BlockManager* p_blockManagerToBind, int p_index);
	CellPosition getCellPosition();
	void setBlockData(Vec2 p_blockPoint, Vec2 p_stagePoint); //子类通过此函数设定blockData矩阵中的坐标值
	BlockType getBlockType();
	int getDegree();
	Vec2 getCenterPointInMatrix();
	void setIndex(int p_index);
	int getIndex();

}; //class Block

#endif //__BLOCK_H__