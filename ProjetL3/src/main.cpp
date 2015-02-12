#include <SFML/Graphics.hpp>
#include "../header/Game.h"
#include "../header/RessourceLoader.h"
#include <string>
#include <vector>

void ParseCommandeLine(int argc, char** argv, std::vector<std::string>& dataset);

int main(int argc, char** argv)
{
	// Récuperation des paramètres de la ligne de commande

	std::vector<std::string> dataset;
	dataset.push_back("data0.dat");
	dataset.push_back("data1.dat");
	dataset.push_back("data2.dat");
	dataset.push_back("data3.dat");
	ParseCommandeLine(argc, argv, dataset);

	//Création de la fenetre.
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Platformer");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//Chargement des textures.
	RessourceLoader::LoadTextures(dataset[0].c_str());
	RessourceLoader::LoadSounds(dataset[1].c_str());
	RessourceLoader::LoadMusics(dataset[2].c_str());
	RessourceLoader::LoadFonts(dataset[3].c_str());
	Game game;
	sf::Clock frameTime;
	//Tant que la fenetre est ouverte.
	while(window.isOpen())
	{
		sf::Time elapsed = frameTime.restart();
		// Begin Update
		game.Update(window, elapsed);

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
	RessourceLoader::ClearSounds();
	RessourceLoader::ClearMusics();
	RessourceLoader::ClearFonts();
}

void ParseCommandeLine(int argc, char** argv, std::vector<std::string>& dataset)
{
	if(argc > 1)
	{
		for(int i=1;i<argc;i++)
		{
			std::string current = argv[i];
			if(!current.substr(0, 10).compare("-textures:"))
			{
				dataset[0] = current.substr(10, std::string::npos);
				continue;
			}
			if(!current.substr(0, 8).compare("-sounds:"))
			{
				dataset[1] = current.substr(8, std::string::npos);
				continue;
			}
			if(!current.substr(0, 8).compare("-musics:"))
			{
				dataset[2] = current.substr(8, std::string::npos);
				continue;
			}
			if(!current.substr(0, 7).compare("-fonts:"))
			{
				dataset[3] = current.substr(7, std::string::npos);
				continue;
			}
		}
	}
}
