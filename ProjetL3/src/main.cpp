#include <SFML/Graphics.hpp>
#include "../header/Game.h"
#include "../header/RessourceLoader.h"

int main(int argc, char** argv)
{
	//Cr�ation de la fenetre.
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Platformer");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//Chargement des textures.
	RessourceLoader::LoadTextures("textures.txt");
	RessourceLoader::LoadFont("../../pictures/segoeui.ttf");
	/*Level level;
	//Chargement du level.
	level.LoadLevel();
	//Chargement du level test.
	level.CreateTestLevel();

	level.GenerateLevel();

	//creation du monde dans box2d.
	level.LoadLevelArray();*/
	Game game;
	sf::Clock frameTime;
	 //Debug view pour voir toute la map.
	//sf::View view(sf::FloatRect(0, 0, BLOC_SIZE * ROOM_WIDTH * LEVEL_WIDTH, BLOC_SIZE * ROOM_HEIGHT * LEVEL_HEIGHT));
	//window.setView(view);
	//Tant que la fenetre est ouverte.
	while(window.isOpen())
	{

		// Begin Update

		//level.Update(window, frameTime);
		game.Update(window, frameTime);

		// End Update
		//Effacement de la fenetre.
		window.clear(sf::Color::Black);

		// Begin Draw
		//Remplissage de la fenetre.
		//level.Draw(window);
		game.Draw(window);

		// End Draw
		//Affichage de la fenetre.
		window.display();
	}

}
