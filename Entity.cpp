#include "Entity.h"

Entity::Entity()
{
	m_sprite = NULL;
} //Entity::Entity()

Entity::~Entity()
{

} //Entity::~Entity()

Sprite* Entity::getSprite()
{
	return this->m_sprite;
} //Sprite* Entity::getSprite()

void Entity::bindSprite(Sprite* sprite)
{
	this->m_sprite = sprite;
	this->addChild(sprite);
} //void Entiry::bindSprite()

