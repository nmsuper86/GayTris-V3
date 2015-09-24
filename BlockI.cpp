#include "BlockI.h"
#include "BlockManager.h"

BlockI::BlockI(int p_index)
{
	this->m_index = p_index;
} //BlockI::BlockI()

BlockI::~BlockI()
{

} //BlockI::~BlockI()

BlockI* BlockI::create(BlockManager* p_blockManagerToBind, int p_index)
{
	BlockI* pRet = new BlockI(p_index);
	if (pRet && pRet->init(p_blockManagerToBind))
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
}

bool BlockI::init(BlockManager* p_blockManagerToBind)
{
	this->m_manager = p_blockManagerToBind;
	this->m_position0_0 = this->m_manager->getBlockStartPosition0_0InMatrix(this->m_index);

	this->bindSprite(Sprite::create("BlockI.png"));
	for (int i = 0; i < 4; i++)
	{
		this->setBlockData(Vec2(i, 1), Vec2(this->m_position0_0.x + i, this->m_position0_0.y + 1));
	}
	this->m_type = BlockType::I;
	return true;
} //bool BlockI::init()