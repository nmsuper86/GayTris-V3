#pragma once

#ifndef __BLOCK_Z_H__
#define __BLOCK_Z_H__

#include "cocos2d.h"
#include "Block.h"

class BlockZ :public Block
{
public:
	BlockZ(int p_index);
	~BlockZ();
	static BlockZ* create(BlockManager* p_blockManagerToBind, int p_index = -1);
	virtual bool init(BlockManager* p_blockManagerToBind);

}; //class BlockZ: public Block

#endif //__BLOCK_Z_H__