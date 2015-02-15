/*

	Platformer Game - Made for the 3rd year of undergraduated project.
    Copyright (C) 2015  Corbat Lisa, Kieffer Joseph

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
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
//#define HIDE_LIGHT

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720


#ifndef SCALE
#define SCALE 25.0
#endif
#ifndef GRAVITY_SCALE
#define GRAVITY_SCALE 5.0
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

#define MINIM_DISTANCE ((int)(1.5 * (LEVEL_WIDTH + LEVEL_HEIGHT)))

#define NB_COINS 30

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

#define SIDE_CENTER 0x0
#define SIDE_LEFT	0x1
#define SIDE_RIGHT	0x2

class Level
{
	public:
		Level(void);
		Level(int seed);
		~Level(void);
		void CreateStaticObject(float x, float y, float width, float height, int side = SIDE_CENTER);
		void CreateSensor(float x, float y, float width, float height);
		b2Body* CreateDynamicObject(float x, float y, float width, float height);
		void LoadLevel(void);
		void Draw(sf::RenderWindow& window);
		void DrawHUB(float winX, float winY, float winW, float winH, sf::RenderWindow& window);
		void Update(sf::RenderWindow& window, sf::Time& frameTime);
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
		void Dump(void);
		bool GetWin(void);
		int GetSeed(void);

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
		sf::Sprite m_start;
		sf::Sprite m_target;
		bool m_win;
		std::vector<Animation*> m_anim;
		Animation* m_currentAnimation;
		sf::Sprite m_coin;
		Random* m_brokenLadderRandom;
		int m_lastLightAlpha;
		int m_coinsGet;
		sf::Font* m_font;
		sf::Sprite m_life;
		sf::Time m_timer;
		sf::Sprite m_clock;
		std::vector<sf::Vector2i> m_traps;
		sf::Sprite m_trapSprite;
		bool m_underTwenty;
};
#endif // LEVEL_H
