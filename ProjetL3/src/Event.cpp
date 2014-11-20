#include "../header/Event.h"

//On traite tous les evenements de la fenetre qui ont ete generée depuis la derniere iteration de la boucle
void Event::event(sf::RenderWindow& window, Character& character)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch(event.type)
		{
			//fermeture de la fenetre lorsque l'utilisateur le souhaite.
			case sf::Event::Closed:
			{
				window.close();
			}
			break;
			default :
			{

			}
			break;
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		character.getBody()->SetLinearVelocity(b2Vec2(10.f, character.getBody()->GetLinearVelocity().y));
		character.GetSprite()->setTextureRect(sf::IntRect(0,94,32,47));
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		character.getBody()->SetLinearVelocity(b2Vec2(-10.f, character.getBody()->GetLinearVelocity().y));
		character.GetSprite()->setTextureRect(sf::IntRect(0,47 ,32,47));

	}
	else
	{
		character.GetSprite()->setTextureRect(sf::IntRect(0,0 ,32,47));
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		character.getBody()->ApplyLinearImpulse(b2Vec2(0.f, -30.f), character.getBody()->GetWorldCenter(), true);
	}

}

