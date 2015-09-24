#pragma once

#ifndef __BLOCK_L_H__
#define __BLOCK_L_H__

#include "cocos2d.h"
#include "Block.h"

using namespace cocos2d;

class BlockL :public Block
{
public:
	BlockL(int p_index);
	~BlockL();
	static BlockL* create(BlockManager* p_blockManagerToBind, int p_index = -1);
	virtual bool init(BlockManager* p_blockManagerToBind);

}; //class BlockL:public Block

#endif // !__BLOCK_L_H__




