#pragma once

#ifndef __BLOCK_MANAGER_H__
#define __BLOCK_MANAGER_H__

#include "cocos2d.h"
#include "Block.h"
#include "DisplayManager.h"

using namespace cocos2d;

class BlockManager :public Node
{
public:
	typedef enum
	{
		Empty,
		Dropping,
		Dead
	}CellState;
	static const int CellMatrixWidth = 10;
	static const int CellMatrixHeight = 20;
	static const int DefaultRefreshTime = 80;
	static const int RequiredUpdateTime = 2;

protected:
	int m_periodDropping;
	bool m_isDropReqiured;

	Block* m_blockDropping;
	Block* m_blockWaiting;

	CellState m_matrixCellState[BlockManager::CellMatrixWidth][BlockManager::CellMatrixHeight];
	Sprite* m_matrixDeadBlockSprites[BlockManager::CellMatrixWidth][BlockManager::CellMatrixHeight];

	DisplayManger* m_boundDisplayManager;
	SpriteBatchNode* m_spriteBatchNodeDeadBlocks;


public: //ϵͳ����

	BlockManager();
	~BlockManager();

	static BlockManager* create(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerTobind);
	virtual bool init(Vec2 p_pointToDisplayOnScreen, DisplayManger* p_displayManagerTobind);

	virtual void update(float p_delta); //��ʱˢ�º���

	virtual void triggerKeyboard(EventKeyboard::KeyCode p_valueKey);

public: //�����Զ��庯��
	int getUpdateTime(); //��ȡ����ʱ��
	void setUpdateTime(int p_updateTime);
	virtual Vec2 convertBlockPositionToPixelPosition(Vec2 p_pointBlock); //����������ת��Ϊʵ�����꣨���½ǣ�
	virtual inline Vec2 convertPositionToCenterPosition(Vec2 p_pointRightBottom)
	{
		Vec2 pointResult = Vec2(p_pointRightBottom.x + Block::CellSize / 2, p_pointRightBottom.y + Block::CellSize / 2);
		return pointResult;
	}
	void setDownKeyState(bool p_isKeyPressed); //�趨��������״̬ 
	virtual Vec2 getBlockStartPosition0_0InMatrix(int p_indexOfBlock);

protected: //˽���Զ��庯��
	virtual Block* _createNewBlock(); //�����µķ���
	virtual void _pushNewBlock(); //�����µ�nextBlock �޸�Stage���� ����ԭnextBlock������̨

	virtual bool _blockShouldTryPeriodicalDrop(Block* p_block, int p_updateTime); //�жϷ����Ƿ�Ӧ������һ��
	virtual void _blockDonotTryDrop(Block* p_block); //����������
	virtual void _blockDoTryMove(Block* p_block, Block::Direction p_direction); //�����ƶ�
	virtual void _blockDoTryRequiredDrop(Block* p_block); //���Դ�������
	virtual void _blockDoTryTurn90Degrees(Block* p_block); //������ת90��

	virtual bool _blockCanMove(Block* p_block, Block::Direction p_direction); //�жϷ����Ƿ�������ض������ƶ�
	virtual void _blockDoMove(Block* p_block, Block::Direction p_direction); //ʹ��ǰ�����ƶ�
	virtual void _blockStopDrop(Block* p_block); //ֹͣ��ǰ��������
	virtual bool _blockCanTurn90Degrees(Block* p_block); //�жϷ����Ƿ������ת90��
	virtual void _blockDoTurn90Degrees(Block* p_block); //ʹ��ǰ������ת

	virtual void _updateCellMatrixForBlockBeforeMove(Block* p_block, Block::Direction p_direction); //ΪBlock�ƶ����µ�Ԫ����
	virtual void _updateCellMatrixForBlockBeforeDie(Block* p_block); //ΪBlock�������µ�Ԫ����
	virtual void _updateCellMatrixAfterChanged(Block::CellPosition originalPosition, Block::CellPosition newPosition); //��position���¾���
	virtual int _eliminateLines(); //��������
	bool _blockOverlayed(Block::CellPosition p_block1, Block::CellPosition p_block2);
	virtual void _endGame(); //������Ϸ


	virtual void _eliminateSingleLine(int lineNum); //�������У�����Ч��,������ݵȣ�
	virtual inline bool _isLineFilled(int lineNum)
	{
		for (int i = 0; i < BlockManager::CellMatrixWidth; i++)
		{
			if (this->m_matrixCellState[i][lineNum] != BlockManager::CellState::Dead)
			{
				return false;
			}
		}
		return true;
	} //�жϸ����Ƿ�����
	virtual void _isTetris(); //Tetris����
	virtual void _rePaintDeadBlocks(); //�ػ�dead blocks


}; //class BlockManager

#endif //!__BLOCK_MANAGER_H__