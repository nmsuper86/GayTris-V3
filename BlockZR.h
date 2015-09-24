#pragma once

#ifndef __BLOCK_Z_R_H__
#define __BLOCK_Z_R_H__

#include "cocos2d.h"
#include "Block.h"

class BlockZR :public Block
{
public:
	BlockZR(int p_index);
	~BlockZR();
	static BlockZR* create(BlockManager* p_blockManagerToBind, int p_index = -1);
	virtual bool init(BlockManager* p_blockManagerToBind);

}; //class BlockZR: public Block

#endif //__BLOCK_Z_R_H__