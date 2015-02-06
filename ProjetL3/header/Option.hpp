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
	int width;
	int height;
};

class Option
{
public:
	explicit Option(void);
	virtual ~Option(void);
	virtual void Update(sf::RenderWindow& window, sf::Clock& frameTime);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Apply(void);
	virtual void Discard(void);
protected:
	sf::Sprite m_background;
	sf::Sprite m_backButton;
	sf::Sprite m_applyButton;
	sf::Sprite m_fsButton;
	sf::Sprite m_musicButton;
	sf::Sprite m_soundsButton;
	sf::Text m_backText;
	sf::Text m_applyText;
	sf::Text m_fsText;
	sf::Text m_musicText;
	sf::Text m_soundText;
	sf::Text m_resolutionText;
	std::vector<std::string> m_resolutionTexts;
	std::vector<sf::Vector2u> m_resolutions;
	int m_resolutionIndex;
	bool m_fullScreen;
	bool m_music;
	bool m_sound;
	struct OptionSet m_set;
private:

};

#endif // OPTION_HPP
