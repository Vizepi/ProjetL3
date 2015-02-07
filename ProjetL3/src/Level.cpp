#include "../header/Level.h"

Level::Level()
: m_gravity(b2Vec2(0.f, 15.f*GRAVITY_SCALE))
, m_world(b2World(m_gravity))
, m_rand(new Random())
, m_lastLightAlpha(128)
, m_coinsGet(0)
, m_font(RessourceLoader::GetFont())
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
	std::cout << m_rand->GetSeed() << std::endl;
}

Level::~Level()
{
	delete m_rand;
	delete m_listener;
	while(m_anim.size() > 0)
	{
		delete m_anim[m_anim.size()-1];
		m_anim.pop_back();
	}
}

//Fonction qui crée un objet statique dans box2d.
void Level::CreateStaticObject(float x, float y, float width, float height, int side)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = m_world.CreateBody(&BodyDef);
    Body->SetFixedRotation(true);
    b2PolygonShape Shape;
    b2Vec2 vertices[8];
    //Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
    float percent = 0.8f;
    float halfW = width/(2.f*SCALE);
    float halfH = height/(2.f*SCALE);
    if(side == 0)
	{
		Shape.SetAsBox((width/2.f)/SCALE, (height/2.f)/SCALE);
	}
	else if(side == SIDE_LEFT)
	{
		vertices[0].Set(-halfW, percent * halfH);
		vertices[1].Set(-percent * halfW, halfH);
		vertices[2].Set(halfW, halfH);
		vertices[3].Set(halfW, -halfH);
		vertices[4].Set(-percent * halfW, -halfH);
		vertices[5].Set(-halfW, -percent * halfH);
		Shape.Set(vertices, 6);
	}
	else if(side == SIDE_RIGHT)
	{
		vertices[0].Set(-halfW, halfH);
		vertices[1].Set(percent * halfW, halfH);
		vertices[2].Set(halfW, percent * halfH);
		vertices[3].Set(halfW, -percent * halfH);
		vertices[4].Set(percent * halfW, -halfH);
		vertices[5].Set(-halfW, -halfH);
		Shape.Set(vertices, 6);
	}
	else if(side == (SIDE_LEFT | SIDE_RIGHT))
	{
		vertices[0].Set(-halfW, percent * halfH);
		vertices[1].Set(-percent * halfW, halfH);
		vertices[2].Set(percent * halfW, halfH);
		vertices[3].Set(halfW, percent * halfH);
		vertices[4].Set(halfW, -percent * halfH);
		vertices[5].Set(percent * halfW, -halfH);
		vertices[6].Set(-percent * halfW, -halfH);
		vertices[7].Set(-halfW, -percent * halfH);
		Shape.Set(vertices, 8);
	}
    b2FixtureDef FixtureDef;
    FixtureDef.density = 10.f;
    FixtureDef.shape = &Shape;
    FixtureDef.isSensor = false;
    Body->CreateFixture(&FixtureDef);
}

//Fonction qui crée un sensor dans box2d.
void Level::CreateSensor(float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2((x+2)/SCALE, y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = m_world.CreateBody(&BodyDef);
    Body->SetFixedRotation(true);
    b2PolygonShape Shape;
    Shape.SetAsBox(((width-4)/2.f)/SCALE, (height/2.f)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.isSensor = true;
    FixtureDef.shape = &Shape;
    FixtureDef.density = 10.f;
    Body->CreateFixture(&FixtureDef);
}

//Fonction qui crée un objet dynamique dans box2d.
b2Body* Level::CreateDynamicObject(float x, float y, float width, float height)
{

	// Rectangle de collision
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

	// Detecteur de sauts
	b2FixtureDef fixtureDef;
    b2PolygonShape rectangle;
	rectangle.SetAsBox(((width)/4.f)/SCALE,2/SCALE, b2Vec2(0, (1+height/2.f)/SCALE), 0.f);
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
	float cX = m_character.GetBody()->GetPosition().x * SCALE;
	float cY = m_character.GetBody()->GetPosition().y * SCALE;
	float wX = window.getSize().x;
	float wY = window.getSize().y;
	if(cX < wX/2.f)
		cX = wX/2.f;
	if(cX > m_array.size() * BLOC_SIZE - wX/2.f)
		cX = m_array.size() * BLOC_SIZE - wX/2.f;
	if(cY < wY/2.f)
		cY = wY/2.f;
	if(cY > m_array[0].size() * BLOC_SIZE - wY/2.f)
		cY = m_array[0].size() * BLOC_SIZE - wY/2.f;
	window.setView(sf::View(sf::Vector2f(cX, cY), sf::Vector2f(wX, wY)));
	#endif
	// Get coins
	for(int i=m_coins.size()-1;i>=0;i--)
	{
		//If le docteur est en colision avec une piece
			//On incremente de 1 le m_coinsGet
			//On supprime le coin dans m_coin.erase[i]
		int posx_coin = m_coins[i].x*BLOC_SIZE+16;
		int posy_coin = m_coins[i].y*BLOC_SIZE+16;
		if(m_character.GetSprite()->getGlobalBounds().contains(posx_coin, posy_coin))
		{
				m_coinsGet++;
				m_coins.erase(m_coins.begin()+i);
				printf("Nombres de pieces :%d\n", m_coinsGet);
				printf("Index : %d, reste %d\n", i, m_coins.size());
				break;
		}
	}
    printf("Vie : %d\n", m_character.GetLife());
}

//Remplissage de la fenetre
void Level::Draw(sf::RenderWindow& window)
{
	DrawBackground(window);
	DrawLevelArray(window);
	window.draw(tardis);
	window.draw(emy);
	//window.draw(m_coin);
	window.draw(*m_character.GetSprite());
	int coin_count = m_coins.size();
	for(int i=0; i< coin_count; i++)
	{
		m_coin.setPosition(m_coins[i].x * BLOC_SIZE, m_coins[i].y * BLOC_SIZE);
		window.draw(m_coin);
	}
	// Creation de la lumière
	#ifndef HIDE_LIGHT
	sf::Sprite light(*RessourceLoader::GetTexture("Light"));
	int lightWidth = light.getTexture()->getSize().x;
	int lightHeight = light.getTexture()->getSize().y;
	int winW = window.getView().getSize().x;
	int winH = window.getView().getSize().y;
	int winX = window.getView().getCenter().x - winW/2;
	int winY = window.getView().getCenter().y - winH/2;
	int posChX = m_character.GetSprite()->getPosition().x;
	int posChY = m_character.GetSprite()->getPosition().y;
	int posLightX = posChX - (lightWidth / 2.f);
	int posLightY = posChY - (lightHeight / 2.f);
	int blackWidth0 = posLightX - winX;
	int blackHeight0 = posLightY - winY;
	int blackWidth1 = winX + winW - posLightX - lightWidth;
	int blackHeight1 = winY + winH - posLightY - lightHeight;
	sf::RectangleShape blackRS;
	blackRS.setFillColor(sf::Color::Black);
	if(blackWidth0 > 0)
	{
		blackRS.setSize(sf::Vector2f(blackWidth0+4, winH+4));
		blackRS.setPosition(winX - 2.f, winY - 2.f);
		window.draw(blackRS);
	}
	if(blackWidth1 > 0)
	{
		blackRS.setSize(sf::Vector2f(blackWidth1+4, winH+4));
		blackRS.setPosition(posLightX + lightWidth - 2.f, winY - 2.f);
		window.draw(blackRS);
	}
	if(blackHeight0 > 0)
	{
		blackRS.setSize(sf::Vector2f(winW+4, blackHeight0+4));
		blackRS.setPosition(winX - 2.f, winY - 2.f);
		window.draw(blackRS);
	}
	if(blackHeight1 > 0)
	{
		blackRS.setSize(sf::Vector2f(winW+4, blackHeight1+4));
		blackRS.setPosition(winX - 2.f, posLightY + lightHeight - 2.f);
		window.draw(blackRS);
	}
	light.setPosition(posLightX, posLightY);
	window.draw(light);
	m_lastLightAlpha += m_rand->NextInt(0, 50)-25;
	if(m_lastLightAlpha < 0)
		m_lastLightAlpha = 0;
	if(m_lastLightAlpha > 255)
		m_lastLightAlpha = 255;
	light.setColor(sf::Color(255, 255, 255, m_lastLightAlpha));
	window.draw(light);
	#endif
	DrawHUB(window.getView().getCenter().x - (window.getView().getSize().x)/2,
			window.getView().getCenter().y - (window.getView().getSize().y)/2,
			window.getView().getSize().x,
			window.getView().getSize().y,
			window);
}

void Level::DrawHUB(int winX, int winY, int winW, int winH, sf::RenderWindow& window)
{
	sf::Text text_coins;
	text_coins.setFont(*m_font);
	std::stringstream strs;
	strs << "COINS : " << m_coinsGet << " / " << NB_COINS;
	std::string text(strs.str());
	text_coins.setString(text);
	text_coins.setPosition(winX+10,winY+10);
	window.draw(text_coins);
	if(gagne == true)
	{
		sf::Text text_final;
		text_final.setFont(*m_font);
		text_final.setString("YOU WIN !");
		text_final.setPosition(winX+(winW/2)-(text_final.getGlobalBounds().width/2), winY+(winH/2)-(text_final.getGlobalBounds().height/2));
		window.draw(text_final);
	}
}

Character* Level::GetCharacter()
{
	return &m_character;
}

//Fonction de generation aléatoire d'un level.
void Level::GenerateLevel()
{
	int i, j;

	Room** tableauExemple = new Room*[LEVEL_WIDTH];
	for(i = 0; i<LEVEL_WIDTH; i++)
	{
			tableauExemple[i] = new Room[LEVEL_HEIGHT];
	}
	for(i = 0; i<LEVEL_WIDTH; i++)
	{
		for(j = 0; j<LEVEL_HEIGHT; j++)
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

	// Récupération des dimension du niveau
	int arrayWidth = LEVEL_WIDTH * ROOM_WIDTH;
	int arrayHeight = LEVEL_HEIGHT * ROOM_HEIGHT;

	m_array.reserve(arrayWidth);
	for(i=0;i<arrayWidth;i++)
	{
		m_array.push_back(std::vector<int>());
		//Reservation de 15 en largeur.
		m_array[i].reserve(arrayHeight);
		for(j=0;j<arrayHeight;j++)
		{
			m_array[i].push_back(lt_empty);
		}
	}

	if(m_array.size() == 0)
		return;

	int rand_x = m_rand->NextInt(0, LEVEL_WIDTH-1);
	int rand_y = m_rand->NextInt(0, LEVEL_HEIGHT-1);
	int rand_xend = m_rand->NextInt(0, LEVEL_WIDTH-1);
	int rand_yend = m_rand->NextInt(0, LEVEL_HEIGHT-1);
	while(rand_x == rand_xend && rand_y == rand_yend)
	{
		rand_xend = m_rand->NextInt(0, LEVEL_WIDTH-1);
		rand_yend = m_rand->NextInt(0, LEVEL_HEIGHT-1);
	}
	printf("x de depart :%d\ny de depart:%d\n", rand_x, rand_y);
	printf("x d'arrive :%d\ny d'arrive:%d\n", rand_xend, rand_yend);

	FindPath(rand_x, rand_y, rand_xend, rand_yend, MINIM_DISTANCE, (Room**)(tableauExemple), arrayWidth, arrayHeight, pile);
	Room* r_end = new Room();
	pile.push(r_end);
	pile.top()->x = rand_xend;
	pile.top()->y = rand_yend;
	pile.top()->North = false;
	pile.top()->South = false;
	pile.top()->East = false;
	pile.top()->West = false;
	//Création d'une deque
	deque<Room*> dq;
	printf("Pile :\n");
	while(!pile.empty())
	{
		dq.push_front(pile.top());
		pile.pop();
	}
	printf("Deque :\n");
	SetRoom(dq);
	for (i=0; i<(signed)dq.size(); i++)
	{
		//On choisit un point au hasard dans chaque Room
		dq[i]->rand_x = m_rand->NextInt(1, ROOM_WIDTH-2);
		dq[i]->rand_y = m_rand->NextInt(2, ROOM_HEIGHT-1);
		if(i != 0)
		{
			while(dq[i]->rand_x == dq[i-1]->rand_x ||
					dq[i]->rand_x == dq[i-1]->rand_x+1 ||
					dq[i]->rand_x == dq[i-1]->rand_x-1)
			{
				dq[i]->rand_x = m_rand->NextInt(1, ROOM_WIDTH-2);
			}
			while(dq[i]->rand_y == dq[i-1]->rand_y ||
					dq[i]->rand_y == dq[i-1]->rand_y+1 ||
					dq[i]->rand_y == dq[i-1]->rand_y-1)
			{
				dq[i]->rand_y = m_rand->NextInt(2, ROOM_HEIGHT-1);
			}
		}
	}

	CreateLevel(tableauExemple, dq);

	tardis.setPosition(sf::Vector2f((ROOM_WIDTH * dq[0]->x + dq[0]->rand_x-0.25) * BLOC_SIZE,
						(ROOM_HEIGHT * dq[0]->y + dq[0]->rand_y - 1.5) * BLOC_SIZE));
	int x = ((ROOM_WIDTH * dq[dq.size()-1]->x + dq[dq.size()-1]->rand_x) * BLOC_SIZE)-(3*BLOC_SIZE)+(m_rand->NextInt(0, 6)* BLOC_SIZE);
	int y = (ROOM_HEIGHT * dq[dq.size()-1]->y + dq[dq.size()-1]->rand_y) * BLOC_SIZE - CHARACTER_HEIGHT - 8;
	emy.setPosition(sf::Vector2f(x,y));

	//Pour eviter qu'Amy soit dans le mur
	while(m_array[emy.getPosition().x/BLOC_SIZE][emy.getPosition().y/BLOC_SIZE] == lt_solid)
	{
		x = ((ROOM_WIDTH * dq[dq.size()-1]->x + dq[dq.size()-1]->rand_x) * BLOC_SIZE)-(3*BLOC_SIZE)+(m_rand->NextInt(0, 6)* BLOC_SIZE);
		y = (ROOM_HEIGHT * dq[dq.size()-1]->y + dq[dq.size()-1]->rand_y) * BLOC_SIZE - CHARACTER_HEIGHT - 8;
		emy.setPosition(sf::Vector2f(x,y));
	}
	m_character.GetBody()->SetTransform(m_startPosition, m_character.GetBody()->GetAngle());

	//Pour supprimer le tableau
	while(dq.size() > 0)
	{
		delete dq.front();
		dq.pop_front();
	}
	for(i=0; i< LEVEL_WIDTH; i++)
	{
		delete [] tableauExemple[i];
	}
	delete [] tableauExemple;

	for(i=0;i<arrayWidth;i++)
	{
		m_array[i][0] = lt_solid;
		m_array[i][arrayHeight-1] = lt_solid;
	}
	for(i=0;i<arrayHeight;i++)
	{
		m_array[0][i] = lt_solid;
		m_array[arrayWidth-1][i] = lt_solid;
	}

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
				if((y+1)>LEVEL_HEIGHT-1 || south)
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
				if((x+1)>LEVEL_WIDTH-1 || east)
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
	int width = ROOM_WIDTH*LEVEL_WIDTH;
	int height = ROOM_HEIGHT*LEVEL_HEIGHT;
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
	// Creation des murs des rooms
	CreateWalls(dq);

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
			{
				if(m_array[posX][posY] != lt_empty)
					m_array[posX][posY] = lt_cross;
				else
					m_array[posX][posY] = lt_ladder;
			}
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
				if(xf >= (int)m_array.size())
					break;
				m_array[xf][p2y] = lt_ground;
			}
			for(int xf=p2x-1; xf>(p2x-4); xf--)
			{
				if(xf < 0)
					break;
				m_array[xf][p2y] = lt_ground;
			}
		}
	}
	CreateGenerateLevel(dq);
	//Mettre les echelles tombantes sur chaques platformes du chemin gagnant
	for(int i = 0; i< (signed)dq.size()-1; i++)
	{
		int p1x = ROOM_WIDTH * dq[i]->x + dq[i]->rand_x;
		int p2x = ROOM_WIDTH * dq[i+1]->x + dq[i+1]->rand_x;
		int y = ROOM_HEIGHT * dq[i]->y + dq[i]->rand_y;
		int rand;
		if(p1x < p2x)
			rand = m_rand->NextInt(p1x, p2x);
		else
			rand = m_rand->NextInt(p2x, p1x);
		if(y >= (int)m_array[0].size()-1)
			continue;
		if(m_array[rand][y+1] != lt_empty)
			continue;
		m_array[rand][y]= lt_cross;
		y++;
		while(m_array[rand][y] == lt_empty)
		{
			m_array[rand][y]= lt_ladder;
			y++;
		}
	}
}

void Level::CreateWalls(deque<Room*> &dq)
{
	for (int i=0; i < (signed)dq.size(); i++)
	{
		int roomX = dq[i]->x * ROOM_WIDTH;
		int roomY = dq[i]->y * ROOM_HEIGHT;
		if(!dq[i]->North)
		{
			for(int j=0;j<ROOM_WIDTH;j++)
			{
				m_array[roomX+j][roomY] = lt_solid;
			}
		}
		if(!dq[i]->East)
		{
			for(int j=0;j<ROOM_HEIGHT;j++)
			{
				m_array[roomX+ROOM_WIDTH-1][roomY+j] = lt_solid;
			}
		}
		if(!dq[i]->South)
		{
			for(int j=0;j<ROOM_WIDTH;j++)
			{
				m_array[roomX+j][roomY+ROOM_HEIGHT-1] = lt_solid;
			}
		}
		if(!dq[i]->West)
		{
			for(int j=0;j<ROOM_HEIGHT;j++)
			{
				m_array[roomX][roomY+j] = lt_solid;
			}
		}
	}
}

void Level::CreateGenerateLevel(deque<Room*> &dq)
{

	//nombre de platforme genere aleatoirement par bloc
	int nbPlatforme = (int)(ROOM_WIDTH*ROOM_HEIGHT / 2)/(ROOM_WIDTH+ROOM_HEIGHT);
	int dqCount = dq.size();

	std::vector<int> globalCrossX;
	std::vector<int> globalCrossY;

	for(int i=0;i<dqCount;i++)
	{
		int roomOriginX = dq[i]->x * ROOM_WIDTH;
		int roomOriginY = dq[i]->y * ROOM_HEIGHT + 1;
		int roomSizeX = ROOM_WIDTH;
		int roomSizeY = ROOM_HEIGHT - 2;
		int offsetX = 0, offsetY = 0;
		std::vector<int> crossX;
		std::vector<int> crossY;
		crossX.push_back(dq[i]->rand_x);
		crossY.push_back(dq[i]->rand_y);
		if(!dq[i]->North)
		{
			offsetX++;
			roomOriginY++;
			roomSizeY--;
		}
		if(!dq[i]->East)
		{
			roomSizeX--;
		}
		if(!dq[i]->South)
		{
			roomSizeY--;
		}
		if(!dq[i]->West)
		{
			offsetX++;
			roomOriginX++;
			roomSizeX--;
		}

		for(int j=0;j<nbPlatforme;j++)
		{
			int generateX = 0, generateY = 0;
			bool valid = true;
			do
			{
				valid = true;
				generateX = m_rand->NextInt(offsetX, roomSizeX-1);
				generateY = m_rand->NextInt(offsetY, roomSizeY-1);
				for(int k=crossX.size()-1;k>=0;k--)
				{
					if(	generateX + 1 == crossX[k] || generateX - 1 == crossX[k] ||
						generateY + 1 == crossY[k] || generateY - 1 == crossY[k])
					{
						valid = false;
						break;
					}
				}
				if(valid)
				{
					int realX = generateX + roomOriginX;
					int realY = generateY + roomOriginY;
					if(	SAME_LEVELTYPE(m_array[realX][realY], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX][realY+1], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX][realY-1], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX+1][realY], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX-1][realY], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX+1][realY+1], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX-1][realY-1], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX-1][realY+1], lt_ground) ||
						SAME_LEVELTYPE(m_array[realX+1][realY-1], lt_ground))
					{
						valid = false;
					}
				}
			} while(!valid);

			m_array[generateX + roomOriginX][generateY + roomOriginY] = lt_cross;
			crossX.push_back(generateX);
			crossY.push_back(generateY);
			globalCrossX.push_back(generateX + roomOriginX);
			globalCrossY.push_back(generateY + roomOriginY);

			// Agrandir la platforme
			int leftSpace = generateX;
			int rightSpace = ROOM_WIDTH - generateX - 1;
			int leftRand = m_rand->NextInt(1, leftSpace);
			int rightRand = m_rand->NextInt(1, rightSpace);
			for(int l=0;l<leftRand;l++)
			{
				if(	SAME_LEVELTYPE(m_array[generateX + roomOriginX - l+1][generateY + roomOriginY+1], lt_ground) ||
					SAME_LEVELTYPE(m_array[generateX + roomOriginX - l-1][generateY + roomOriginY-1], lt_ground) ||
					SAME_LEVELTYPE(m_array[generateX + roomOriginX - l-1][generateY + roomOriginY+1], lt_ground) ||
					SAME_LEVELTYPE(m_array[generateX + roomOriginX - l+1][generateY + roomOriginY-1], lt_ground))
				{
					break;
				}
				if(	m_array[generateX + roomOriginX - l][generateY + roomOriginY] == lt_ladder ||
					m_array[generateX + roomOriginX - l][generateY + roomOriginY] == lt_cross)
					m_array[generateX + roomOriginX - l][generateY + roomOriginY] = lt_cross;
				else
					m_array[generateX + roomOriginX - l][generateY + roomOriginY] = lt_ground;
			}
			for(int l=0;l<rightRand;l++)
			{
				if(	SAME_LEVELTYPE(m_array[generateX + roomOriginX + l+1][generateY + roomOriginY+1], lt_ground) ||
					SAME_LEVELTYPE(m_array[generateX + roomOriginX + l-1][generateY + roomOriginY-1], lt_ground) ||
					SAME_LEVELTYPE(m_array[generateX + roomOriginX + l-1][generateY + roomOriginY+1], lt_ground) ||
					SAME_LEVELTYPE(m_array[generateX + roomOriginX + l+1][generateY + roomOriginY-1], lt_ground))
				{
					break;
				}
				if(	m_array[generateX + roomOriginX + l][generateY + roomOriginY] == lt_ladder ||
					m_array[generateX + roomOriginX + l][generateY + roomOriginY] == lt_cross)
					m_array[generateX + roomOriginX + l][generateY + roomOriginY] = lt_cross;
				else
					m_array[generateX + roomOriginX + l][generateY + roomOriginY] = lt_ground;
			}
		}
	}

	// Generation des echelles
	for(int i = globalCrossX.size()-1;i>=0;i--)
	{
		int cX = globalCrossX[i];
		int cY = globalCrossY[i];
		if(cY+1 < (int)m_array[0].size() && (m_array[cX][cY+1] != lt_ground && m_array[cX][cY+1] != lt_solid))
			m_array[cX][cY] = lt_cross;
		cY++;
		while(m_array[cX][cY] == lt_empty)
		{
			m_array[cX][cY] = lt_ladder;
			cY++;
		}
	}

	//Appel de la fonction pour mettre les pieces
	PutCoin();
}

void Level::PutCoin()
{
	for(int i =0; i< NB_COINS; i++)
	{
		int rand_x = m_rand->NextInt(0, ROOM_WIDTH*LEVEL_WIDTH-1);
		int rand_y = m_rand->NextInt(0,ROOM_HEIGHT*LEVEL_HEIGHT-1);
		while(m_array[rand_x][rand_y] != lt_empty)
		{
			rand_x = m_rand->NextInt(0, ROOM_WIDTH*LEVEL_WIDTH-1);
			rand_y = m_rand->NextInt(0,ROOM_HEIGHT*LEVEL_HEIGHT-1);
		}
		int cY = rand_y;
		while(m_array[rand_x][cY+1] == lt_empty)
		{
			cY++;
		}
		m_coins.push_back(sf::Vector2i(rand_x, cY));

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
	#ifdef LITTLE_BLOCS_PHYSIC
	int side = 0;
	for(int i =0; i < arrayWidth; i++)
	{
		for(int j =0; j<arrayHeight; j++)
		{
			side = 0;
			switch(m_array[i][j])
			{
			case lt_cross :
			case lt_ladder :
				CreateSensor(i*BLOC_SIZE, j*BLOC_SIZE, BLOC_SIZE, BLOC_SIZE);
				break;
			case lt_ground :
			case lt_solid :
				if(i > 0 && m_array[i-1][j] != lt_empty)
				{
					if(j > 0 && m_array[i-1][j-1] != lt_ladder)
						side = side | SIDE_LEFT;
				}
				if(i < arrayWidth-1 && m_array[i+1][j] != lt_empty)
				{
					if(j > 0 && m_array[i+1][j-1] != lt_ladder)
						side = side | SIDE_RIGHT;
				}
				CreateStaticObject(i*BLOC_SIZE, j*BLOC_SIZE, BLOC_SIZE, BLOC_SIZE, side);
				break;
			default:
				break;

			}

		}
	}

	m_character.GetBody()->SetTransform(m_startPosition, m_character.GetBody()->GetAngle());
	m_listener->SetPos(m_character.GetBody()->GetPosition().y * SCALE);
	return;
	#endif

	bool mark[mul];
	for(int i=0;i<mul;i++)
		mark[i] = false; // Initialisation du tableau de booléens
    int width = 0;
    int height = 0;
    float x, y;
    int prev[3] = {0, 0, 0}; // {type, x, y}
    int state;
	for(int j=0;j<arrayHeight;j++)
	{
		for(int i=0;i<arrayWidth;i++)
		{
			assert(i * arrayHeight + j < mul);
			if(!mark[i * arrayHeight + j]) // Si on n'est pas encore passé sur le bloc
			{
				if(m_array[i][j] == lt_empty)//Si le bloc est vide on continue
				{
					mark[i * arrayHeight + j] = true;
					continue;
				}
				state = 0;
				int currentX = i, currentY = j;
				while(state != 3)
				{
					if(state == 0) // Debut de rectangle
					{
						mark[currentX * arrayHeight + currentY] = true;
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
								mark[currentX * arrayHeight + currentY] = true;
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
								mark[k * arrayHeight + currentY] = true;
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
					x = prev[1];
					y = prev[2];
					std::cout << "static : " << x << " " << y << " " << width << " " << height << std::endl;
					CreateStaticObject(x * BLOC_SIZE, y * BLOC_SIZE, width * BLOC_SIZE, height * BLOC_SIZE);
					break;
				case lt_ladder:
				case lt_cross:
					x = prev[1];
					y = prev[2];
					std::cout << "sensor : " << x << " " << y << " " << width << " " << height << std::endl;
					CreateSensor(x * BLOC_SIZE, y * BLOC_SIZE, width * BLOC_SIZE, height * BLOC_SIZE);
					break;
				default:
					break;
				}
			}
		}
	}
	m_character.GetBody()->SetTransform(m_startPosition, m_character.GetBody()->GetAngle());
	m_listener->SetPos(m_character.GetBody()->GetPosition().y * SCALE);
}

void Level::DrawBackground(sf::RenderWindow& window)
{
	float coef = 2.f;
	sf::Sprite backgroundTexture(*RessourceLoader::GetTexture("Background"));
	int px = m_character.GetSprite()->getPosition().x / coef;
	int py = m_character.GetSprite()->getPosition().y / coef;
	int texX = backgroundTexture.getTexture()->getSize().x;
	int texY = backgroundTexture.getTexture()->getSize().y;
	while(std::abs(m_character.GetSprite()->getPosition().x - px) > texX)
		px += texX;
	while(std::abs(m_character.GetSprite()->getPosition().y - py) > texY)
		py += texY;
	// Dessine le font du niveau.
	backgroundTexture.setPosition(px, py);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px-texX, py);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px, py-texY);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px-texX, py-texY);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px+texX, py-texY);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px-texX, py+texY);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px+texX, py);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px, py+texY);
	window.draw(backgroundTexture);
	backgroundTexture.setPosition(px+texX, py+texY);
	window.draw(backgroundTexture);
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
	m_brokenLadderRandom = new Random(68435);
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
			if(m_array[i][j] == lt_cross)
			{
				sprite.setTextureRect(sf::IntRect(RS_POS(2), RS_POS(7), RS_BLOC_SIZE, RS_BLOC_SIZE));
				window.draw(sprite);
			}
		}
	}


	delete m_brokenLadderRandom;
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
		else
			rs.setFillColor(sf::Color(0, 255, 0));
		window.draw(rs);
		bodyIterator = bodyIterator->GetNext();
	}
	#endif
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
		if(m_brokenLadderRandom->NextInt(0, 7) == 0)
		{
			if(m_brokenLadderRandom->NextBool())
				*x = 1;
			else
				*x = 3;
		}
		return;
	case lt_cross:
		GetTextureCoords(x, y, lt_solid, north, east, south, west);
		break;
	default:
		*x = 0;
		*y = 0;
		break;
	}
}

void Level::Dump()
{
	int width = m_array.size();
	int height = m_array[0].size();
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			std::cout << m_array[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

