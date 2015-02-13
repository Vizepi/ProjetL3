/*

	Platformer Game - Made for the 3rd year of undergraduated project.
    Copyright (C) 2015  Corbat Lisa, Kieffer Joseph

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
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
