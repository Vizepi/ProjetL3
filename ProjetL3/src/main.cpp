#include <SFML/Graphics.hpp>
#include "../header/Game.h"
#include "../header/RessourceLoader.h"

int main(int argc, char** argv)
{
	//Création de la fenetre.
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Platformer");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//Chargement des textures.
	RessourceLoader::LoadTextures("data.dat");
	RessourceLoader::LoadFont("../../pictures/segoeui.ttf");
	Game game;
	sf::Clock frameTime;
	//Tant que la fenetre est ouverte.
	while(window.isOpen())
	{

		// Begin Update
		game.Update(window, frameTime);

		// End Update
		//Effacement de la fenetre.
		window.clear(sf::Color::Black);

		// Begin Draw
		//Remplissage de la fenetre.
		game.Draw(window);

		// End Draw
		//Affichage de la fenetre.
		window.display();
	}
	RessourceLoader::ClearTextures();
	RessourceLoader::ClearFont();
}
