#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"

using namespace cocos2d;

class Entity :public Node
{
public:
	Entity();
	~Entity();
	Sprite* getSprite();
	void bindSprite(Sprite* sprite);

private:
	Sprite* m_sprite;
}; //class Entity

#endif //__ENTITY_H__