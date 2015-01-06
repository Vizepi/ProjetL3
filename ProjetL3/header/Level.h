#ifndef LEVEL_H
#define LEVEL_H
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Character.h"
#include "Event.h"

#ifndef SCALE
#define SCALE 30.f
#endif
#ifndef GRAVITY_SCALE
#define GRAVITY_SCALE 4.f
#endif // GRAVITY_SCALE
#ifndef BLOC_SIZE
#define BLOC_SIZE 48.f
#endif // BLOC_SIZE

#define RS_BLOC_SIZE 40
#define RS_POS(x) RS_BLOC_SIZE * x

#define SOLID_MASK 0x3
#define SENSOR_MASK 0xC

enum LevelType
{
	lt_empty = 0x0, // Bloc vide
	lt_solid = 0x1, // Bloc de terre
	lt_ground = 0x2, // Bloc de sol
	lt_ladder = 0x4, // Bloc d'echelle
	lt_cross = 0x8 // Bloc de croisement entre un sol et une echelle
};

// Si les variables a et b sont de même type (static ou sensor), retourne true.
#define SAME_LEVELTYPE(a, b) (((a & 0x3) && (b & 0x3)) || ((a & 0xC) && (b & 0xC)))


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
		JumpListener* m_listener;

};
#endif // LEVEL_H
