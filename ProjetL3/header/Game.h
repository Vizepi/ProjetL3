#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Menu.h"
#include "Option.h"
#include <SFML/Graphics.hpp>

enum
{
	STATE_MENU,
	STATE_INGAME,
	STATE_PAUSE,
	STATE_OPTION,
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
	virtual void NewLevel(void);
	virtual void SetMusic(bool state);
	virtual void SetSounds(bool state);
	virtual void SetShadow(bool state);
	virtual bool IsMusicActive(void) const;
	virtual bool IsSoundsActive(void) const;
	virtual bool IsShadowActive(void) const;
	static Game* s_instance;
protected:
	Level* m_level;
	Menu m_menu;
	Option m_option;
	int m_state;
	bool m_musicActive;
	bool m_soundsActive;
	bool m_shadowActive;
private:

};

#endif // GAME_H
