#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Character.h"
#include "Event.h"
#include "Random.h"
#include <cstdlib>
#include <time.h>
#include <stack>
#include <deque>
#include <cassert>
#include <cmath>
#include <sstream>

using namespace std;

//#define SHOW_ALL_MAP
//#define SHOW_COLLISION_BOXES
#define LITTLE_BLOCS_PHYSIC
#define HIDE_LIGHT

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720


#ifndef SCALE
#define SCALE 30.0
#endif
#ifndef GRAVITY_SCALE
#define GRAVITY_SCALE 4.0
#endif // GRAVITY_SCALE
#ifndef BLOC_SIZE
#define BLOC_SIZE 48.0
#endif // BLOC_SIZE

#define RS_BLOC_SIZE 46.0
#define RS_POS(x) RS_BLOC_SIZE * x

#define SOLID_MASK 0x3
#define SENSOR_MASK 0xC

#define ROOM_WIDTH 15
#define ROOM_HEIGHT 15
#define LEVEL_WIDTH 6
#define LEVEL_HEIGHT 6

#define MINIM_DISTANCE ((int)(1 * (LEVEL_WIDTH + LEVEL_HEIGHT)))

#define NB_COINS 20

enum LevelType
{
	lt_empty = 0x0, // Bloc vide
	lt_solid = 0x1, // Bloc de terre
	lt_ground = 0x2, // Bloc de sol
	lt_ladder = 0x4, // Bloc d'echelle
	lt_cross = 0x8 // Bloc de croisement entre un sol et une echelle
};

// Si les variables a et b sont de même type (static ou sensor), retourne true.
#define SAME_LEVELTYPE(a, b) (((a & SOLID_MASK) && (b & SOLID_MASK)) || ((a & SENSOR_MASK) && (b & SENSOR_MASK)))

struct Room
{
	bool North;
	bool South;
	bool East;
	bool West;
	int x;
	int y;
	int rand_x;
	int rand_y;
};

class Level
{
	public:
		Level(void);
		~Level(void);
		void CreateStaticObject(float x, float y, float width, float height);
		void CreateSensor(float x, float y, float width, float height);
		b2Body* CreateDynamicObject(float x, float y, float width, float height);
		void LoadLevel(void);
		void Draw(sf::RenderWindow& window);
		void DrawHUB(int winX, int winY, int winW, int winH, sf::RenderWindow& window);
		void Update(sf::RenderWindow& window, sf::Clock& frameTime);
		Character* GetCharacter(void);
		void GenerateLevel(void);
		void CreateLevel(Room** t, deque<Room*> &dq);
		void CreateWalls(deque<Room*> &dq);
		void CreateGenerateLevel(deque<Room*> &dq);
		void PutCoin(void);
		void CreateTestLevel(void);
		void LoadLevelArray(void);
		void DrawBackground(sf::RenderWindow& window);
		void DrawLevelArray(sf::RenderWindow& window);
		void SetRoom(deque<Room*> &dq);
		bool FindPath(int x, int y, int xend, int yend, int minimDistance, Room** tableauExemple, int w, int h, stack<Room*> &pile);
		void GetTextureCoords(int* x, int* y, int center, int north, int east, int south, int west);

	protected:

	private:
		Event  m_event;
		b2Vec2 m_gravity;
		b2World m_world;
		Character m_character;
		std::vector<std::vector<int> > m_array;
		std::vector<sf::Vector2i> m_coins;
		b2Vec2 m_startPosition;
		JumpListener* m_listener;
		Random* m_rand;
		sf::Sprite tardis;
		sf::Sprite emy;
		bool gagne;
		std::vector<Animation*> m_anim;
		Animation* m_currentAnimation;
		sf::Sprite m_coin;
		Random* m_brokenLadderRandom;
		int m_lastLightAlpha;
		int m_coinsGet;

};
#endif // LEVEL_H
