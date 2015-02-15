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
	m_title.setFont(*RessourceLoader::GetFont("Title"));
	m_title.setCharacterSize(72);
	m_title.setColor(sf::Color(200, 40, 0, 255));
	m_title.setString("PLATFORMER");
	m_title.setPosition((m_background.getTexture()->getSize().x - m_title.getLocalBounds().width)/2,
							m_background.getTexture()->getSize().y/3 - m_title.getLocalBounds().height/2);
	m_playText.setFont(*RessourceLoader::GetFont("Default"));
	m_playText.setString("Play");
	m_playText.setColor(sf::Color::Black);
	m_playText.setPosition((m_background.getTexture()->getSize().x - m_playText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_playText.getLocalBounds().height)/2 - 8);
	m_optionText.setFont(*RessourceLoader::GetFont("Default"));
	m_optionText.setString("Options");
	m_optionText.setColor(sf::Color::Black);
	m_optionText.setPosition((m_background.getTexture()->getSize().x - m_optionText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_optionText.getLocalBounds().height)/2 - 8 + 80);
	m_quitText.setFont(*RessourceLoader::GetFont("Default"));
	m_quitText.setString("Exit");
	m_quitText.setColor(sf::Color::Black);
	m_quitText.setPosition((m_background.getTexture()->getSize().x - m_quitText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_quitText.getLocalBounds().height)/2 - 8 + 160);
}

/*virtual*/ Menu::~Menu()
{

}

/*virtual*/ void Menu::Update(sf::RenderWindow& window, sf::Time& frameTime)
{
	Game::s_instance->SetFrom(true);
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
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				Game::s_instance->SwitchState(STATE_INGAME);
				Game::s_instance->NewLevel();
				if(Game::s_instance->IsMusicActive())
					RessourceLoader::GetMusic("Level")->play();
			}
			else if(m_optionButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				Game::s_instance->SwitchState(STATE_OPTION);
			}
			else if(m_quitButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
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

