#include "../header/Level.h"

Level::Level()
: m_gravity(b2Vec2(0.f, 15.f*GRAVITY_SCALE))
, m_world(b2World(m_gravity))
{
	m_listener = new JumpListener(&m_character);
	m_world.SetContactListener(m_listener);
}

//Fonction qui crée un objet statique dans box2d.
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

//Fonction qui crée un sensor dans box2d.
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
    FixtureDef.density = 1.f;
    Body->CreateFixture(&FixtureDef);
}

//Fonction qui crée un objet dynamique dans box2d.
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
//Chargement du level.
void Level::LoadLevel()
{
	//Chargement du sprite du personnage.
	m_character.LoadSprite();
	//Création du corp physique du personnage.
	m_character.SetBody(CreateDynamicObject(m_world, 0, 0, CHARACTER_WIDTH,CHARACTER_HEIGHT));
}

//Fonction de mise a jour.
void Level::Update(sf::RenderWindow& window, sf::Clock& frameTime)
{
	//On regarde les evenements de la fenetre.
	m_event.event(window, m_character);
	m_world.Step(frameTime.restart().asSeconds(), 8, 5);
	m_character.Update();
	// @TODO Changer view
}

//Remplissage de la fenetre
void Level::Draw(sf::RenderWindow& window)
{
	DrawLevelArray(window);
	window.draw(*m_character.GetSprite());
}

Character* Level::GetCharacter()
{
	return &m_character;
}

//Fonction de generation aléatoire d'un level.
void Level::GenerateLevel()
{

}

//Création du level test.
void Level::CreateTestLevel()
{
	//Tableau de jeu, reservation de 20 en longueur.
	m_array.reserve(20);
	for(int i=0;i<20;i++)
	{
		m_array.push_back(std::vector<int>());
		//Reservation de 15 en largeur.
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

//Fonction de creation du monde dans box2d.
void Level::LoadLevelArray()
{
	if(m_array.size() == 0)
		return;
	// Récupération des dimension du niveau
	int arrayWidth = m_array.size();
	int arrayHeight = m_array[0].size();
	int mul = arrayWidth * arrayHeight; // Nombre de blocs du niveau
	bool* mark = new bool[mul];
	for(int i=0;i<mul;i++)
		mark[i] = false; // Initialisation du tableau de booléens, pour marquer les blocs déjà crées.
    int width = 0;
    int height = 0;
    int prev[3] = {0, 0, 0}; // {type, x, y}
    int state;
	for(int j=0;j<arrayHeight;j++)
	{
		for(int i=0;i<arrayWidth;i++)
		{
			if(!mark[i * arrayWidth + j]) // Si on n'est pas encore passé sur le bloc
			{
				if(m_array[i][j] == lt_empty)//Si le bloc est vide on continue
				{
					mark[i * arrayWidth + j] = true;
					continue;
				}
				state = 0;
				int currentX = i, currentY = j;
				while(state != 3)
				{
					if(state == 0) // Debut de rectangle
					{
						mark[currentX * arrayWidth + currentY] = true;
						prev[0] = m_array[i][j];
						prev[1] = i;
						prev[2] = j;
						width = 1;
						height = 1;
						state = 1;
					}
					if(state == 1) // Recherche de la longueur
					{
						// Avancement des positions courantes et passage
						currentX++;
						if(currentX == arrayWidth)//Si on est au bout de la fenetre en X
						{
							currentX = prev[1];
							currentY++;
							if(currentY == arrayHeight)//Si on est au bout de la fenetre en Y
							{
								state = 3;
							}
							state = 2;
						}
						else
						{
							if(SAME_LEVELTYPE(prev[0], m_array[currentX][currentY]))
							{
								width++;
								mark[currentX * arrayWidth + currentY] = true;
							}
							else
							{
								currentX = prev[1];
								currentY++;
								if(currentY == arrayHeight)
								{
									state = 3;
								}
								state = 2;
							}
						}
					}
					if(state == 2) // Recherche de la hauteur
					{
						bool isOk = true;
						int lineEnd = prev[1]+width;
						for(int k=prev[1];k<lineEnd;k++)
						{
							if(m_array[k][currentY] != prev[0])
							{
								isOk = false;
								break;
							}
						}
						if(isOk)
						{
							for(int k=prev[1];k<lineEnd;k++)
								mark[k * arrayWidth + currentY] = true;
							height++;
							currentX = prev[1];
							currentY++;
							if(currentY == arrayHeight)
							{
								state = 3;
							}
						}
						else
							state = 3;
					}
				}
				switch(prev[0])
				{
				case lt_solid:
				case lt_ground:
					CreateStaticObject(m_world, prev[1] * 48.f, prev[2] * 48.f, width * 48.f, height * 48.f);
					std::cout << "static" << std::endl;
					break;
				case lt_ladder:
				case lt_cross:
					CreateSensor(m_world, prev[1] * 48.f, prev[2] * 48.f, width * 48.f, height * 48.f);
					std::cout << "sensor" << std::endl;
					break;
				default:
					break;
				}
			}
		}
	}
	m_character.GetBody()->SetTransform(m_startPosition, m_character.GetBody()->GetAngle());
}

//Dessine le tableau du niveau.
void Level::DrawLevelArray(sf::RenderWindow& window)
{
	if(m_array.size() == 0)
		return;
	int arrayWidth = m_array.size();
	int arrayHeight = m_array[0].size();
	sf::Sprite sprite;
	sprite.setTexture(*RessourceLoader::GetTexture("Skin01"));
	sprite.setScale(48.f/40.f, 48.f/40.f);
	for(int i=0;i<arrayWidth;i++)
	{
		for(int j=0;j<arrayHeight;j++)
		{
			switch(m_array[i][j])
			{
			case lt_cross://croisement entre une echelle et un sol
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			case lt_ground://sol
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			case lt_ladder://echelle
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			case lt_solid://bloc
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			default:
				break;
			}
		}
	}
}

