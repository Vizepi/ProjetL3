#ifndef EVENT_H
#define EVENT_H
#include <Box2D/Box2D.h>
#include "Character.h"


class Event
{
	public :
		void event(sf::RenderWindow& window, Character& character, bool canJump);

	protected :

	private :


};
#endif // EVENT_H

