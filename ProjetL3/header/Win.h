#ifndef WIN_H
#define WIN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "RessourceLoader.h"

class Win
{
public:
	explicit Win(void);
	virtual ~Win(void);
	virtual void Update(sf::RenderWindow& window, sf::Clock& frameTime);
	virtual void Draw(sf::RenderWindow& window);
protected:

private:
	sf::Sprite m_background;
	sf::Sprite m_replayButton;
	sf::Sprite m_saveButton;
	sf::Sprite m_quitButton;
	sf::Sprite m_title;
	sf::Text m_replayText;
	sf::Text m_saveText;
	sf::Text m_quitText;
};

#endif // WIN_H

