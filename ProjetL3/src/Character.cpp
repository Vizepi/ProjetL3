#include "../header/Character.h"

Character::Character()
{
	LoadSprite();
}

void Character::LoadSprite()
{
	image.setTexture(*RessourceLoader::GetTexture("Matt Smith"));
	image.setTextureRect(sf::IntRect(0,0,32,47));
}

sf::Sprite* Character::GetSprite()
{
	return &image;
}

void Character::SetSprite(sf::Sprite sprite)
{
	image = sprite;
}

b2Body* Character::getBody()
{
	return body;
}

void Character::setBody(b2Body* body)
{
	this->body = body;
}
