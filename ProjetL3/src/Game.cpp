#include <header/Game.h>

/*static*/ Game* Game::s_instance = NULL;

/*explicit*/ Game::Game()
: m_level(NULL)
, m_state(STATE_MENU)
{
	s_instance = this;
}

/*virtual*/ Game::~Game()
{

}

/*virtual*/ void Game::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{
	switch(m_state)
	{
	case STATE_MENU:
		m_menu.Update(window, frameTime);
		break;
	case STATE_INGAME:
		m_level->Update(window, frameTime);
		break;
	case STATE_PAUSE:
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed:
				m_state = STATE_QUIT;
				break;
			case sf::Event::KeyReleased:
				switch(event.key.code)
				{
				case sf::Keyboard::P:
				case sf::Keyboard::Escape:
					m_state = STATE_INGAME;
					break;
				default:
					break;
				}
				break;
			case sf::Event::MouseButtonReleased:
				sf::Rect<float> menuButton;
				if(menuButton.contains(sf::Mouse::getPosition(window)))
				{
					m_state = STATE_MENU;
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		if(m_level)
		{
			delete m_level;
		}
		window.close();
		break;
	}
}

/*virtual*/ void Game::Draw(sf::RenderWindow& window)
{
	switch(m_state)
	{
	case STATE_MENU:
		m_menu.Draw(window);
		break;
	case STATE_INGAME:
		m_level->Draw(window);
		break;
	case STATE_PAUSE:
		m_level->Draw(window);
		sf::RectangleShape rs;
		rs.setFillColor(sf::Color(0, 0, 0, 128));
		rs.setPosition(0, 0);
		rs.setSize(window.getSize());
		window.draw(rs);
		break;
	default:
		break;
	}
}

/*virtual*/ void Game::SwitchState(int state)
{
	m_state = state;
}
