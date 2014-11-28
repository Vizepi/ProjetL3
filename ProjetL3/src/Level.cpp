#include "../header/Level.h"
#ifndef SCALE
#define SCALE 30.f
#endif
#ifndef GRAVITY_SCALE
#define GRAVITY_SCALE 3.f
#endif // GRAVITY_SCALE
#ifndef BLOC_SIZE
#define BLOC_SIZE 48.f
#endif // BLOC_SIZE

Level::Level()
: m_gravity(b2Vec2(0.f, 9.81f*GRAVITY_SCALE))
, m_world(b2World(m_gravity))
{

}

void Level::CreateStaticObject(b2World& world, float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = world.CreateBody(&BodyDef);
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}
b2Body* Level::CreateDynamicObject(b2World& world, float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = world.CreateBody(&BodyDef);
    Body->SetFixedRotation(true);
	// Creation d'une premiere forme "glissante", rectangulaire.
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, ((height-0.5f)/2.f)/SCALE, b2Vec2(0, -1.f/SCALE), 0.f); // Dimension width * height-1 decalée de 1px vers le haut
    b2FixtureDef FixtureDef;
    FixtureDef.density = 10.f;
    FixtureDef.friction = 0.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    // Creation d'une seconde forme triangluaire sous le personnage avec friction.
    b2Vec2 down[3];
    down[0].Set((-(width-0.5f)/2.f)/SCALE, ((height-0.5f)/2.f)/SCALE);	//Forme du triangle sous le prersonnage :
    down[1].Set(((width-0.5f)/2.f)/SCALE, ((height-0.5f)/2.f)/SCALE);	// *                     *
    down[2].Set(0.f, ((height-0.4f)/2.f)/SCALE);						//            *
    Shape.Set(down, 3);
    b2FixtureDef FixtureDef2;
    FixtureDef2.density = 0.f;
    FixtureDef2.friction = 10.f;
    FixtureDef2.shape = &Shape;
    Body->CreateFixture(&FixtureDef2);
    return Body;
}

void Level::LoadLevel()
{
	m_character.LoadSprite();
	m_character.SetBody(CreateDynamicObject(m_world, 0, 0, CHARACTER_WIDTH,CHARACTER_HEIGHT));
}

void Level::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{
	m_event.event(window, m_character, true);
	m_world.Step(frameTime.restart().asSeconds(), 8, 5);
	m_character.Update();

}
void Level::Draw(sf::RenderWindow& window)
{
	DrawLevelArray(window);
	window.draw(*m_character.GetSprite());
}

Character* Level::GetCharacter()
{
	return &m_character;
}

void Level::GenerateLevel()
{

}

void Level::CreateTestLevel()
{
	m_array.reserve(20);
	for(int i=0;i<20;i++)
	{
		m_array.push_back(std::vector<int>());
		m_array[i].reserve(15);
	}
	for(int i=0;i<20;i++)
	{
		for(int j=0;j<15;j++)
		{
			m_array[i].push_back(lt_empty);
		}
	}
	for(int i=0;i<20;i++)
	{
		if(i<15)
		{
			m_array[0][i] = lt_solid;
			m_array[19][i] = lt_solid;
		}
		m_array[i][0] = lt_solid;
		m_array[i][14] = lt_ground;
		if(i > 5 && i < 15)
			m_array[i][7] = lt_ground;
		if(i > 7 && i < 14)
			m_array[10][i] = lt_ladder;
	}
	m_array[10][7] = lt_cross;
	m_startPosition.Set(64.f / SCALE, 620.f / SCALE);
}

void Level::LoadLevelArray()
{
	if(m_array.size() == 0)
		return;
	int m_arrayWidth = m_array.size();
	int m_arrayHeight = m_array[0].size();
	for(int i=0;i<m_arrayWidth;i++)
	{
		for(int j=0;j<m_arrayHeight;j++)
		{
			if(m_array[i][j] == lt_ground || m_array[i][j] == lt_solid)
			{
				CreateStaticObject(m_world, i*BLOC_SIZE, j*BLOC_SIZE, BLOC_SIZE, BLOC_SIZE);
			}
		}
	}
	m_character.GetBody()->SetTransform(m_startPosition, m_character.GetBody()->GetAngle());
}

void Level::DrawLevelArray(sf::RenderWindow& window)
{
	if(m_array.size() == 0)
		return;
	int m_arrayWidth = m_array.size();
	int m_arrayHeight = m_array[0].size();
	for(int i=0;i<m_arrayWidth;i++)
	{
		for(int j=0;j<m_arrayHeight;j++)
		{
			sf::RectangleShape rs(sf::Vector2f(BLOC_SIZE, BLOC_SIZE));
			rs.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
			switch(m_array[i][j])
			{
			case lt_cross:
				rs.setFillColor(sf::Color::Blue);
				break;
			case lt_ground:
				rs.setFillColor(sf::Color::Red);
				break;
			case lt_ladder:
				rs.setFillColor(sf::Color::Green);
				break;
			case lt_solid:
				rs.setFillColor(sf::Color::Yellow);
				break;
			default:
				rs.setFillColor(sf::Color::Black);
				break;
			}
			window.draw(rs);
		}
	}
}
