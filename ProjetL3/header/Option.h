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
#ifndef OPTION_HPP
#define OPTION_HPP

#include <SFML/Graphics.hpp>
#include "RessourceLoader.h"
#include <vector>

struct OptionSet
{
	bool fullscreen;
	bool music;
	bool sound;
	bool shadow;
};

class Option
{
public:
	explicit Option(void);
	virtual ~Option(void);
	virtual void Update(sf::RenderWindow& window, sf::Time& frameTime);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Apply(sf::RenderWindow& window);
	virtual void Discard(void);
protected:
	sf::Sprite m_background;
	sf::Sprite m_backButton;
	sf::Sprite m_applyButton;
	sf::Sprite m_fsButton;
	sf::Sprite m_musicButton;
	sf::Sprite m_soundsButton;
	sf::Sprite m_shadowButton;
	sf::Text m_title;
	sf::Text m_backText;
	sf::Text m_applyText;
	sf::Text m_fsText;
	sf::Text m_musicText;
	sf::Text m_soundsText;
	sf::Text m_shadowText;
	sf::Color m_enabled;
	sf::Color m_disabled;
	bool m_fullScreen;
	bool m_music;
	bool m_sound;
	bool m_shadow;
	struct OptionSet m_set;
	sf::Vector2u m_lastSize;
private:

};

#endif // OPTION_HPP
