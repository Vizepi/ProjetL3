#ifndef LEVEL_H
#define LEVEL_H
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Character.h"
#include "../header/Event.h"

class Level
{
	public :
		Level(void);
		void CreateStaticObject(b2World& world, float x, float y, float width, float height);
		b2Body* CreateDynamicObject(b2World& world, float x, float y, float width, float height);
		void LoadLevel(void);
		void Draw(sf::RenderWindow& window);
		void Blah(sf::RenderWindow& window, sf::Clock& frameTime);
		Character* getCharacter(void);

	protected:

	private :
		Event  event;
		b2Vec2 gravity;
		b2World world;
		Character character;


};
#endif // LEVEL_H
