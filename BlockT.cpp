#include "BlockT.h"
#include "BlockManager.h"

BlockT::BlockT(int p_index)
{
	this->m_index = p_index;
} //BlockT::BlockT()

BlockT::~BlockT()
{

} //BlockT::~BlockT()

BlockT* BlockT::create(BlockManager* p_blockManagerToBind, int p_index)
{
	BlockT* pRet = new BlockT(p_index);
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

bool BlockT::init(BlockManager* p_blockManagerToBind)
{
	this->m_manager = p_blockManagerToBind;
	this->m_position0_0 = this->m_manager->getBlockStartPosition0_0InMatrix(this->m_index);

	this->bindSprite(Sprite::create("BlockT.png"));
	this->setBlockData(Vec2(2, 0), Vec2(this->m_position0_0.x + 2, this->m_position0_0.y + 0));
	this->setBlockData(Vec2(1, 1), Vec2(this->m_position0_0.x + 1, this->m_position0_0.y + 1));
	this->setBlockData(Vec2(2, 1), Vec2(this->m_position0_0.x + 2, this->m_position0_0.y + 1));
	this->setBlockData(Vec2(2, 2), Vec2(this->m_position0_0.x + 2, this->m_position0_0.y + 2));
	this->m_type = BlockType::T;

	return true;
} //bool BlockT::init()