#pragma once

#ifndef __BLOCK_I_H__
#define __BLOCK_I_H__

#include "cocos2d.h"
#include "Block.h"

using namespace cocos2d;

class BlockI :public Block
{
public:
	BlockI(int p_index);
	~BlockI();
	static BlockI* create(BlockManager* p_blockManagerToBind, int p_index = -1);
	virtual bool init(BlockManager* p_blockManagerToBind);
}; //class BlockI

#endif //!__BLOCK_I_H__