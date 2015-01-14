#include "../header/Level.h"

Level::Level()
: m_gravity(b2Vec2(0.f, 15.f*GRAVITY_SCALE))
, m_world(b2World(m_gravity))
, m_rand(new Random())
{
	m_listener = new JumpListener(&m_character);
	m_world.SetContactListener(m_listener);
}

Level::~Level()
{
	delete m_rand;
}

//Fonction qui crée un objet statique dans box2d.
void Level::CreateStaticObject(float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = m_world.CreateBody(&BodyDef);
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.shape = &Shape;
    FixtureDef.isSensor = false;
    Body->CreateFixture(&FixtureDef);
    std::cout << x << " " << y << " " << width << " " << height << std::endl;
}

//Fonction qui crée un sensor dans box2d.
void Level::CreateSensor(float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = m_world.CreateBody(&BodyDef);
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.isSensor = true;
    FixtureDef.shape = &Shape;
    FixtureDef.density = 1.f;
    Body->CreateFixture(&FixtureDef);
    std::cout << x << " " << y << " " << width << " " << height << std::endl;
}

//Fonction qui crée un objet dynamique dans box2d.
b2Body* Level::CreateDynamicObject(float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = m_world.CreateBody(&BodyDef);
    Body->SetFixedRotation(true);
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
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
	m_character.SetBody(CreateDynamicObject(0, 0, CHARACTER_WIDTH,CHARACTER_HEIGHT));
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
	int i, j;
	Room** tableauExemple = new Room*[6];
	for(i = 0; i<6; i++)
	{
			tableauExemple[i] = new Room[6];
	}
	for(i = 0; i<6; i++)
	{
		for(j = 0; j<6; j++)
		{
			tableauExemple[i][j].North = false;
			tableauExemple[i][j].South = false;
			tableauExemple[i][j].East = false;
			tableauExemple[i][j].West = false;
			tableauExemple[i][j].y=j;
			tableauExemple[i][j].x=i;
		}
	}
	//Création pile :
	stack<Room*> pile;


	if(m_array.size() == 0)
		return;
	// Récupération des dimension du niveau
	int arrayWidth = m_array.size();
	int arrayHeight = m_array[0].size();

	int rand_x = (m_rand->NextInt())%6;
	int rand_y = (m_rand->NextInt())%6;
	int rand_xend = (m_rand->NextInt())%6;
	int rand_yend = (m_rand->NextInt())%6;
	while(rand_x == rand_xend && rand_y == rand_yend)
	{
		rand_xend = (m_rand->NextInt())%6;
		rand_yend = (m_rand->NextInt())%6;
	}
	printf("x de depart :%d\ny de depart:%d\n", rand_x, rand_y);
	printf("x d'arrive :%d\ny d'arrive:%d\n", rand_xend, rand_yend);
	printf("hauteur : %d\nlargeur : %d\n", arrayWidth, arrayHeight);

	FindPath(rand_x, rand_y, rand_xend, rand_yend, MINIM_DISTANCE, (Room**)(tableauExemple), arrayWidth, arrayHeight, pile);
	Room* r_end = new Room();
	pile.push(r_end);
	pile.top()->x = rand_xend;
	pile.top()->y = rand_yend;
	pile.top()->North = false;
	pile.top()->South = false;
	pile.top()->East = false;
	pile.top()->West = false;
	for(i = 0; i<6; i++)
	{
		for(j = 0; j<6; j++)
		{
			printf("x :%d, j : %d, %d %d %d %d\n",tableauExemple[i][j].x, tableauExemple[i][j].y, tableauExemple[i][j].North,tableauExemple[i][j].East, tableauExemple[i][j].South, tableauExemple[i][j].West);
		}
		printf("\n");
	}
	for(j=0; j<6; j++)
	{
		for(i=0; i<6; i++)
		{
			if(tableauExemple[i][j].North == true ||tableauExemple[i][j].South == true || tableauExemple[i][j].East == true || tableauExemple[i][j].West == true)
			{
				std::cout <<"1 ";
			}
			else
			{
				std::cout <<"0 ";
			}
		}
		std::cout <<"\n";
	}
	//Création d'une deque
	deque<Room*> dq;
	printf("Pile :\n");
	while(!pile.empty())
	{
		dq.push_front(pile.top());
		std::cout<<pile.top()->x<<" "<<pile.top()->y<<std::endl;
		pile.pop();
	}
	printf("Deque :\n");
	SetRoom(dq);
	for (i=0; i<(signed)dq.size(); i++)
	{
		std::cout << dq[i]->x<<" " <<dq[i]->y<<" "<<dq[i]->North<<" "<<dq[i]->East<<" "<<dq[i]->South<<" "<<dq[i]->West<< std::endl;
	}

	for(i=0; i< 6; i++)
	{
		delete [] tableauExemple[i];
	}
	delete [] tableauExemple;

}

void Level::SetRoom(deque<Room*> &dq)
{
	for(int i = 0; i< (signed)dq.size()-1; i++)
	{
		if(dq.at(i)->x < dq.at(i+1)->x)
		{
			dq.at(i)->East = true;
			dq.at(i+1)->West = true;
		}
		else if(dq.at(i)->x > dq.at(i+1)->x)
		{
			dq.at(i)->West = true;
			dq.at(i+1)->East = true;
		}
		else if(dq.at(i)->y > dq.at(i+1)->y)
		{
			dq.at(i)->North = true;
			dq.at(i+1)->South = true;
		}
		else if(dq.at(i)->y < dq.at(i+1)->y)
		{
			dq.at(i)->South = true;
			dq.at(i+1)->North = true;
		}
	}
}
bool Level::FindPath(int x, int y, int xend, int yend, int minDistance, Room** t, int w, int h, stack<Room*> &pile)
{
	//std::cout << /*x << " " << y << " " << xend << " " << yend << " " << minDistance*/ pile.size() << std::endl;
	if((minDistance == 0 || minDistance == -1) && (x == xend && y == yend))
	{
		std::cout << "found" <<std::endl;
		return true;
	}
	if((minDistance < -1) || (x == xend && y == yend) || t[x][y].North == true ||t[x][y].South == true || t[x][y].East == true || t[x][y].West == true)
	{
		return false;
	}

	Room *room = new Room();
	room->x= x;
	room->y=y;
	room->North = t[x][y].North;

	room->East = t[x][y].East;

	room->South = t[x][y].South;

	room->West = t[x][y].West;
	//ON MET DANS LA PILE
	pile.push(room);

	bool north = false, south = false, east = false, west = false;
	int r = (m_rand->NextInt())%4;
	while(!(north && south && east && west))
	{
		switch(r)
		{
			case 0 :
				if((y-1)<0 || north)
				{
					north = true;
					break;
				}
				t[x][y].North = true;
				if (FindPath(x,y-1,xend, yend, minDistance - 1, t, w, h, pile) == true)
					return true;//north
				north = true;
				break;
			case 1 :
				if((y+1)>5 || south)
				{
					south = true;
					break;
				}
				t[x][y].South = true;
				if (FindPath(x,y+1,xend, yend, minDistance - 1, t, w, h, pile) == true)
					return true;//south
				south = true;
				break;
			case 2 :
				if((x+1)>5 || east)
				{
					east = true;
					break;
				}
				t[x][y].East = true;
				if (FindPath(x+1,y,xend, yend, minDistance - 1, t, w, h, pile) == true)
					return true;//East
				east = true;
				break;
			case 3 :
				if((x-1)<0 || west)
				{
					west = true;
					break;
				}
				t[x][y].West = true;
				if (FindPath(x-1,y,xend, yend, minDistance - 1, t, w, h, pile) == true)
					return true;//west
				west = true;
				break;

		}
		t[x][y].North = false;
		t[x][y].South = false;
		t[x][y].East = false;
		t[x][y].West = false;
		r = (r+1) %4;
	}
	delete pile.top();
	//ON DEPILE
	pile.pop();
	return false;

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
	m_array[19][14] = lt_solid;
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
	std::vector<bool> mark;
	for(int i=0;i<mul;i++)
		mark.push_back(false); // Initialisation du tableau de booléens
    int width = 0;
    int height = 0;
    int x, y;
    int prev[3] = {0, 0, 0}; // {type, x, y}
    int state;
	for(int i=0;i<arrayHeight;i++)
	{
		for(int j=0;j<arrayWidth;j++)
		{
			if(mark[j*arrayWidth + i])
				std::cout << "1 ";
			else
				std::cout << "0 ";
		}
		std::cout << std::endl;
	}
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
							// Ligne non terminée.
							if(!SAME_LEVELTYPE(m_array[k][currentY],  prev[0]))
							{
								isOk = false;
								break;
							}
						}
						if(isOk)
						{
							// Marquage.
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
					std::cout << "static " << prev[1] << " " << prev[2] << " " << width << " " << height << std::endl;
					x = prev[1];
					y = prev[2];
					CreateStaticObject(x * BLOC_SIZE, y * BLOC_SIZE, width * BLOC_SIZE, height * BLOC_SIZE);
					for(int i=0;i<arrayHeight;i++)
					{
						for(int j=0;j<arrayWidth;j++)
						{
							if(mark[j*arrayWidth + i])
								std::cout << "1 ";
							else
								std::cout << "0 ";
						}
						std::cout << std::endl;
					}
					std::cout << "ok" << std::endl;
					break;
				case lt_ladder:
				case lt_cross:
					x = prev[1];
					y = prev[2];
					std::cout << "sensor " << prev[1] << " " << prev[2] << " " << width << " " << height << std::endl;
					CreateSensor(x * BLOC_SIZE, y * BLOC_SIZE, width * BLOC_SIZE, height * BLOC_SIZE);
					for(int i=0;i<arrayHeight;i++)
					{
						for(int j=0;j<arrayWidth;j++)
						{
							if(mark[j*arrayWidth + i])
								std::cout << "1 ";
							else
								std::cout << "0 ";
						}
						std::cout << std::endl;
					}
					std::cout << "ok" << std::endl;
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
	sprite.setScale(BLOC_SIZE/RS_BLOC_SIZE, BLOC_SIZE/RS_BLOC_SIZE);
	for(int i=0;i<arrayWidth;i++)
	{
		for(int j=0;j<arrayHeight;j++)
		{
			if(m_array[i][j] == lt_empty)
				continue;
			int x, y;
			int north = lt_empty, south = lt_empty, east = lt_empty, west = lt_empty;
			if(j-1 >= 0)
				north = m_array[i][j-1];
			if(j+1 < arrayHeight)
				south = m_array[i][j+1];
			if(i-1 >= 0)
				east = m_array[i-1][j];
			if(i+1 < arrayWidth)
				west = m_array[i+1][j];
			GetTextureCoords(&x, &y, m_array[i][j], north, east, south, west);
			sprite.setTextureRect(sf::IntRect(RS_POS(x), RS_POS(y), RS_BLOC_SIZE, RS_BLOC_SIZE));
			sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
			window.draw(sprite);
			/*switch(m_array[i][j])
			{
			case lt_cross://croisement entre une echelle et un sol
				sprite.setTextureRect(CLIP_CROSS);
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			case lt_ground://sol
				sprite.setTextureRect(CLIP_GROUND);
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			case lt_ladder://echelle
				sprite.setTextureRect(CLIP_LADDER);
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			case lt_solid://bloc
				sprite.setTextureRect(CLIP_SOLID);
				sprite.setPosition(i*BLOC_SIZE, j*BLOC_SIZE);
				window.draw(sprite);
				break;
			default:
				break;
			}*/
		}
	}
	/*b2Body* bodyIterator = m_world.GetBodyList();
	while(bodyIterator)
	{
		sf::RectangleShape rs;
		float width = bodyIterator->GetFixtureList()->GetAABB(0).upperBound.x - bodyIterator->GetFixtureList()->GetAABB(0).lowerBound.x;
		float height = bodyIterator->GetFixtureList()->GetAABB(0).upperBound.y - bodyIterator->GetFixtureList()->GetAABB(0).lowerBound.y;
		rs.setSize(sf::Vector2f(width * SCALE, height * SCALE));
		rs.setPosition(bodyIterator->GetPosition().x * SCALE, bodyIterator->GetPosition().y * SCALE);
		rs.setFillColor(sf::Color(m_rand->NextInt()%255, m_rand->NextInt()%255, m_rand->NextInt()%255));
		window.draw(rs);
		bodyIterator = bodyIterator->GetNext();
	}*/
}

void Level::GetTextureCoords(int* x, int* y, int center, int north, int east, int south, int west)
{

	switch(center)
	{
	case lt_ground:
	case lt_solid:
		switch(north) // Rien au dessus
		{
		case lt_empty:
		case lt_ladder:
			switch(south)
			{
			case lt_empty: // Rien en dessous
			case lt_ladder:
				switch(east)
				{
				case lt_empty: // Rien à droite
				case lt_ladder:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 0;
						*y = 0;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 1;
						*y = 0;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				case lt_ground: // Bloc à droite
				case lt_solid:
				case lt_cross:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 3;
						*y = 0;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 2;
						*y = 0;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				default:
					*x = 0;
					*y = 0;
					break;
				}
				break;
			case lt_ground: // Bloc en dessous
			case lt_solid:
			case lt_cross:
				switch(east)
				{
				case lt_empty: // Rien à droite
				case lt_ladder:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 0;
						*y = 1;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 1;
						*y = 1;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				case lt_ground: // Bloc à droite
				case lt_solid:
				case lt_cross:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 3;
						*y = 1;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 2;
						*y = 1;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				default:
					*x = 0;
					*y = 0;
					break;
				}
				break;
			default:
				*x = 0;
				*y = 0;
				break;
			}
			break;
		case lt_ground: // Bloc au dessus
		case lt_solid:
		case lt_cross:
			switch(south)
			{
			case lt_empty: // Rien en dessous
			case lt_ladder:
				switch(east)
				{
				case lt_empty: // Rien à droite
				case lt_ladder:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 0;
						*y = 3;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 1;
						*y = 3;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				case lt_ground: // Bloc à droite
				case lt_solid:
				case lt_cross:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 3;
						*y = 3;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 2;
						*y = 3;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				default:
					*x = 0;
					*y = 0;
					break;
				}
				break;
			case lt_ground: // Bloc en dessous
			case lt_solid:
			case lt_cross:
				switch(east)
				{
				case lt_empty: // Rien à droite
				case lt_ladder:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 0;
						*y = 2;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 1;
						*y = 2;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				case lt_ground: // Bloc à droite
				case lt_solid:
				case lt_cross:
					switch(west)
					{
					case lt_empty: // Rien à gauche
					case lt_ladder:
						*x = 3;
						*y = 2;
						break;
					case lt_ground: // Bloc à gauche
					case lt_solid:
					case lt_cross:
						*x = 2;
						*y = 2;
						break;
					default:
						*x = 0;
						*y = 0;
						break;
					}
					break;
				default:
					*x = 0;
					*y = 0;
					break;
				}
				break;
			default:
				*x = 0;
				*y = 0;
				break;
			}
			break;
		default:
			*x = 0;
			*y = 0;
			break;
		}
		break;
	case lt_ladder:
		*x = 2;
		*y = 7;
		return;
	case lt_cross:
		*y = 4;
		switch(east)
		{
		case lt_empty:
		case lt_ladder:
			switch(west)
			{
			case lt_empty:
			case lt_ladder:
				*x = 0;
				*y = 7;
				break;
			case lt_solid:
			case lt_ground:
			case lt_cross:
				*x = 1;
				break;
			default:
				break;
			}
			break;
		case lt_solid:
		case lt_ground:
		case lt_cross:
			switch(west)
			{
			case lt_empty:
			case lt_ladder:
				*x = 3;
				break;
			case lt_solid:
			case lt_ground:
			case lt_cross:
				*x = 2;
				break;
			default:
				break;
			}
			break;
		default:
			*x = 0;
			*y = 0;
			break;
		}
		break;
	default:
		*x = 0;
		*y = 0;
		break;
	}
}

