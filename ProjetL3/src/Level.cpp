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

	int rand_x = rand()%6;
	int rand_y = rand()%6;
	int rand_xend = rand()%6;
	int rand_yend = rand()%6;
	while(rand_x == rand_xend && rand_y == rand_yend)
	{
		rand_xend = rand()%6;
		rand_yend = rand()%6;
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
	int r = rand()%4;
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
/*

bool IsAPath(int x, int y, int xe, int ye, bool** t, int w, int h, bool** tPas)
{
	printf("bouh\n");
	std::cout << x << " " << y << " " << xe << " " << ye << " " << (t)[x][y] <<" " <<(tPas)[x][y]<< std::endl;
	if(x < 0 || x >= w || y < 0 || y >= h || (t)[x][y] || (tPas)[x][y])
	{
		printf("Retourne faux ici\n");
		return false;

	}
	if(x == xe && y == ye)
	{
		printf("Retourne vrai dans x==xe et y == ye\n");
		tPas[x][y]= true;
		return true;
	}
	tPas[x][y-1] = true;
	if(IsAPath(x, y-1, xe, ye, t, w, h, tPas))
	{
		printf("Retourne vrai nord\n");
		return true;
	}
	else
	{
		tPas[x][y-1] = false;
		tPas[x-1][y] = true;
		if(IsAPath(x-1, y, xe, ye, t, w, h, tPas))
		{
			printf("Retourne vrai ouest\n");
			return true;
		}
		else
		{
			tPas[x-1][y]= false;
			tPas[x][y+1]= true;
			if(IsAPath(x, y+1, xe, ye, t, w, h, tPas))
			{
				printf("Retourne vrai sud\n");
				return true;
			}
			else
			{
				tPas[x][y+1]= false;
				tPas[x+1][y]= true;
				if(IsAPath(x+1, y, xe, ye, t, w, h, tPas))
				{
					printf("Retourne vrai est\n");
					return true;
				}
				else
				{
					printf("Retourne faux dans le dernier else\n");
					tPas[x+1][y]= false;
				}
			}
		}
	}
	printf("Fin du isAPath\n");
	return false;
}

bool Level::FindPath(int x, int y, int xend, int yend, int minDistance, bool** tableauExemple, int w, int h)
{
	int i;
	printf("FindPpath\n");
	bool** tPas = new bool*[6];
	for(i = 0; i<6; i++)
	{
			tPas[i] = new bool[6];
	}
	for( i = 0; i<6; i++)
	{
		printf("boucle\n");
		for(int j = 0; j<6; j++)
		{
			printf("le j\n");
			tPas[i][j] = tableauExemple[i][j];
		}
	}
	printf("quatre\n");
	if(x == xend && y == yend && minDistance >= 0)
	{
		printf("un\n");
		tableauExemple[x][y] = true;
	}
	if(IsAPath(x, y, xend, yend, tableauExemple, w, h, (bool**)(tPas)))
	{
		printf("deux\n");
		bool ok = false;
		while(!ok)
		{
			int random = rand() % 4;
			switch(random)
			{
			case 0:
				if(y-1 < 0)
					break;
				if(!(tableauExemple)[x][y-1])
				{
					(tableauExemple)[x][y] = true;
					ok = FindPath(x, y-1, xend, yend, minDistance-1, tableauExemple, w, h);
					if(!ok)
						(tableauExemple)[x][y] = false;
				}
				break;
			case 1:
				if(y+1 >= h)
					break;
				if(!(tableauExemple)[x][y-1])
				{
					(tableauExemple)[x][y] = true;
					ok = FindPath(x, y+1, xend, yend, minDistance-1, tableauExemple, w, h);
					if(!ok)
						(tableauExemple)[x][y] = false;
				}
				break;
			case 2:
				if(x-1 < 0)
					break;
				if(!(tableauExemple)[x-1][y])
				{
					(tableauExemple)[x][y] = true;
					ok = FindPath(x-1, y, xend, yend, minDistance-1, tableauExemple, w, h);
					if(!ok)
						(tableauExemple)[x][y] = false;
				}
				break;
			default:
				if(x+1 >= w)
					break;
				if(!(tableauExemple)[x+1][y])
				{
					(tableauExemple)[x][y] = true;
					ok = FindPath(x+1, y, xend, yend, minDistance-1, tableauExemple, w, h);
					if(!ok)
						(tableauExemple)[x][y] = false;
				}
				break;
			}
		}
		for(i=0; i< 6; i++)
		{
			delete [] tPas[i];
		}
		delete [] tPas;
		return true;
	}
	else
	{
		for(i=0; i< 6; i++)
		{
			delete [] tPas[i];
		}
		delete [] tPas;
		printf("fin du FindPath\n");
		return false;
	}
}
*/

/*function findPath(x, y, minDistance):
    if (x,y is goal and minDistance == 0) return true
    if (x,y not open) return false
    mark x,y as part of layout path
    switch(random number 1 out of 4):
        case 1: if (findPath(North of x,y, minDistance - 1) == true) return true
        case 2: if (findPath(East of x,y, minDistance - 1) == true) return true
        case 3: if (findPath(South of x,y, minDistance - 1) == true) return true
        case 4: if (findPath(West of x,y, minDistance - 1) == true) return true
    unmark x,y as part of solution path
    return false
*/


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

