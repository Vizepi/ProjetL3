/*

	Platformer Game - Made for the 3rd year of undergraduated project.
    Copyright (C) 2015  Corbat Lisa, Kieffer Joseph

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "../header/Event.h"
#include "../header/Game.h"

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
			Game::s_instance->SwitchState(STATE_QUIT);
			break;
		case sf::Event::KeyReleased:
			switch(event.key.code)
			{
				#ifndef USE_FULLTIME_TESTS
			case sf::Keyboard::Down:
				if(character.GetCurrentDirection() == LOOK_UP)
				{
					character.GetCurrentAnimation()->Stop();
				}
				character.GetBody()->SetLinearVelocity(b2Vec2(character.GetBody()->GetLinearVelocity().x, 0));
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
				{
					character.GetCurrentAnimation()->Stop();
				}
				character.GetBody()->SetLinearVelocity(b2Vec2(character.GetBody()->GetLinearVelocity().x, 0));
				break;
				#endif // USE_FULLTIME_TESTS
			case sf::Keyboard::P:
				character.GetBody()->SetLinearVelocity(b2Vec2(0, 0));
				character.GetCurrentAnimation()->Stop();
				if(Game::s_instance->IsMusicActive())
					RessourceLoader::GetMusic("Level")->pause();
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetMusic("Clock")->pause();
				Game::s_instance->SwitchState(STATE_PAUSE);
				break;
			case sf::Keyboard::Escape:
				if(Game::s_instance->IsMusicActive())
					RessourceLoader::GetMusic("Level")->stop();
				Game::s_instance->SwitchState(STATE_MENU);
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyPressed:
			switch(event.key.code)
			{
				#ifndef USE_FULLTIME_TESTS
			case sf::Keyboard::Down:
				if(character.IsClimbEnabled())
				{
					character.GetBody()->SetLinearVelocity(b2Vec2(character.GetBody()->GetLinearVelocity().x, CHARACTER_VELOCITY));
					character.SetAnimation(LOOK_UP, ANIM_PLAY);
				}
				break;
			case sf::Keyboard::Left:
				character.GetBody()->SetLinearVelocity(b2Vec2(-CHARACTER_VELOCITY, character.GetBody()->GetLinearVelocity().y));
				if(!character.IsClimbEnabled())
				{
					character.SetAnimation(LOOK_LEFT, ANIM_PLAY);
				}
				break;
			case sf::Keyboard::Right:
				character.GetBody()->SetLinearVelocity(b2Vec2(CHARACTER_VELOCITY, character.GetBody()->GetLinearVelocity().y));
				if(!character.IsClimbEnabled())
				{
					character.SetAnimation(LOOK_RIGHT, ANIM_PLAY);
				}
				break;
			case sf::Keyboard::Up:
				if(character.IsClimbEnabled())
				{
					character.GetBody()->SetLinearVelocity(b2Vec2(character.GetBody()->GetLinearVelocity().x, -CHARACTER_VELOCITY));
					character.SetAnimation(LOOK_UP, ANIM_PLAY);
				}
				break;
				#endif // USE_FULLTIME_TESTS
			case sf::Keyboard::Space:
				if(character.IsJumpEnabled() && !character.IsClimbEnabled())
				{
					character.GetBody()->SetGravityScale(1.f);
					character.GetBody()->ApplyLinearImpulse(b2Vec2(0.f, -JUMP_IMPULSE), character.GetBody()->GetWorldCenter(), true);
				}
				character.EnableJump(false);
				break;
			default:
				break;
			}
			break;
		case sf::Event::Resized:
			// Gere la taille du viewport lors du redimensionnement !
			if(event.size.width > 1920)
				window.setSize(sf::Vector2u(1920, window.getSize().y));
			if(event.size.width < 800)
				window.setSize(sf::Vector2u(800, window.getSize().y));
			if(event.size.height > 1080)
				window.setSize(sf::Vector2u(window.getSize().x, 1080));
			if(event.size.height < 600)
				window.setSize(sf::Vector2u(window.getSize().x, 600));
			break;
		default :
			break;
		}
	}

	#ifdef USE_FULLTIME_TESTS
	int dir = 0;
	if(character.IsClimbEnabled())
	{
		character.GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
	}
	else
	{
		character.GetBody()->SetLinearVelocity(b2Vec2(0.f, character.GetBody()->GetLinearVelocity().y));
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		dir = dir | 0x1;
		character.GetBody()->SetLinearVelocity(b2Vec2(	character.GetBody()->GetLinearVelocity().x - CHARACTER_VELOCITY,
														character.GetBody()->GetLinearVelocity().y));
        if(character.GetCurrentDirection() != LOOK_LEFT)
		{
			character.GetCurrentAnimation()->Stop();
		}
		character.SetAnimation(LOOK_LEFT, ANIM_PLAY);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		dir = dir | 0x2;
		character.GetBody()->SetLinearVelocity(b2Vec2(	character.GetBody()->GetLinearVelocity().x + CHARACTER_VELOCITY,
														character.GetBody()->GetLinearVelocity().y));
        if(character.GetCurrentDirection() != LOOK_RIGHT)
		{
			character.GetCurrentAnimation()->Stop();
		}
		character.SetAnimation(LOOK_RIGHT, ANIM_PLAY);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		dir = dir | 0x4;
		if(character.IsClimbEnabled())
		{
			character.GetBody()->SetLinearVelocity(b2Vec2(	character.GetBody()->GetLinearVelocity().x,
															character.GetBody()->GetLinearVelocity().y - CHARACTER_VELOCITY));
			if(character.GetCurrentDirection() != LOOK_UP)
			{
				character.GetCurrentAnimation()->Stop();
			}
			character.SetAnimation(LOOK_UP, ANIM_PLAY);
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		dir = dir | 0x8;
		if(character.IsClimbEnabled())
		{
			character.GetBody()->SetLinearVelocity(b2Vec2(	character.GetBody()->GetLinearVelocity().x,
															character.GetBody()->GetLinearVelocity().y + CHARACTER_VELOCITY));
			if(character.GetCurrentDirection() != LOOK_UP)
			{
				character.GetCurrentAnimation()->Stop();
			}
			character.SetAnimation(LOOK_UP, ANIM_PLAY);
		}
	}
	if(dir == 0)
		character.GetCurrentAnimation()->Stop();
	#endif // USE_FULLTIME_TESTS

}

