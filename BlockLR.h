#pragma once

#ifndef __BLOCK_L_R_H__
#define __BLOCK_L_R_H__

#include "cocos2d.h"
#include "Block.h"

class BlockLR :public Block
{
public:
	BlockLR(int p_index);
	~BlockLR();
	static BlockLR* create(BlockManager* p_blockManagerToBind, int p_index = -1);
	virtual bool init(BlockManager* p_blockManagerToBind);

}; //class BlockLR: public Block

#endif //__BLOCK_L_R_H__