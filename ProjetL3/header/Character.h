#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "RessourceLoader.h"

#include <iostream>
#include <stdio.h>

class Character
{
	public :
		Character(void);
		void LoadSprite(void);
		sf::Sprite* GetSprite(void);
		void SetSprite(sf::Sprite sprite);
		b2Body* getBody();
		void setBody(b2Body* body);

	protected :

	private :
		sf::Sprite image;
		b2Body* body;

};
#endif // CHARACTER_H
