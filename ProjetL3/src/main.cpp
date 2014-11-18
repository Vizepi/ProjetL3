#include <SFML/Graphics.hpp>
#include "../header/Level.h"
#include "../header/RessourceLoader.h"

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Platformer");
	window.setFramerateLimit(60);
	RessourceLoader::LoadTextures("textures.txt");
	Level level;
	level.LoadLevel();
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		// Begin Update

		// End Update

		window.clear(sf::Color::Black);
		level.Draw(window);
		// Begin Render

		// End Render

		window.display();
	}
}
