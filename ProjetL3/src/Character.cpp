#include "../header/Character.h"

Character::Character()
: m_body(NULL)
, m_currentAnimation(NULL)
, m_currentDirection(LOOK_DOWN)
, m_jumpEnabled(true)
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

void Character::LoadSprite()
{
	m_image.setTexture(*RessourceLoader::GetTexture("Matt Smith"));
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
	m_image.setPosition(SCALE * m_body->GetPosition().x + m_image.getGlobalBounds().width/4.f, SCALE * m_body->GetPosition().y+1.f);

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

bool Character::IsJumpEnabled() const
{
	return m_jumpEnabled;
}
