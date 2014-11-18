#include "../header/Level.h"


Level::Level()
{

}
void Level::LoadLevel()
{
	character.LoadSprite();
}

void Level::Draw(sf::RenderWindow& window)
{
	window.draw(character.GetSprite());
}
