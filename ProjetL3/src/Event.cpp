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
				{
					character.GetCurrentAnimation()->Stop();
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					character.GetBody()->SetLinearVelocity(b2Vec2(CHARACTER_VELOCITY, character.GetBody()->GetLinearVelocity().y));
					character.SetAnimation(LOOK_RIGHT, true);
				}
				else
					character.GetBody()->SetLinearVelocity(b2Vec2(0.f, character.GetBody()->GetLinearVelocity().y));
				break;
			case sf::Keyboard::Right:
				if(character.GetCurrentDirection() == LOOK_RIGHT)
				{
					character.GetCurrentAnimation()->Stop();
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					character.GetBody()->SetLinearVelocity(b2Vec2(-CHARACTER_VELOCITY, character.GetBody()->GetLinearVelocity().y));
					character.SetAnimation(LOOK_LEFT, true);
				}
				else
					character.GetBody()->SetLinearVelocity(b2Vec2(0.f, character.GetBody()->GetLinearVelocity().y));
				break;
			case sf::Keyboard::Up:
				if(character.GetCurrentDirection() == LOOK_UP)
					character.GetCurrentAnimation()->Stop();
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyPressed:
			switch(event.key.code)
			{
			case sf::Keyboard::Down:
				if(character.IsClimbEnabled())
					character.GetBody()->SetLinearVelocity(b2Vec2(character.GetBody()->GetLinearVelocity().x, CHARACTER_VELOCITY));
				character.SetAnimation(LOOK_DOWN, ANIM_PLAY);
				break;
			case sf::Keyboard::Left:
				character.GetBody()->SetLinearVelocity(b2Vec2(-CHARACTER_VELOCITY, character.GetBody()->GetLinearVelocity().y));
				character.SetAnimation(LOOK_LEFT, ANIM_PLAY);
				break;
			case sf::Keyboard::Right:
				character.GetBody()->SetLinearVelocity(b2Vec2(CHARACTER_VELOCITY, character.GetBody()->GetLinearVelocity().y));
				character.SetAnimation(LOOK_RIGHT, ANIM_PLAY);
				break;
			case sf::Keyboard::Up:
				if(character.IsClimbEnabled())
					character.GetBody()->SetLinearVelocity(b2Vec2(character.GetBody()->GetLinearVelocity().x, -CHARACTER_VELOCITY));
				character.SetAnimation(LOOK_UP, ANIM_PLAY);
				break;
			case sf::Keyboard::Space:
				if(character.IsJumpEnabled())
					character.GetBody()->ApplyLinearImpulse(b2Vec2(0.f, -225.f), character.GetBody()->GetWorldCenter(), true);
				character.EnableJump(false);
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

}

