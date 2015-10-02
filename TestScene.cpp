#include "TestScene.h"

CCScene* TestScene::createScene()
{
	CCScene* scene = CCScene::create();

	TestScene* layer = TestScene::create();

	scene->addChild(layer);

	return scene;
}

bool TestScene::init()
{
// 	this->m_displayManager = DisplayManger::create();
// 
// 	this->m_displayManager->setPosition(650, 270);
// 
// 	this->addChild(this->m_displayManager);
// 
// 	this->m_blockManager = BlockManager::create(Vec2(400, 300), this->m_displayManager);

	this->m_blockManager = BlockManagerDoubleBlock::create(Vec2(400, 300), NULL);

	this->addChild(m_blockManager);

	m_keyboardListener = EventListenerKeyboard::create();
	m_keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode p_keyCode, Event* p_event) {this->m_blockManager->triggerKeyboard(p_keyCode);};
	m_keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode p_keyCode, Event* p_event) {this->m_blockManager->setDownKeyState(false);}; 
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyboardListener, this);

	return true;
}

