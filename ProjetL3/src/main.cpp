#include <SFML/Graphics.hpp>

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "Platformer");
	sf::Clock frameTimer;
	window.setVerticalSyncEnabled(true);

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

		// Begin Render

		// End Render

		window.display();
	}
}
