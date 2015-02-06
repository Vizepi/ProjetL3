#include "../header/Option.hpp"

/*explicit*/ Option::Option()
: m_resolutionIndex(1)
, m_fullScreen(false)
, m_music(true)
, m_sound(true)
{
	m_background.setTexture(*RessourceLoader::GetTexture("Menu Background"));
	m_background.setPosition(0, 0);
	m_backButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_applyButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_fsButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_musicButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_soundsButton.setTexture(*RessourceLoader::GetTexture("Menu Button"));
	m_backText.setFont(*RessourceLoader::GetFont());
	m_backText.setString("Back");
	m_applyText.setFont(*RessourceLoader::GetFont());
	m_applyText.setString("Apply");
	m_fsText.setFont(*RessourceLoader::GetFont());
	m_fsText.setString("FullScreen");
	m_musicText.setFont(*RessourceLoader::GetFont());
	m_musicText.setString("Music");
	m_soundText.setFont(*RessourceLoader::GetFont());
	m_soundText.setString("Sounds");
	m_resolutionText.setFont(*RessourceLoader::GetFont());
	m_resolutionText.setString("Resolution");
	m_resolutionTexts.push_back("800 X 600");
	m_resolutionTexts.push_back("960 X 720");
	m_resolutionTexts.push_back("1024 X 768");
	m_resolutionTexts.push_back("1280 X 960");
	m_resolutionTexts.push_back("1280 X 1024");
	m_resolutionTexts.push_back("1366 X 768");
	m_resolutionTexts.push_back("1600 X 900");
	m_resolutionTexts.push_back("1920 X 1080");
	m_resolutions.push_back(sf::Vector2u(800, 600));
	m_resolutions.push_back(sf::Vector2u(920, 720));
	m_resolutions.push_back(sf::Vector2u(1024, 768));
	m_resolutions.push_back(sf::Vector2u(1280, 960));
	m_resolutions.push_back(sf::Vector2u(1280, 1024));
	m_resolutions.push_back(sf::Vector2u(1366, 768));
	m_resolutions.push_back(sf::Vector2u(1600, 900));
	m_resolutions.push_back(sf::Vector2u(1920, 1080));
	m_set.fullscreen = m_fullScreen;
	m_set.music = m_music;
	m_set.sound = m_sound;
	m_set.width = m_resolutions[m_resolutionIndex].x;
	m_set.height = m_resolutions[m_resolutionIndex].y;
}

/*virtual*/ Option::~Option()
{

}

/*virtual*/ void Option::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{

}

/*virtual*/ void Option::Draw(sf::RenderWindow& window)
{

}

/*virtual*/ void Option::Apply()
{

}

/*virtual*/ void Option::Discard()
{

}

