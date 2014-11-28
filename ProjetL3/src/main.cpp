#include <SFML/Graphics.hpp>
#include "../header/Level.h"
#include "../header/RessourceLoader.h"
int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(960, 720), "Platformer");
	window.setFramerateLimit(60);
	RessourceLoader::LoadTextures("textures.txt");
	Level level;
	level.LoadLevel();
	level.CreateTestLevel();
	level.LoadLevelArray();
	sf::Clock frameTime;
	while(window.isOpen())
	{

		// Begin Update

		level.Update(window, frameTime);

		// End Update

		window.clear(sf::Color::Black);

		// Begin Draw

		level.Draw(window);

		// End Draw

		window.display();
	}
}
