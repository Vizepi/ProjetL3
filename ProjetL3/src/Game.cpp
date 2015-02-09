#include "../header/Game.h"

/*static*/ Game* Game::s_instance = NULL;

/*explicit*/ Game::Game()
: m_level(NULL)
, m_state(STATE_MENU)
, m_musicActive(true)
, m_soundsActive(true)
{
	s_instance = this;
}

/*virtual*/ Game::~Game()
{

}

/*virtual*/ void Game::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{
	sf::Rect<int> menuButton;
	float cX = 0.f;
	float cY = 0.f;
	float wX = 0.f;
	float wY = 0.f;
	if(m_level != NULL)
	{
		cX = m_level->GetCharacter()->GetBody()->GetPosition().x * SCALE;
		cY = m_level->GetCharacter()->GetBody()->GetPosition().y * SCALE;
		wX = window.getSize().x;
		wY = window.getSize().y;
	}
	switch(m_state)
	{
	case STATE_MENU:
		m_menu.Update(window, frameTime);
		break;
	case STATE_INGAME:
		if(/*gagner*/ m_level->GetWin())
		{
			m_win.Update(window, frameTime);
			m_state = STATE_WIN;
		}
		else if(/*perdu*/m_level->GetCharacter()->GetLife() <=0)
		{
			m_lose.Update(window, frameTime);
			m_state = STATE_LOSE;
		}
		else
		{
			m_level->Update(window, frameTime);
		}
		break;
	case STATE_WIN:
		m_win.Update(window, frameTime);
	break;
	case STATE_LOSE:
		m_lose.Update(window, frameTime);
	break;
	case STATE_OPTION:
		m_option.Update(window, frameTime);
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
				if(menuButton.contains(sf::Mouse::getPosition(window)))
				{
					m_state = STATE_MENU;
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
		if(cX < wX/2.f)
			cX = wX/2.f;
		if(cX > LEVEL_WIDTH * ROOM_WIDTH * BLOC_SIZE - wX/2.f)
			cX = LEVEL_WIDTH * ROOM_WIDTH * BLOC_SIZE - wX/2.f;
		if(cY < wY/2.f)
			cY = wY/2.f;
		if(cY > LEVEL_HEIGHT * ROOM_HEIGHT * BLOC_SIZE - wY/2.f)
			cY = LEVEL_HEIGHT * ROOM_HEIGHT * BLOC_SIZE - wY/2.f;
		window.setView(sf::View(sf::Vector2f(cX, cY), sf::Vector2f(wX, wY)));
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
	sf::RectangleShape rs;
	sf::Text pauseText;
	switch(m_state)
	{
	case STATE_MENU:
		m_menu.Draw(window);
		break;
	case STATE_INGAME:
		m_level->Draw(window);
		break;
	case STATE_WIN:
		m_win.Draw(window);
		break;
	case STATE_LOSE:
		m_lose.Draw(window);
		break;
	case STATE_OPTION:
		m_option.Draw(window);
		break;
	case STATE_PAUSE:
		m_level->Draw(window);
		rs.setFillColor(sf::Color(0, 0, 0, 128));
		rs.setPosition(window.getView().getCenter().x-window.getSize().x/2, window.getView().getCenter().y-window.getSize().y/2);
		rs.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
		window.draw(rs);
		pauseText.setFont(*RessourceLoader::GetFont());
		pauseText.setString("PAUSE");
		pauseText.setColor(sf::Color::White);
		pauseText.setPosition(window.getView().getCenter().x-pauseText.getGlobalBounds().width/2, window.getView().getCenter().y-pauseText.getGlobalBounds().height/2);
		window.draw(pauseText);
		break;
	default:
		break;
	}
}

/*virtual*/ void Game::NewLevel()
{
	if(m_level)
	{
		delete m_level;
	}
	m_level = new Level();
	m_level->LoadLevel();
	m_level->GenerateLevel();
	m_level->LoadLevelArray();
}

/*virtual*/ void Game::NewLevelReplay()
{
	int seed = m_level->GetSeed();
	if(m_level)
	{
		delete m_level;
	}
	m_level = new Level(seed);
	m_level->LoadLevel();
	m_level->GenerateLevel();
	m_level->LoadLevelArray();
}

/*virtual*/ void Game::SwitchState(int state)
{
	m_state = state;
}

/*virtual*/ void Game::SetMusic(bool state)
{
	m_musicActive = state;
}

/*virtual*/ void Game::SetSounds(bool state)
{
	m_soundsActive = state;
}

/*virtual*/ void Game::SetShadow(bool state)
{
	m_shadowActive = state;
}

/*virtual*/ bool Game::IsMusicActive() const
{
	return m_musicActive;
}

/*virtual*/ bool Game::IsSoundsActive() const
{
	return m_soundsActive;
}

/*virtual*/ bool Game::IsShadowActive() const
{
	return m_shadowActive;
}

Level* Game::GetLevel()
{
	return m_level;
}

