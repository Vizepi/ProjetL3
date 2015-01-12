#include <SFML/Graphics.hpp>
#include "../header/Level.h"
#include "../header/RessourceLoader.h"
#include <ctime>

int main(int argc, char** argv)
{
	srand(time(NULL));
	//Création de la fenetre.
	sf::RenderWindow window(sf::VideoMode(960, 720), "Platformer");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//Chargement des textures.
	RessourceLoader::LoadTextures("textures.txt");
	Level level;
	//Chargement du level.
	level.LoadLevel();
	//Chargement du level test.
	level.CreateTestLevel();

	level.GenerateLevel();

	//creation du monde dans box2d.
	/*level.LoadLevelArray();
	sf::Clock frameTime;
	//Tant que la fenetre est ouverte.
	while(window.isOpen())
	{

		// Begin Update

		level.Update(window, frameTime);

		// End Update
		//Effacement de la fenetre.
		window.clear(sf::Color::Black);

		// Begin Draw
		//Remplissage de la fenetre.
		level.Draw(window);

		// End Draw
		//Affichage de la fenetre.
		window.display();
	}
	*/
}
