#include "../header/Game.h"

/*static*/ Game* Game::s_instance = NULL;

/*explicit*/ Game::Game()
: m_level(NULL)
, m_state(STATE_MENU)
, m_musicActive(true)
, m_soundsActive(true)
, m_fromMenu(true)
{
	s_instance = this;
}

/*virtual*/ Game::~Game()
{

}

/*virtual*/ void Game::Update(sf::RenderWindow& window, sf::Time& frameTime)
{
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
		m_pause.Update(window, frameTime);
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
		m_pause.Draw(window);
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

/*virtual*/ void Game::SetFrom(bool menu)
{
	m_fromMenu = menu;
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

/*virtual*/ bool Game::WasInMenu() const
{
	return m_fromMenu;
}

Level* Game::GetLevel()
{
	return m_level;
}

