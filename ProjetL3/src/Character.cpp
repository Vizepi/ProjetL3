#include "../header/Character.h"

Character::Character()
{
	LoadSprite();
}

void Character::LoadSprite()
{
	character.setTexture(*RessourceLoader::GetTexture("Matt Smith"));
	character.setTextureRect(sf::IntRect(0,0,32,47));
	character.setPosition(0, 700);

}

sf::Sprite Character::GetSprite()
{
	return character;
}

void Character::SetSprite(sf::Sprite sprite)
{
	character = sprite;
}
