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
#include "../header/Lose.h"
#include "../header/Game.h"

/*explicit*/ Lose::Lose()
{
	m_background.setTexture(*RessourceLoader::GetTexture("Menu Background"));
	m_background.setPosition(0, 0);
	m_replayButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_replayButton.setPosition((m_background.getTexture()->getSize().x - m_replayButton.getTexture()->getSize().x)/2,
							(m_background.getTexture()->getSize().y - m_replayButton.getTexture()->getSize().y)/2);
	m_saveButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_saveButton.setPosition((m_background.getTexture()->getSize().x - m_saveButton.getTexture()->getSize().x)/2,
							(m_background.getTexture()->getSize().y - m_saveButton.getTexture()->getSize().y)/2 + 80);
	m_quitButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_quitButton.setPosition((m_background.getTexture()->getSize().x - m_quitButton.getTexture()->getSize().x)/2,
							(m_background.getTexture()->getSize().y - m_quitButton.getTexture()->getSize().y)/2 + 160);
	m_title.setFont(*RessourceLoader::GetFont("Title"));
	m_title.setCharacterSize(72);
	m_title.setColor(sf::Color(255, 255, 255, 255));
	m_title.setString("GAME OVER");
	m_title.setPosition((m_background.getTexture()->getSize().x - m_title.getLocalBounds().width)/2,
							m_background.getTexture()->getSize().y/3 - m_title.getLocalBounds().height/2);
	m_replayText.setFont(*RessourceLoader::GetFont("Default"));
	m_replayText.setString("Replay");
	m_replayText.setColor(sf::Color::Black);
	m_replayText.setPosition((m_background.getTexture()->getSize().x - m_replayText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_replayText.getLocalBounds().height)/2 - 8);
	m_saveText.setFont(*RessourceLoader::GetFont("Default"));
	m_saveText.setString("Save level");
	m_saveText.setColor(sf::Color::Black);
	m_saveText.setPosition((m_background.getTexture()->getSize().x - m_saveText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_saveText.getLocalBounds().height)/2 - 8 + 80);
	m_quitText.setFont(*RessourceLoader::GetFont("Default"));
	m_quitText.setString("Menu");
	m_quitText.setColor(sf::Color::Black);
	m_quitText.setPosition((m_background.getTexture()->getSize().x - m_quitText.getLocalBounds().width)/2,
							(m_background.getTexture()->getSize().y - m_quitText.getLocalBounds().height)/2 - 8 + 160);
}

/*virtual*/ Lose::~Lose()
{

}

/*virtual*/ void Lose::Update(sf::RenderWindow& window, sf::Time& frameTime)
{
	// Positionnement

	int winX = window.getView().getCenter().x - window.getSize().x / 2;
	int winY = window.getView().getCenter().y - window.getSize().y / 2;
	m_background.setPosition(0, 0);
	m_replayButton.setPosition(winX + (window.getSize().x - m_replayButton.getTexture()->getSize().x)/2,
							winY + (window.getSize().y - m_replayButton.getTexture()->getSize().y)/2);
	m_saveButton.setPosition(winX + (window.getSize().x - m_saveButton.getTexture()->getSize().x)/2,
							winY + (window.getSize().y - m_saveButton.getTexture()->getSize().y)/2 + 80);
	m_quitButton.setPosition(winX + (window.getSize().x - m_quitButton.getTexture()->getSize().x)/2,
							winY + (window.getSize().y - m_quitButton.getTexture()->getSize().y)/2 + 160);
	m_title.setPosition(winX + (window.getSize().x - m_title.getLocalBounds().width)/2,
							winY + window.getSize().y/3 - m_title.getLocalBounds().height/2);
	m_replayText.setPosition(winX + (window.getSize().x - m_replayText.getLocalBounds().width)/2,
							winY + (window.getSize().y - m_replayText.getLocalBounds().height)/2 - 8);
	m_saveText.setPosition(winX + (window.getSize().x - m_saveText.getLocalBounds().width)/2,
							winY + (window.getSize().y - m_saveText.getLocalBounds().height)/2 - 8 + 80);
	m_quitText.setPosition(winX + (window.getSize().x - m_quitText.getLocalBounds().width)/2,
							winY + (window.getSize().y - m_quitText.getLocalBounds().height)/2 - 8 + 160);

	// Evenements
	sf::Event event;
	while(window.pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed:
			Game::s_instance->SwitchState(STATE_QUIT);
			break;
		case sf::Event::MouseButtonReleased:
			if(m_replayButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				if(Game::s_instance->IsMusicActive())
					RessourceLoader::GetMusic("Level")->play();
				Game::s_instance->SwitchState(STATE_INGAME);
				Game::s_instance->NewLevelReplay();
			}
			else if(m_saveButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				//Game::s_instance->SwitchState(STATE_OPTION/*A CHANGER*/);
			}
			else if(m_quitButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				Game::s_instance->SwitchState(STATE_MENU);
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

	sf::View view(window.getView().getCenter(), sf::Vector2f(window.getSize()));
	window.setView(view);

}

/*virtual*/ void Lose::Draw(sf::RenderWindow& window)
{
	Game::s_instance->GetLevel()->Draw(window);
	window.draw(m_title);
	window.draw(m_replayButton);
	window.draw(m_saveButton);
	window.draw(m_quitButton);
	window.draw(m_replayText);
	window.draw(m_saveText);
	window.draw(m_quitText);
}



