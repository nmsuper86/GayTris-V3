#include "BlockZR.h"
#include "BlockManager.h"

BlockZR::BlockZR(int p_index)
{
	this->m_index = p_index;
} //BlockZR::BlockZR()

BlockZR::~BlockZR()
{

} //BlockZR::~BlockZR()

BlockZR* BlockZR::create(BlockManager* p_blockManagerToBind, int p_index)
{
	BlockZR* pRet = new BlockZR(p_index);
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


bool BlockZR::init(BlockManager* p_blockManagerToBind)
{
	this->m_manager = p_blockManagerToBind;
	this->m_position0_0 = this->m_manager->getBlockStartPosition0_0InMatrix(this->m_index);

	this->bindSprite(Sprite::create("BlockZR.png"));
	this->setBlockData(Vec2(2, 1), Vec2(this->m_position0_0.x + 2, this->m_position0_0.y + 1));
	this->setBlockData(Vec2(3, 1), Vec2(this->m_position0_0.x + 3, this->m_position0_0.y + 1));
	this->setBlockData(Vec2(1, 2), Vec2(this->m_position0_0.x + 1, this->m_position0_0.y + 2));
	this->setBlockData(Vec2(2, 2), Vec2(this->m_position0_0.x + 2, this->m_position0_0.y + 2));
	this->m_type = BlockType::Z_Reverse;

	
	return true;
} //bool BlockZR::init()