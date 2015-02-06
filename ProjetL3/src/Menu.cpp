#include "../header/Menu.h"
#include "../header/Game.h"

/*explicit*/ Menu::Menu()
{
	m_background.setTexture(*RessourceLoader::GetTexture("Menu Background"));
	m_background.setPosition(0, 0);
	m_playButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_playButton.setPosition((m_background.getTexture()->getSize().x - m_playButton.getTexture()->getSize().x)/2,
							(m_background.getTexture()->getSize().y - m_playButton.getTexture()->getSize().y)/2);
	m_optionButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_optionButton.setPosition((m_background.getTexture()->getSize().x - m_optionButton.getTexture()->getSize().x)/2,
							(m_background.getTexture()->getSize().y - m_optionButton.getTexture()->getSize().y)/2 + 80);
	m_quitButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_quitButton.setPosition((m_background.getTexture()->getSize().x - m_quitButton.getTexture()->getSize().x)/2,
							(m_background.getTexture()->getSize().y - m_quitButton.getTexture()->getSize().y)/2 + 160);
	m_title.setTexture(*RessourceLoader::GetTexture("Menu Title"));
	m_title.setPosition((m_background.getTexture()->getSize().x - m_title.getTexture()->getSize().x)/2,
							m_background.getTexture()->getSize().y/3 - m_title.getTexture()->getSize().y/2);
	m_playText.setFont(*RessourceLoader::GetFont());
	m_playText.setString("Play");
	m_playText.setColor(sf::Color::Black);
	m_playText.setPosition((m_background.getTexture()->getSize().x - m_playText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_playText.getLocalBounds().height)/2 - 8);
	m_optionText.setFont(*RessourceLoader::GetFont());
	m_optionText.setString("Options");
	m_optionText.setColor(sf::Color::Black);
	m_optionText.setPosition((m_background.getTexture()->getSize().x - m_optionText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_optionText.getLocalBounds().height)/2 - 8 + 80);
	m_quitText.setFont(*RessourceLoader::GetFont());
	m_quitText.setString("Exit");
	m_quitText.setColor(sf::Color::Black);
	m_quitText.setPosition((m_background.getTexture()->getSize().x - m_quitText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_quitText.getLocalBounds().height)/2 - 8 + 160);
}

/*virtual*/ Menu::~Menu()
{

}

/*virtual*/ void Menu::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{
	sf::Event event;
	while(window.pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed:
			Game::s_instance->SwitchState(STATE_QUIT);
			break;
		case sf::Event::MouseButtonReleased:
			if(m_playButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				Game::s_instance->SwitchState(STATE_INGAME);
				Game::s_instance->NewLevel();
			}
			else if(m_optionButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				std::cout << "option clic" << std::endl;
			}
			else if(m_quitButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				Game::s_instance->SwitchState(STATE_QUIT);
			}
			break;
		case sf::Event::Resized:
			// Gere la taille du viewport lors du redimensionnement !
			if(event.size.width > 1920)
				window.setSize(sf::Vector2u(1920, window.getSize().y));
			if(event.size.width < 800)
				window.setSize(sf::Vector2u(800, window.getSize().y));
			if(event.size.height > 1080)
				window.setSize(sf::Vector2u(window.getSize().x, 1080));
			if(event.size.height < 600)
				window.setSize(sf::Vector2u(window.getSize().x, 600));
			break;
		default:
			break;
		}
	}
	sf::View view(sf::Vector2f(m_background.getTexture()->getSize())/2.f, sf::Vector2f(window.getSize()));
	window.setView(view);
}

/*virtual*/ void Menu::Draw(sf::RenderWindow& window)
{
	window.draw(m_background);
	window.draw(m_title);
	window.draw(m_playButton);
	window.draw(m_optionButton);
	window.draw(m_quitButton);
	window.draw(m_playText);
	window.draw(m_optionText);
	window.draw(m_quitText);
}

