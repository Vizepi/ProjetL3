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
#ifndef __ANIMATION_H
#define __ANIMATION_H

#define ANIM_PLAY true
#define ANIM_STOP false

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Rect.hpp>

enum
{
	LOOK_DOWN, LOOK_LEFT, LOOK_RIGHT, LOOK_UP
};

class Animation
{
	public:

		explicit Animation(sf::Sprite* sprite, sf::IntRect first, int count, int frameMs);
		void Update(void);
		void Play(void);
		void Stop(void);
		void Pause(void);

	private:

	protected:

		sf::Sprite* m_sprite;
		int m_count;
		int m_index;
		sf::IntRect m_rect;
		bool m_state;
		int m_frameTime;
		sf::Clock m_clock;

};

#endif // __ANIMATION_H
