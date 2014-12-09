#ifndef LEVEL_H
#define LEVEL_H
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Character.h"
#include "Event.h"

enum LevelType
{
	lt_empty = 0, // Bloc vide
	lt_solid, // Bloc de terre
	lt_ground, // Bloc de sol
	lt_ladder, // Bloc d'echelle
	lt_cross // Bloc de croisement entre un sol et une echelle
};

class Level
{
	public :
		Level(void);
		void CreateStaticObject(b2World& world, float x, float y, float width, float height);
		void CreateSensor(b2World& world, float x, float y, float width, float height);
		b2Body* CreateDynamicObject(b2World& world, float x, float y, float width, float height);
		void LoadLevel(void);
		void Draw(sf::RenderWindow& window);
		void Update(sf::RenderWindow& window, sf::Clock& frameTime);
		Character* GetCharacter(void);
		void GenerateLevel(void);
		void CreateTestLevel(void);
		void LoadLevelArray(void);
		void DrawLevelArray(sf::RenderWindow& window);

	protected:

	private :
		Event  m_event;
		b2Vec2 m_gravity;
		b2World m_world;
		Character m_character;
		std::vector<std::vector<int> > m_array;
		b2Vec2 m_startPosition;
		sf::Sprite m_ground;
		sf::Sprite m_ladder;
		sf::Sprite m_cross;
		JumpListener* m_listener;

};
#endif // LEVEL_H
