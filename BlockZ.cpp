#include "BlockZ.h"
#include "BlockManager.h"

BlockZ::BlockZ(int p_index)
{
	this->m_index = p_index;
} //BlockZ::BlockZ()

BlockZ::~BlockZ()
{

} //BlockZ::~BlockZ()

BlockZ* BlockZ::create(BlockManager* p_blockManagerToBind, int p_index)
{
	BlockZ* pRet = new BlockZ(p_index);
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


bool BlockZ::init(BlockManager* p_blockManagerToBind)
{
	this->m_manager = p_blockManagerToBind;
	this->m_position0_0 = this->m_manager->getBlockStartPosition0_0InMatrix(this->m_index);

	this->bindSprite(Sprite::create("BlockZ.png"));
	this->setBlockData(Vec2(0, 1), Vec2(this->m_position0_0.x + 0, this->m_position0_0.y + 1));
	this->setBlockData(Vec2(1, 1), Vec2(this->m_position0_0.x + 1, this->m_position0_0.y + 1));
	this->setBlockData(Vec2(1, 2), Vec2(this->m_position0_0.x + 1, this->m_position0_0.y + 2));
	this->setBlockData(Vec2(2, 2), Vec2(this->m_position0_0.x + 2, this->m_position0_0.y + 2));
	this->m_type = BlockType::Z;

	return true;
} //bool BlockZ::init()
