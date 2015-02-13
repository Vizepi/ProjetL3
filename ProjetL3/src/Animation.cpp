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
#include "../header/Animation.h"

/*explicit*/ Animation::Animation(sf::Sprite* sprite, sf::IntRect first, int count, int frameMs)
: m_sprite(sprite)
, m_count(count)
, m_index(0)
, m_rect(first)
, m_state(ANIM_PLAY)
, m_frameTime(frameMs)
{
	m_clock.restart();
}

void Animation::Update(void)
{
	if(ANIM_PLAY == m_state)
	{
		if(m_clock.getElapsedTime().asMilliseconds() >= m_frameTime)
		{
			m_clock.restart();
			m_index = (m_index+1)%m_count;
		}
	}
	else
	{
		m_clock.restart();
	}
	m_rect.left = m_rect.width * m_index;
	m_sprite->setTextureRect(m_rect);
}

void Animation::Play()
{
	m_state = ANIM_PLAY;
	m_rect.left = m_rect.width * m_index;
	m_sprite->setTextureRect(m_rect);
}

void Animation::Stop()
{
	m_state = ANIM_STOP;
	m_index = 0;
}

void Animation::Pause()
{
	m_state = ANIM_STOP;
}

