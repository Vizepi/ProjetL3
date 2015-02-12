#ifndef PAUSE_H
#define PAUSE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "RessourceLoader.h"

class Pause
{
public:
	explicit Pause(void);
	virtual ~Pause(void);
	virtual void Update(sf::RenderWindow& window, sf::Time& frameTime);
	virtual void Draw(sf::RenderWindow& window);
protected:

private:
	sf::Sprite m_background;
	sf::Sprite m_replayButton;
	sf::Sprite m_optionButton;
	sf::Sprite m_quitButton;
	sf::Text m_title;
	sf::Text m_replayText;
	sf::Text m_optionText;
	sf::Text m_quitText;
};

#endif // PAUSE_H


