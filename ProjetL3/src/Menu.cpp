#include <header/Menu.h>

/*explicit*/ Menu::Menu()
{
	m_background.setTexture(*RessourceLoader::GetTexture("Menu Background"));
	m_background.setPosition(0, 0);
	m_playButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_optionButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_quitButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
}

/*virtual*/ Menu::~Menu()
{

}

/*virtual*/ void Menu::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{
	int winW = window.getSize().x;
	int winH = window.getSize().y;
	int
}

/*virtual*/ void Menu::Draw(sf::RenderWindow& window)
{

}

