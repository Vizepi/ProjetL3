
#ifndef LOSE_H
#define LOSE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "RessourceLoader.h"

class Lose
{
public:
	explicit Lose(void);
	virtual ~Lose(void);
	virtual void Update(sf::RenderWindow& window, sf::Time& frameTime);
	virtual void Draw(sf::RenderWindow& window);
protected:

private:
	sf::Sprite m_background;
	sf::Sprite m_replayButton;
	sf::Sprite m_saveButton;
	sf::Sprite m_quitButton;
	sf::Text m_title;
	sf::Text m_replayText;
	sf::Text m_saveText;
	sf::Text m_quitText;
};

#endif // LOSE_H

