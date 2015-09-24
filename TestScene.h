#pragma once

#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "BlockManager.h"
#include "DisplayManager.h"
#include <windows.h>

using namespace cocos2d;

class TestScene :public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TestScene);

	void update(float delta);

private:
	BlockManager* m_blockManager;
	DisplayManger* m_displayManager;
	EventListenerKeyboard* m_keyboardListener;

	static TestScene* m_scene;

};

#endif //__TEST_SCENE_H__