#include "../header/Event.h"

//On traite tous les evenements de la fenetre qui ont ete generée depuis la derniere iteration de la boucle
void Event::event(sf::RenderWindow& window, Character& character, bool canJump)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch(event.type)
		{
			//fermeture de la fenetre lorsque l'utilisateur le souhaite.
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyReleased:
			switch(event.key.code)
			{
			case sf::Keyboard::Down:
				if(character.GetCurrentDirection() == LOOK_DOWN)
					character.GetCurrentAnimation()->Stop();
				break;
			case sf::Keyboard::Left:
				if(character.GetCurrentDirection() == LOOK_LEFT)
					character.GetCurrentAnimation()->Stop();
				break;
			case sf::Keyboard::Right:
				if(character.GetCurrentDirection() == LOOK_RIGHT)
					character.GetCurrentAnimation()->Stop();
				break;
			case sf::Keyboard::Up:
				if(character.GetCurrentDirection() == LOOK_UP)
					character.GetCurrentAnimation()->Stop();
				break;
			default:
				break;
			}
			break;
		case sf::Event::Resized:
			// @TODO Gerer la taille du viewport lors du redimensionnement !
			break;
		default :
			break;
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		character.GetBody()->SetLinearVelocity(b2Vec2(10.f, character.GetBody()->GetLinearVelocity().y));
		character.SetAnimation(LOOK_RIGHT, ANIM_PLAY);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		character.GetBody()->SetLinearVelocity(b2Vec2(-10.f, character.GetBody()->GetLinearVelocity().y));
		character.SetAnimation(LOOK_LEFT, ANIM_PLAY);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		character.SetAnimation(LOOK_DOWN, ANIM_PLAY);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		character.SetAnimation(LOOK_UP, ANIM_PLAY);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canJump)
	{
		character.GetBody()->ApplyLinearImpulse(b2Vec2(0.f, -30.f), character.GetBody()->GetWorldCenter(), true);
	}

}

