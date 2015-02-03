#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>

enum
{
	STATE_MENU,
	STATE_INGAME,
	STATE_PAUSE,
	STATE_QUIT
};

class Game
{
public:
	explicit Game(void);
	virtual ~Game(void);
	virtual void Update(sf::RenderWindow& window, sf::Clock& frameTime);
	virtual void Draw(sf::RenderWindow& window);
	virtual void SwitchState(int state);
	static Game* s_instance;
protected:
	Level* m_level;
	Menu m_menu;
	int m_state;
private:

};

#endif // GAME_H
