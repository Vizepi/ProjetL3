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
