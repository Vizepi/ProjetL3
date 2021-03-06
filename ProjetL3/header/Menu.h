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
#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "RessourceLoader.h"

class Menu
{
public:
	explicit Menu(void);
	virtual ~Menu(void);
	virtual void Update(sf::RenderWindow& window, sf::Time& frameTime);
	virtual void Draw(sf::RenderWindow& window);
protected:

private:
	sf::Sprite m_background;
	sf::Sprite m_playButton;
	sf::Sprite m_optionButton;
	sf::Sprite m_quitButton;
	sf::Text m_title;
	sf::Text m_playText;
	sf::Text m_optionText;
	sf::Text m_quitText;
};

#endif // MENU_H
