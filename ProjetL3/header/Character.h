#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include "RessourceLoader.h"

#include <iostream>
#include <stdio.h>

class Character
{
	public :
		Character(void);
		void LoadSprite(void);
		sf::Sprite GetSprite(void);
		void SetSprite(sf::Sprite sprite);

	protected :

	private :
		sf::Sprite character;

};
#endif // CHARACTER_H
