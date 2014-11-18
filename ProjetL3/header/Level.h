#ifndef LEVEL_H
#define LEVEL_H
#include <SFML/Graphics.hpp>
#include "Character.h"

class Level
{
	public :
		Level(void);
		void LoadLevel(void);
		void Draw(sf::RenderWindow& window);

	protected:

	private :
		Character character;


};
#endif // LEVEL_H
