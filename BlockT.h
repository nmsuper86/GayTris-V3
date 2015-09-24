#pragma once

#ifndef __BLOCK_T_H__
#define __BLOCK_T_H__

#include "cocos2d.h"
#include "Block.h"

class BlockT :public Block
{
public:
	BlockT(int p_index);
	~BlockT();
	static BlockT* create(BlockManager* p_blockManagerToBind, int p_index = -1);
	virtual bool init(BlockManager* p_blockManagerToBind);

}; //class BlockT: public Block

#endif //__BLOCK_T_H__