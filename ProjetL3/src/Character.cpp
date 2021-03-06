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
#include "../header/Character.h"
#include "../header/Game.h"

Character::Character()
: m_body(NULL)
, m_currentAnimation(NULL)
, m_currentDirection(LOOK_DOWN)
, m_jumpEnabled(true)
, m_climb(0)
, m_life(MAX_LIFE)
{
	// Chargement de l'image
	LoadSprite();
	// Creation des animations pour les quatre directions.
	m_anim.push_back(new Animation(&m_image, sf::IntRect(0, 0, 32, 48), 4, 150));
	m_anim.push_back(new Animation(&m_image, sf::IntRect(0, 48, 32, 48), 4, 150));
	m_anim.push_back(new Animation(&m_image, sf::IntRect(0, 96, 32, 48), 4, 150));
	m_anim.push_back(new Animation(&m_image, sf::IntRect(0, 144, 32, 48), 4, 150));
	// Définition de l'animation courante.
	m_currentAnimation = m_anim[LOOK_DOWN];
	m_currentAnimation->Stop();
}

/*virtual*/ Character::~Character()
{
	while(m_anim.size() > 0)
	{
		delete m_anim[m_anim.size()-1];
		m_anim.pop_back();
	}
}

void Character::LoadSprite()
{
	m_image.setTexture(*RessourceLoader::GetTexture("Character"));
}

sf::Sprite* Character::GetSprite()
{
	return &m_image;
}

void Character::SetSprite(const sf::Sprite& sprite)
{
	m_image = sprite;
}

b2Body* Character::GetBody()
{
	return m_body;
}

void Character::SetBody(b2Body* body)
{
	m_body = body;
}

void Character::Update()
{
	if(m_currentAnimation)
		m_currentAnimation->Update();
	m_image.setPosition(SCALE * m_body->GetPosition().x + m_image.getGlobalBounds().width/4.f, SCALE * m_body->GetPosition().y - 6.f);

}

void Character::SetAnimation(unsigned int direction, bool play)
{
	if(direction < 0 || direction >= m_anim.size())
		return;
	m_currentDirection = direction;
	for(unsigned int i=0;i<m_anim.size();i++)
	{
		if(i == direction)
		{
			m_currentAnimation = m_anim[i];
			if(play)
				m_currentAnimation->Play();
			else
				m_currentAnimation->Stop();
		}
	}
}

Animation* Character::GetCurrentAnimation()
{
	return m_currentAnimation;
}

int Character::GetCurrentDirection() const
{
	return m_currentDirection;
}

void Character::EnableJump(bool state)
{
	m_jumpEnabled = state;
}

void Character::EnableClimb(bool state)
{
	int prev = m_climb;
	if(state)
		m_climb++;
	else
		m_climb--;
	if(m_climb <= 0)
	{
		m_climb = 0;
		m_body->SetGravityScale(1.f);
	}
	else
	{
		m_body->SetGravityScale(0.f);
		if(prev == 0)
			m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, 0.f));
	}
}

bool Character::IsJumpEnabled() const
{
	return m_jumpEnabled;
}

bool Character::IsClimbEnabled() const
{
	return (m_climb > 0);
}

void Character::SetLife(int life)
{
	m_life = life;
}

int Character::GetLife() const
{
	return m_life;
}

sf::RectangleShape Character::GetCollisionBox()
{
	sf::RectangleShape rs;
	rs.setPosition(	m_image.getPosition().x + (m_image.getGlobalBounds().width - CHARACTER_WIDTH) /2.0,
					m_image.getPosition().y + BLOC_SIZE - CHARACTER_HEIGHT);
	rs.setSize(sf::Vector2f(CHARACTER_WIDTH, CHARACTER_HEIGHT));
	return rs;
}


void JumpListener::BeginContact(b2Contact* contact)
{
    if(	(contact->GetFixtureA()->GetBody() == m_character->GetBody() && contact->GetFixtureA()->IsSensor()) ||
		(contact->GetFixtureB()->GetBody() == m_character->GetBody() && contact->GetFixtureB()->IsSensor()))
	{
			m_character->EnableJump(true);
	}
	else if((contact->GetFixtureA()->GetBody() != m_character->GetBody() && contact->GetFixtureA()->IsSensor() &&
		contact->GetFixtureB()->GetBody() == m_character->GetBody()) ||
		(contact->GetFixtureB()->GetBody() != m_character->GetBody() && contact->GetFixtureB()->IsSensor() &&
		contact->GetFixtureA()->GetBody() == m_character->GetBody()))
	{
		m_character->EnableClimb(true);
	}
	//Pour verifier si le personnage est en chute
	if (m_character->GetBody()->GetPosition().y * SCALE > m_pos+FALL_HEIGHT)
	{
		m_fall = true;
		if(Game::s_instance->IsSoundsActive())
			RessourceLoader::GetSound("Fall")->play();
		for(int i =0; i< 7; i++)
		{
			if((m_character->GetBody()->GetPosition().y * SCALE - m_pos) >= i*FALL_HEIGHT+FALL_HEIGHT)
			m_character->SetLife(m_character->GetLife()-1);
			if(m_character->GetLife() <= 0)
			{
				if(Game::s_instance->IsMusicActive())
					RessourceLoader::GetMusic("Level")->stop();
				if(Game::s_instance->IsSoundsActive())
					RessourceLoader::GetSound("Dead Fall")->play();
			}
		}
	}
	m_pos=m_character->GetBody()->GetPosition().y * SCALE;
}
void JumpListener::EndContact(b2Contact* contact)
{
	if(((contact->GetFixtureA()->GetBody() != m_character->GetBody() && contact->GetFixtureA()->IsSensor()) &&
		(contact->GetFixtureB()->GetBody() == m_character->GetBody() && !contact->GetFixtureB()->IsSensor())) ||
		((contact->GetFixtureB()->GetBody() != m_character->GetBody() && contact->GetFixtureB()->IsSensor()) &&
		(contact->GetFixtureA()->GetBody() == m_character->GetBody() && !contact->GetFixtureA()->IsSensor())))
	{
		m_character->EnableClimb(false);
	}
	m_pos=m_character->GetBody()->GetPosition().y * SCALE;
}

int JumpListener::GetPos()
{

	return m_pos;
}
void JumpListener::SetPos(int pos)
{
	m_pos = pos;
}

