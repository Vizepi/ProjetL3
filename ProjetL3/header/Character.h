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
#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "RessourceLoader.h"
#include "Animation.h"
#include <iostream>
#include <stdio.h>
#include <vector>

#ifndef SCALE
#define SCALE 25.0
#endif

#define CHARACTER_VELOCITY 12.0

#define JUMP_IMPULSE 225.0

#define FALL_HEIGHT 300

#define CHARACTER_HEIGHT 36
#define CHARACTER_WIDTH 20
#define MAX_LIFE 7

class Character
{
	public :
        /** \brief Constructeur par defaut
         */
		Character(void);
		/** \brief Destructeur
		 */
		virtual ~Character(void);
        /** \brief Charge l'image du personnage.
         */
		void LoadSprite(void);
        /** \brief Retourne l'image du personnage
         *
         * \return Pointeur sur le sprite.
         */
		sf::Sprite* GetSprite(void);
        /** \brief Copie un sprite à la place de l'image actuelle.
         *
         * \param sprite Le sprite à copier.
         */
		void SetSprite(const sf::Sprite& sprite);
        /** \brief Retourne le corps physique.
         *
         * \return Un pointeur sur le corps physique du personnage.
         */
		b2Body* GetBody();
        /** \brief Remplace le corps physique du personnage.
         *
         * \param body Pointeur sur le nouveau corps.
         */
		void SetBody(b2Body* body);
        /** \brief Met à jour le personnage (animation, image, position).
         */
		void Update();
        /** \brief Change l'animation courante du personnage.
         *
         * \param direction Direction de l'animation.
         * \param play Est-ce que l'animation est jouée directement ?
         */
		void SetAnimation(unsigned int direction, bool play);
        /** \brief Retourne l'animation courante.
         * \return Pointeur sur l'animation courante du personnage.
         */
		Animation* GetCurrentAnimation(void);
        /** \brief Retourne la direction du personnage.
         * \return La direction du personnage (enum LOOK_DOWN, LOOK_LEFT, LOOK_RIGHT ou LOOK_UP).
         */
		int GetCurrentDirection(void) const;
        /** \brief Définis l'état du saut
         * \param state true pour autoriser, false pour empecher.
         */
		void EnableJump(bool state);

		void EnableClimb(bool state);
        /** \brief Indique si le caractère peut sauter.
         * \return true si le saut est autorisé, false sinon.
         */
		bool IsJumpEnabled(void) const;

		bool IsClimbEnabled(void) const;

		void SetLife(int life);

		int GetLife(void) const;

	protected:

	private:
		sf::Sprite m_image; /**< Image representant le personnage. */
		b2Body* m_body; /**< Objet physique representant le personnage. */
		std::vector<Animation*> m_anim; /**< Animateur de l'image. */
		Animation* m_currentAnimation; /**< Animation courante. */
		int m_currentDirection; /**< Direction courante. */
		bool m_jumpEnabled; /**< Le personnage peut-il sauter ? (collsion sous les pieds) */
		int m_climb;
		int m_life;

};

class JumpListener : public b2ContactListener
{
public:
	JumpListener(Character* character) : b2ContactListener(), m_character(character), m_fall(false)
	{
	}
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	int GetPos(void);
	void SetPos(int pos);
	bool GetLose(void);
private:
	Character* m_character;
	int m_pos;
	bool m_fall;
};

#endif // CHARACTER_H
