#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Menu.h"
#include "Option.h"
#include "Win.h"
#include "Lose.h"
#include "Pause.h"
#include <SFML/Graphics.hpp>

enum
{
	STATE_MENU,
	STATE_INGAME,
	STATE_PAUSE,
	STATE_WIN,
	STATE_LOSE,
	STATE_OPTION,
	STATE_QUIT
};

class Game
{
public:
	explicit Game(void);
	virtual ~Game(void);
	virtual void Update(sf::RenderWindow& window, sf::Time& frameTime);
	virtual void Draw(sf::RenderWindow& window);
	virtual void SwitchState(int state);
	virtual void NewLevel(void);
	virtual void NewLevelReplay(void);
	virtual void SetMusic(bool state);
	virtual void SetSounds(bool state);
	virtual void SetShadow(bool state);
	virtual void SetFrom(bool menu);
	virtual bool IsMusicActive(void) const;
	virtual bool IsSoundsActive(void) const;
	virtual bool IsShadowActive(void) const;
	virtual bool WasInMenu(void) const;
	static Game* s_instance;
	Level* GetLevel(void);
protected:
	Level* m_level;
	Pause m_pause;
	Menu m_menu;
	Option m_option;
	Win m_win;
	Lose m_lose;
	int m_state;
	bool m_musicActive;
	bool m_soundsActive;
	bool m_shadowActive;
	bool m_fromMenu;
private:

};

#endif // GAME_H
