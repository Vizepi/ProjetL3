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

using namespace std;

//#define SHOW_ALL_MAP
//#define SHOW_COLLISION_BOXES
//#define SINGLE_BODY_LEVEL

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

#define CLIP_SOLID sf::IntRect(RS_POS(2), RS_POS(2), RS_BLOC_SIZE, RS_BLOC_SIZE)
#define CLIP_GROUND sf::IntRect(RS_POS(2), RS_POS(1), RS_BLOC_SIZE, RS_BLOC_SIZE)
#define CLIP_GROUNDWEST sf::IntRect(RS_POS(0), RS_POS(1), RS_BLOC_SIZE, RS_BLOC_SIZE)
#define CLIP_GROUNDEAST sf::IntRect(RS_POS(0), RS_POS(3), RS_BLOC_SIZE, RS_BLOC_SIZE)
#define CLIP_LADDER sf::IntRect(RS_POS(2), RS_POS(7), RS_BLOC_SIZE, RS_BLOC_SIZE)
#define CLIP_CROSS sf::IntRect(RS_POS(0), RS_POS(7), RS_BLOC_SIZE, RS_BLOC_SIZE)

#define MINIM_DISTANCE 12

#define ROOM_WIDTH 10
#define ROOM_HEIGHT 10

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
		void Update(sf::RenderWindow& window, sf::Clock& frameTime);
		Character* GetCharacter(void);
		void GenerateLevel(void);
		void CreateLevel(Room** t, deque<Room*> &dq);
		void CreateTestLevel(void);
		void LoadLevelArray(void);
		void DrawLevelArray(sf::RenderWindow& window);
		void SetRoom(deque<Room*> &dq);
		bool FindPath(int x, int y, int xend, int yend, int minimDistance, Room** tableauExemple, int w, int h, stack<Room*> &pile);
		void GetTextureCoords(int* x, int* y, int center, int north, int east, int south, int west);

	protected:

	private:
		Event  m_event;
		b2Vec2 m_gravity;
		b2World m_world;
		#ifdef SINGLE_BODY_LEVEL
		b2Body* m_levelBody;
		#endif
		Character m_character;
		std::vector<std::vector<int> > m_array;
		b2Vec2 m_startPosition;
		JumpListener* m_listener;
		Random* m_rand;
		sf::Sprite tardis;
		sf::Sprite emy;
		bool gagne;
		std::vector<Animation*> m_anim;
		Animation* m_currentAnimation;
		sf::Sprite m_coin;

};
#endif // LEVEL_H
