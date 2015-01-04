#include "../header/Level.h"

Level::Level()
: m_gravity(b2Vec2(0.f, 15.f*GRAVITY_SCALE))
, m_world(b2World(m_gravity))
{
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
	int mul = m_arrayWidth * m_arrayHeight;
	bool* mark = new bool[mul];
	for(int i=0;i<mul;i++)
		mark[i] = false;
	int x = 0;
	int y = 0;
    int width = 0;
    int height = 0;
    int prev[3] = {0, 0, 0}; // {type, x, y}
    int state = 0;
	for(int i=0;i<m_arrayHeight;i++)
	{
		for(int j=0;j<m_arrayWidth;j++)
		{
			if(!mark[i * m_arrayWidth + j])
			{
				state = 0;
				int currentX = i, currentY = j;
				while(state != 3)
				{
					mark[currentX * m_arrayWidth + currentY] = true;
					if(state == 0) // Debut de rectangle
					{
						prev[0] = m_array[i][j];
						prev[1] = i;
						prev[2] = j;
						x = i;
						y = j;
						width = 0;
						height = 0;
						state = 1;
					}
					if(state == 1) // Recherche de la longueur
					{
						if(prev[1] == currentX)
						{
							if(prev[0] == m_array[currentX][currentY])
							{
								width++;
							}
							else
							{

							}
						}
						else
						{
							height++;
							state = 2;
						}
					}
					if(state == 2) // Recherche de la hauteur
					{

					}
				}
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
	sf::Sprite sprite;
	sprite.setTexture(*RessourceLoader::GetTexture("Skin01"));
	sprite.setScale(48.f/40.f, 48.f/40.f);
	for(int i=0;i<m_arrayWidth;i++)
	{
		for(int j=0;j<m_arrayHeight;j++)
		{
			switch(m_array[i][j])
			{
			case lt_cross://croisement entre une echelle et un sol
				m_sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(m_sprite);
				break;
			case lt_ground://sol
				m_sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(m_sprite);
				break;
			case lt_ladder://echelle
				m_sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(m_sprite);
				break;
			case lt_solid://bloc
				m_sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(m_sprite);
				break;
			default:
				break;
			}
		}
	}
}

