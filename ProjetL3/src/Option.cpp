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
#include "../header/Option.h"
#include "../header/Game.h"

/*explicit*/ Option::Option()
: m_fullScreen(false)
, m_music(true)
, m_sound(true)
, m_shadow(true)
{
	m_enabled.r = 0;
	m_enabled.g = 96;
	m_enabled.b = 0;
	m_disabled.r = 96;
	m_disabled.g = 0;
	m_disabled.b = 0;
	m_background.setTexture(*RessourceLoader::GetTexture("Menu Background"));
	m_background.setPosition(0, 0);
	m_backButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_backButton.setPosition((m_background.getTexture()->getSize().x - m_backButton.getTexture()->getSize().x)/2 - 200,
							(m_background.getTexture()->getSize().y - m_backButton.getTexture()->getSize().y)/2 + 200);
	m_applyButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_applyButton.setPosition((m_background.getTexture()->getSize().x - m_applyButton.getTexture()->getSize().x)/2 + 200,
							(m_background.getTexture()->getSize().y - m_applyButton.getTexture()->getSize().y)/2 + 200);
	m_fsButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_fsButton.setPosition((m_background.getTexture()->getSize().x - m_fsButton.getTexture()->getSize().x)/2 - 200,
							(m_background.getTexture()->getSize().y - m_fsButton.getTexture()->getSize().y)/2 - 60);
	m_musicButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_musicButton.setPosition((m_background.getTexture()->getSize().x - m_musicButton.getTexture()->getSize().x)/2 - 200,
							(m_background.getTexture()->getSize().y - m_musicButton.getTexture()->getSize().y)/2 + 60);
	m_soundsButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_soundsButton.setPosition((m_background.getTexture()->getSize().x - m_soundsButton.getTexture()->getSize().x)/2 + 200,
							(m_background.getTexture()->getSize().y - m_soundsButton.getTexture()->getSize().y)/2 - 60);
	m_shadowButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_shadowButton.setPosition((m_background.getTexture()->getSize().x - m_shadowButton.getTexture()->getSize().x)/2 + 200,
							(m_background.getTexture()->getSize().y - m_shadowButton.getTexture()->getSize().y)/2 + 60);
	m_title.setFont(*RessourceLoader::GetFont("Title"));
	m_title.setCharacterSize(72);
	m_title.setColor(sf::Color(255, 255, 255, 255));
	m_title.setString("OPTION");
	m_title.setPosition((m_background.getTexture()->getSize().x - m_title.getLocalBounds().width)/2,
							m_background.getTexture()->getSize().y/3 - m_title.getLocalBounds().height/2);
	m_backText.setFont(*RessourceLoader::GetFont("Default"));
	m_backText.setString("Back");
	m_backText.setColor(sf::Color::Black);
	m_backText.setPosition((m_background.getTexture()->getSize().x - m_backText.getLocalBounds().width)/2 - 200,
							(m_background.getTexture()->getSize().y - m_backText.getLocalBounds().height)/2 + 200 - 8);
	m_applyText.setFont(*RessourceLoader::GetFont("Default"));
	m_applyText.setString("Apply");
	m_applyText.setColor(sf::Color::Black);
	m_applyText.setPosition((m_background.getTexture()->getSize().x - m_applyText.getLocalBounds().width)/2 + 200,
							(m_background.getTexture()->getSize().y - m_applyText.getLocalBounds().height)/2 + 200 - 8);
	m_fsText.setFont(*RessourceLoader::GetFont("Default"));
	m_fsText.setString("FullScreen");
	m_fsText.setColor(m_disabled);
	m_fsText.setPosition((m_background.getTexture()->getSize().x - m_fsText.getLocalBounds().width)/2 - 200,
							(m_background.getTexture()->getSize().y - m_fsText.getLocalBounds().height)/2 - 60 - 8);
	m_musicText.setFont(*RessourceLoader::GetFont("Default"));
	m_musicText.setString("Music");
	m_musicText.setColor(m_enabled);
	m_musicText.setPosition((m_background.getTexture()->getSize().x - m_musicText.getLocalBounds().width)/2 - 200,
							(m_background.getTexture()->getSize().y - m_musicText.getLocalBounds().height)/2 + 60 - 8);
	m_soundsText.setFont(*RessourceLoader::GetFont("Default"));
	m_soundsText.setString("Sounds");
	m_soundsText.setColor(m_enabled);
	m_soundsText.setPosition((m_background.getTexture()->getSize().x - m_soundsText.getLocalBounds().width)/2 + 200,
							(m_background.getTexture()->getSize().y - m_soundsText.getLocalBounds().height)/2 - 60 - 8);
	m_shadowText.setFont(*RessourceLoader::GetFont("Default"));
	m_shadowText.setString("Shadow");
	m_shadowText.setColor(m_enabled);
	m_shadowText.setPosition((m_background.getTexture()->getSize().x - m_shadowText.getLocalBounds().width)/2 + 200,
							(m_background.getTexture()->getSize().y - m_shadowText.getLocalBounds().height)/2 + 60 - 8);
	m_set.fullscreen = m_fullScreen;
	m_set.music = m_music;
	m_set.sound = m_sound;
	m_set.shadow = m_shadow;
}

/*virtual*/ Option::~Option()
{

}

/*virtual*/ void Option::Update(sf::RenderWindow& window, sf::Time& frameTime)
{
	sf::Event event;
	while(window.pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed:
			Game::s_instance->SwitchState(STATE_QUIT);
			break;
		case sf::Event::KeyReleased:
			switch(event.key.code)
			{
			case sf::Keyboard::Escape:
				Discard();
				break;
			case sf::Keyboard::Return:
				Apply(window);
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonReleased:
			if(m_fsButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				m_set.fullscreen = !m_set.fullscreen;
			}
			else if(m_shadowButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				m_set.shadow = !m_set.shadow;
			}
			else if(m_musicButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				m_set.music = !m_set.music;
			}
			else if(m_soundsButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				m_set.sound = !m_set.sound;
			}
			else if(m_backButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				Discard();
			}
			else if(m_applyButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)) + window.getView().getCenter() - sf::Vector2f(window.getSize())/2.f))
			{
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Click")->play();
				Apply(window);
			}
			break;
		default:
			break;
		}
	}
	sf::View view(sf::Vector2f(m_background.getTexture()->getSize())/2.f, sf::Vector2f(window.getSize()));
	window.setView(view);
	if(m_set.fullscreen)
		m_fsText.setColor(m_enabled);
	else
		m_fsText.setColor(m_disabled);
	if(m_set.music)
		m_musicText.setColor(m_enabled);
	else
		m_musicText.setColor(m_disabled);
	if(m_set.sound)
		m_soundsText.setColor(m_enabled);
	else
		m_soundsText.setColor(m_disabled);
	if(m_set.shadow)
		m_shadowText.setColor(m_enabled);
	else
		m_shadowText.setColor(m_disabled);
}

/*virtual*/ void Option::Draw(sf::RenderWindow& window)
{
	window.draw(m_background);
	window.draw(m_backButton);
	window.draw(m_backText);
	window.draw(m_applyButton);
	window.draw(m_applyText);
	window.draw(m_fsButton);
	window.draw(m_fsText);
	window.draw(m_musicButton);
	window.draw(m_musicText);
	window.draw(m_soundsButton);
	window.draw(m_soundsText);
	window.draw(m_shadowButton);
	window.draw(m_shadowText);
	window.draw(m_title);
}

/*virtual*/ void Option::Apply(sf::RenderWindow& window)
{
	if(m_fullScreen != m_set.fullscreen)
	{
		if(m_set.fullscreen)
		{
			m_lastSize = window.getSize();
			window.create(sf::VideoMode::getFullscreenModes()[0], "Platformer", sf::Style::Fullscreen);
		}
		else
		{
			window.create(sf::VideoMode(m_lastSize.x, m_lastSize.y), "Platformer", sf::Style::Default);
		}
	}
	m_fullScreen = m_set.fullscreen;
	m_music = m_set.music;
	m_sound = m_set.sound;
	m_shadow = m_set.shadow;
	Game::s_instance->SetMusic(m_music);
	Game::s_instance->SetSounds(m_sound);
	Game::s_instance->SetShadow(m_shadow);
	if(Game::s_instance->WasInMenu())
		Game::s_instance->SwitchState(STATE_MENU);
	else
		Game::s_instance->SwitchState(STATE_PAUSE);
}

/*virtual*/ void Option::Discard()
{
	m_set.fullscreen = m_fullScreen;
	m_set.music = m_music;
	m_set.sound = m_sound;
	m_set.shadow = m_shadow;
	if(Game::s_instance->WasInMenu())
		Game::s_instance->SwitchState(STATE_MENU);
	else
		Game::s_instance->SwitchState(STATE_PAUSE);
}

