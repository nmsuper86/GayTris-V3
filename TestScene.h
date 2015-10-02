#pragma once

#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "BlockManager.h"
#include "BlockManagerDoubleBlock.h"
#include "DisplayManager.h"
#include <windows.h>

using namespace cocos2d;

class TestScene :public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TestScene);

private:
	BlockManager* m_blockManager;
	DisplayManger* m_displayManager;
	EventListenerKeyboard* m_keyboardListener;

};

#endif //__TEST_SCENE_H__