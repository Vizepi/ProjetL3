#include "../header/Level.h"
#ifndef SCALE
#define SCALE 30.f
#endif
#ifndef GRAVITY_SCALE
#define GRAVITY_SCALE 4.f
#endif // GRAVITY_SCALE
#ifndef BLOC_SIZE
#define BLOC_SIZE 48.f
#endif // BLOC_SIZE


Level::Level()
: m_gravity(b2Vec2(0.f, 15.f*GRAVITY_SCALE))
, m_world(b2World(m_gravity))
{
	m_ground.setTexture(*RessourceLoader::GetTexture("Skin01"));
	m_ground.setTextureRect(sf::IntRect(89,48,40,40));
	m_ground.setScale(48.f/40.f,48.f/40.f);

	m_ladder.setTexture(*RessourceLoader::GetTexture("Skin01"));
	m_ladder.setTextureRect(sf::IntRect(180,94,32,32));
	m_ladder.setScale(48.f/32.f,48.f/32.f);
	m_cross.setTexture(*RessourceLoader::GetTexture("Skin01"));
	m_cross.setTextureRect(sf::IntRect(175,14,40,40));
	m_cross.setScale(48.f/40.f,48.f/40.f);

	m_listener = new JumpListener(&m_character);
	m_world.SetContactListener(m_listener);
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

void Level::CreateSensor(b2World& world, float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = world.CreateBody(&BodyDef);
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.isSensor = true;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    FixtureDef.density = 1.f;
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
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE); // Dimension width * height-1 decalée de 1px vers le haut
    b2FixtureDef FixtureDef;
    FixtureDef.density = 10.f;
    FixtureDef.friction = 0.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);

	b2FixtureDef fixtureDef;
    b2PolygonShape rectangle;
	rectangle.SetAsBox(((width)/4.f)/SCALE,2/SCALE, b2Vec2(0, (height/2.f)/SCALE), 0.f);
	fixtureDef.shape = &rectangle;
	fixtureDef.isSensor = true;
	Body->CreateFixture(&fixtureDef);

    return Body;
}

void Level::LoadLevel()
{
	m_character.LoadSprite();
	m_character.SetBody(CreateDynamicObject(m_world, 0, 0, CHARACTER_WIDTH,CHARACTER_HEIGHT));
}

void Level::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{
	m_event.event(window, m_character);
	m_world.Step(frameTime.restart().asSeconds(), 8, 5);
	m_character.Update();
	// @TODO Changer view
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
		{
			m_array[10][i] = lt_ladder;

		}
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
			if(m_array[i][j] == lt_ladder || m_array[i][j] == lt_cross)
			{
				CreateSensor(m_world,i*BLOC_SIZE, j*BLOC_SIZE, BLOC_SIZE, BLOC_SIZE);
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
			case lt_cross://croisement entre une echelle et un sol
				/*rs.setFillColor(sf::Color::Blue);
				window.draw(rs);*/
				m_cross.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(m_cross);
				break;
			case lt_ground://sol
				//rs.setFillColor(sf::Color::Red);
				m_ground.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(m_ground);
				break;
			case lt_ladder://echelle
				//rs.setFillColor(sf::Color::Green);
				m_ladder.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(m_ladder);
				break;
			case lt_solid://bloc
				rs.setFillColor(sf::Color::Yellow);
				window.draw(rs);
				break;
			default:
				//rs.setFillColor(sf::Color::Black);
				//window.draw(rs);
				break;
			}
		}
	}
}

