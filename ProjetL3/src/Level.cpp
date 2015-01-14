#include "../header/Level.h"

Level::Level()
: m_gravity(b2Vec2(0.f, 15.f*GRAVITY_SCALE))
, m_world(b2World(m_gravity))
#ifdef SINGLE_BODY_LEVEL
, m_levelBody(NULL)
#endif
, m_rand(new Random())
{
	gagne = false;
	m_listener = new JumpListener(&m_character);
	m_world.SetContactListener(m_listener);
	tardis.setTexture(*RessourceLoader::GetTexture("Tardis"));
	tardis.setScale(0.75f, 0.75f);
	emy.setTexture(*RessourceLoader::GetTexture("Amy Pond"));
	emy.setTextureRect(sf::IntRect(0, 0, 32, 48));

	m_coin.setTexture(*RessourceLoader::GetTexture("Coins"));
	m_anim.push_back(new Animation(&m_coin, sf::IntRect(0, 0, 32, 32), 8, 150));

	m_currentAnimation = m_anim[LOOK_DOWN];
	m_currentAnimation->Play();

	#ifdef SINGLE_BODY_LEVEL
	// Creation du corps physique du niveau
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(0.f, 0.f);
	bodyDef.type = b2_staticBody;
	m_levelBody = m_world.CreateBody(&bodyDef);
	#endif
}

Level::~Level()
{
	delete m_rand;
}

//Fonction qui crée un objet statique dans box2d.
void Level::CreateStaticObject(float x, float y, float width, float height)
{
	#ifndef SINGLE_BODY_LEVEL
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
    #else
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.shape = &Shape;
    FixtureDef.isSensor = false;
    m_levelBody->CreateFixture(&FixtureDef);
    #endif
}

//Fonction qui crée un sensor dans box2d.
void Level::CreateSensor(float x, float y, float width, float height)
{
	#ifndef SINGLE_BODY_LEVEL
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
    #else
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.isSensor = true;
    FixtureDef.shape = &Shape;
    FixtureDef.density = 1.f;
    m_levelBody->CreateFixture(&FixtureDef);
    #endif
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
	m_currentAnimation->Update();
	//Test si la partie est gagnée
	int posx_d = GetCharacter()->GetSprite()->getPosition().x;
	int posy_d = GetCharacter()->GetSprite()->getPosition().y;
	int posx_a = emy.getPosition().x;
	int posy_a= emy.getPosition().y;
	if((posy_d >= posy_a-5 && posy_d <=  posy_a+5) && (posx_d >= posx_a-80 && posx_d <= posx_a+80 ))
	{
			gagne = true;
	}


	#ifndef SHOW_ALL_MAP
	window.setView(sf::View(sf::Vector2f(m_character.GetBody()->GetPosition().x * SCALE, m_character.GetBody()->GetPosition().y * SCALE),
					sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)));
	#endif
}

//Remplissage de la fenetre
void Level::Draw(sf::RenderWindow& window)
{
	DrawLevelArray(window);
	window.draw(tardis);
	window.draw(emy);
	window.draw(m_coin);
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

	int rand_x = m_rand->NextInt(0, 5);
	int rand_y = m_rand->NextInt(0, 5);
	int rand_xend = m_rand->NextInt(0, 5);
	int rand_yend = m_rand->NextInt(0, 5);
	while(rand_x == rand_xend && rand_y == rand_yend)
	{
		rand_xend = m_rand->NextInt(0, 5);
		rand_yend = m_rand->NextInt(0, 5);
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
		//On choisit un point au hasard dans chaque Room
		dq[i]->rand_x = m_rand->NextInt(0, ROOM_WIDTH-2)+1;
		dq[i]->rand_y = m_rand->NextInt(0, ROOM_HEIGHT-2)+1;
		if(i != 0)
		{
			while(dq[i]->rand_x == dq[i-1]->rand_x ||
					dq[i]->rand_x == dq[i-1]->rand_x+1 ||
					dq[i]->rand_x == dq[i-1]->rand_x-1)
			{
				dq[i]->rand_x = m_rand->NextInt(0, ROOM_WIDTH-2)+1;
			}
			while(dq[i]->rand_y == dq[i-1]->rand_y ||
					dq[i]->rand_y == dq[i-1]->rand_y+1 ||
					dq[i]->rand_y == dq[i-1]->rand_y-1)
			{
				dq[i]->rand_y = m_rand->NextInt(0, ROOM_HEIGHT-2)+1;
			}
		}
	}

	CreateLevel(tableauExemple, dq);

	tardis.setPosition(sf::Vector2f((ROOM_WIDTH * dq[0]->x + dq[0]->rand_x-0.25) * BLOC_SIZE,
						(ROOM_HEIGHT * dq[0]->y + dq[0]->rand_y - 1.5) * BLOC_SIZE));
	m_coin.setPosition(sf::Vector2f(((ROOM_WIDTH * dq[dq.size()-1]->x + dq[dq.size()-1]->rand_x) * BLOC_SIZE)-(3*BLOC_SIZE)+(m_rand->NextInt(0, 6)* BLOC_SIZE),
						(ROOM_HEIGHT * dq[dq.size()-1]->y + dq[dq.size()-1]->rand_y) * BLOC_SIZE - CHARACTER_HEIGHT));
	//Pour supprimer le tableau
	while(dq.size() > 0)
	{
		delete dq.front();
		dq.pop_front();
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
		if(dq[i]->x < dq[i+1]->x)
		{
			dq[i]->East = true;
			dq[i+1]->West = true;
		}
		else if(dq[i]->x > dq[i+1]->x)
		{
			dq[i]->West = true;
			dq[i+1]->East = true;
		}
		else if(dq[i]->y > dq[i+1]->y)
		{
			dq[i]->North = true;
			dq[i+1]->South = true;
		}
		else if(dq[i]->y < dq[i+1]->y)
		{
			dq[i]->South = true;
			dq[i+1]->North = true;
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
	int r = m_rand->NextInt(0, 3);
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

//Création du level
void Level::CreateLevel(Room** t, deque<Room*> &dq)
{
	int width = ROOM_WIDTH*6;
	int height = ROOM_HEIGHT*6;
	for(int i=m_array.size()-1;i>=0;i--)
		m_array[i].clear();
	m_array.clear();
	//initialisation
	m_array.reserve(width);
	for(int i=0;i<width;i++)
	{
		m_array.push_back(std::vector<int>());

		m_array[i].reserve(height);
	}
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			m_array[i].push_back(lt_solid);
		}
	}
	// Remplissage des rooms de la deque en lt_empty.
	for(int i = 0; i< (signed)dq.size(); i++)
	{
		// Position de la room suivante en longueur.
		int endj = ROOM_WIDTH*(dq[i]->x+1);

		for(int j = ROOM_WIDTH*dq[i]->x; j< endj; j++)
		{
			// Position de la room suivante en hauteur.
			int endk = ROOM_HEIGHT*(dq[i]->y+1);

			for(int k=  ROOM_HEIGHT*dq[i]->y; k< endk; k++)
			{
				m_array[j][k] = lt_empty;
			}
		}
	}

	for(int i = 0; i< (signed)dq.size()-1; i++)
	{

		int p1x = ROOM_WIDTH * dq[i]->x + dq[i]->rand_x;
		int p1y = ROOM_HEIGHT * dq[i]->y + dq[i]->rand_y;
		int p2x = ROOM_WIDTH * dq[i+1]->x + dq[i+1]->rand_x;
		int p2y = ROOM_HEIGHT * dq[i+1]->y + dq[i+1]->rand_y;

		if(i == 0)
			m_startPosition.Set(p1x * BLOC_SIZE / SCALE, (p1y - 1 ) * BLOC_SIZE / SCALE);

		int it = 1;
		if(p1x > p2x)
		{
			it = -1;
		}
		int posX = p1x;
		while(posX != p2x)
		{
			//placeBloc(posX, p1y);
			if(i != 0 && posX == p1x && p1y < ROOM_HEIGHT * dq[i-1]->y + dq[i-1]->rand_y)
				m_array[posX][p1y] = lt_cross;
			else
				m_array[posX][p1y] = lt_ground;
			posX += it;
		}
		it = 1;
		if(p1y > p2y)
		{
			it = -1;
		}
		int posY = p1y;
		while(posY != p2y)
		{
			//placeBloc(posX, posY);
			if(it == 1 && posY == p1y)
				m_array[posX][posY] = lt_cross;
			else if (it == -1 && posY == p2y)
				m_array[posX][posY] = lt_cross;
			else if(it == -1 && posY == p1y )
				m_array[posX][posY] = lt_ground;
			else
				m_array[posX][posY] = lt_ladder;
			posY += it;
		}
		//Faire la plateforme de fin
		if(i == (signed)dq.size()-2)
		{
			if(p2y< p1y)
				m_array[p2x][p2y] = lt_cross;
			else
				m_array[p2x][p2y]= lt_ground;
			for(int xf=p2x+1; xf<(p2x+4); xf++)
			{
				m_array[xf][p2y] = lt_ground;
			}
			for(int xf=p2x-1; xf>(p2x-4); xf--)
			{
				m_array[xf][p2y] = lt_ground;
			}
		}


		/*
		//Si le point du room2 est superieur au point du room 1
		if(dq[i]->rand_x+(ROOM_WIDTH*dq[i]->x) < dq[i+1]->rand_x+(ROOM_WIDTH*dq[i+1]->x))
		{
			int posY = dq[i]->rand_y+(ROOM_HEIGHT*dq[i]->y);
			//On remplit de solide
			for(int x =(dq[i]->rand_x+(ROOM_WIDTH*dq[i]->x)); x < ROOM_WIDTH*(dq[i+1]->x);x++)
			{
				m_array[x][posY] = lt_solid;
			}
		}
		//sinon (si le point du room2 est inferieur au point du room 1
		else
		{
			int posY = dq[i]->rand_y+(ROOM_HEIGHT*dq[i]->y);
			//On remplit de solide
			for(int x = (ROOM_WIDTH*(dq[i+1]->x)); x > (ROOM_WIDTH*(dq[i+1]->x)) ;x--)
			{
				m_array[x][posY] = lt_solid;
			}
		}
		//Si le point en y de room 1 est inferieur au point en y du room 2, alors il faut descendre)
		if(dq[i]->rand_y+(ROOM_HEIGHT*dq[i]->y) < dq[i+1]->rand_y+(ROOM_HEIGHT*dq[i+1]->y))
		{
			//On ajoute le bloc intersection
			m_array[ROOM_WIDTH*(dq[i+1]->x)][ROOM_HEIGHT*dq[i]->y] = lt_cross;
			//On ajoute les echelles
			int ech = ROOM_HEIGHT*dq[i]->y+1 + dq[i]->rand_y;
			int posX = ROOM_WIDTH*(dq[i+1]->x) + dq[i+1]->rand_x;

			while(ech < ROOM_HEIGHT*dq[i+1]->y)
			{
				m_array[posX][ech] = lt_ladder;
				ech = ech+1;
			}
			ech= ech+1;
			m_array[ROOM_WIDTH*(dq[i+1]->x)][ech] = lt_solid;
		}
		//Sinon (si le point y de room1 est superieur que le point y du room 2, alors on monte)
		else
		{
			m_array[ROOM_WIDTH*(dq[i+1]->x)][ROOM_HEIGHT*dq[i]->y] = lt_solid;
			//On ajoute les echelles
			int ech = ROOM_HEIGHT*dq[i]->y-1 + dq[i]->rand_y;
			int posX = ROOM_WIDTH*(dq[i+1]->x) + dq[i+1]->rand_x;

			while(ech > ROOM_HEIGHT*dq[i+1]->y)
			{
				m_array[posX][ech] = lt_ladder;
				ech = ech-1;
			}
			ech= ech-1;
			m_array[ROOM_WIDTH*(dq[i+1]->x)][ech] = lt_cross;
		}*/
	}

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
	for(int i =0; i < arrayWidth; i++)
	{
		for(int j =0; j<arrayHeight; j++)
		{
			switch(m_array[i][j])
			{
			case lt_cross :
			case lt_ladder :
				CreateSensor(i*BLOC_SIZE, j*BLOC_SIZE, BLOC_SIZE, BLOC_SIZE);
				break;
			case lt_ground :
			case lt_solid :
				CreateStaticObject(i*BLOC_SIZE, j*BLOC_SIZE, BLOC_SIZE, BLOC_SIZE);
				break;
			default:
				break;

			}

		}
	}

	m_character.GetBody()->SetTransform(m_startPosition, m_character.GetBody()->GetAngle());
	return;

	std::vector<bool> mark;
	for(int i=0;i<mul;i++)
		mark.push_back(false); // Initialisation du tableau de booléens
    int width = 0;
    int height = 0;
    int x, y;
    int prev[3] = {0, 0, 0}; // {type, x, y}
    int state;
	/*for(int i=0;i<arrayHeight;i++)
	{
		for(int j=0;j<arrayWidth;j++)
		{
			if(mark[j*arrayWidth + i])
				std::cout << "1 ";
			else
				std::cout << "0 ";
		}
		std::cout << std::endl;
	}*/
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
					//std::cout << "static " << prev[1] << " " << prev[2] << " " << width << " " << height << std::endl;
					x = prev[1];
					y = prev[2];
					CreateStaticObject(x * BLOC_SIZE, y * BLOC_SIZE, width * BLOC_SIZE, height * BLOC_SIZE);
					/*for(int i=0;i<arrayHeight;i++)
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
					std::cout << "ok" << std::endl;*/
					break;
				case lt_ladder:
				case lt_cross:
					x = prev[1];
					y = prev[2];
					//std::cout << "sensor " << prev[1] << " " << prev[2] << " " << width << " " << height << std::endl;
					CreateSensor(x * BLOC_SIZE, y * BLOC_SIZE, width * BLOC_SIZE, height * BLOC_SIZE);
					/*for(int i=0;i<arrayHeight;i++)
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
					std::cout << "ok" << std::endl;*/
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
	#ifndef SHOW_COLLISION_BOXES
	int arrayWidth = m_array.size();
	int arrayHeight = m_array[0].size();
	sf::Sprite sprite;
	sprite.setTexture(*RessourceLoader::GetTexture("Skin01"));
	sprite.setScale(BLOC_SIZE/RS_BLOC_SIZE, BLOC_SIZE/RS_BLOC_SIZE);
	for(int i=0;i<arrayWidth;i++)
	{
		for(int j=0;j<arrayHeight;j++)
		{
			switch(m_array[i][j])
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
			}
		}
	}
	#else
	b2Body* bodyIterator = m_world.GetBodyList();
	while(bodyIterator)
	{
		sf::RectangleShape rs;
		int width = bodyIterator->GetFixtureList()->GetAABB(0).upperBound.x - bodyIterator->GetFixtureList()->GetAABB(0).lowerBound.x;
		int height = bodyIterator->GetFixtureList()->GetAABB(0).upperBound.y - bodyIterator->GetFixtureList()->GetAABB(0).lowerBound.y;
		rs.setSize(sf::Vector2f(width * SCALE, height * SCALE));
		rs.setPosition(bodyIterator->GetPosition().x * SCALE, bodyIterator->GetPosition().y * SCALE);
		rs.setFillColor(sf::Color(128, 128, 128));
		if(bodyIterator->GetFixtureList()->IsSensor())
			rs.setFillColor(sf::Color(0, 0, 255));
		else if(bodyIterator->GetType() == b2_staticBody)
			rs.setFillColor(sf::Color(255, 0, 0));
		else if(bodyIterator->GetType() == b2_dynamicBody)
			rs.setFillColor(sf::Color(0, 255, 0));
		window.draw(rs);
		bodyIterator = bodyIterator->GetNext();
	}
	#endif
}

