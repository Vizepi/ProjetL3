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
	virtual void Update(sf::RenderWindow& window, sf::Clock& frameTime);
	virtual void Draw(sf::RenderWindow& window);
protected:

private:
	sf::Sprite m_background;
	sf::Sprite m_playButton;
	sf::Sprite m_optionButton;
	sf::Sprite m_quitButton;
	sf::Sprite m_title;
	sf::Text m_playText;
	sf::Text m_optionText;
	sf::Text m_quitText;
};

#endif // MENU_H
