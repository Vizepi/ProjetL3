#include "../header/Level.h"
#define SCALE 30.f
#define GRAVITY_SCALE 3.f

Level::Level() : gravity(b2Vec2(0.f, 9.81f*GRAVITY_SCALE)), world(b2World(gravity))
{

}

void Level::CreateStaticObject(b2World& world, float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = world.CreateBody(&BodyDef);
    b2PolygonShape Shape;
    Shape.SetAsBox((width/2)/SCALE, (height/2)/SCALE); // Creates a box shape. Divide your desired width and height by 2.
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;  // Sets the density of the body
    FixtureDef.shape = &Shape; // Sets the shape
    Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}
b2Body* Level::CreateDynamicObject(b2World& world, float x, float y, float width, float height)
{
	b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/SCALE, y/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = world.CreateBody(&BodyDef);
    Body->SetFixedRotation(true);

    b2PolygonShape Shape;
    Shape.SetAsBox((width/2)/SCALE, (height/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 10.f;
    FixtureDef.friction = 5.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    return Body;
}

void Level::LoadLevel()
{
	character.LoadSprite();
	character.setBody(CreateDynamicObject(world, 0, 663, 32.f,37.f));
	CreateStaticObject(world, 512, 734, 1024, 68);
	character.GetSprite()->setPosition(SCALE * character.getBody()->GetPosition().x, SCALE * character.getBody()->GetPosition().y);
}

void Level::Blah(sf::RenderWindow& window, sf::Clock& frameTime)
{
	event.event(window, character);
	world.Step(frameTime.restart().asSeconds(), 8, 3);
	character.GetSprite()->setPosition(SCALE * character.getBody()->GetPosition().x, SCALE * character.getBody()->GetPosition().y);

}
void Level::Draw(sf::RenderWindow& window)
{
	window.draw(*character.GetSprite());
}

Character* Level::getCharacter()
{
	return &character;
}
