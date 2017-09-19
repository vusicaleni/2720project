/*
 * boss.h
 * 
 * Copyright 2015 - 2720 Final Project
 * Vu Leni - Will Hleucka - Naif Saud
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef BOSS_H
#define BOSS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>

#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>  

#include "drawable.h"
#include "moveable.h"
#include "global.h"
#include "enemy_bullet.h"

/// End level boss object
class boss : public drawable, public moveable{
	private:
		/// Starting X and Y coordinates
		float _x, _y; 
		/// Boss health points
		float _health; 
		/// Boss shield points
		float _shields; 
		/// Max value for shields
		float _shieldMax; 
		/// Min value for shileds
		float _shieldMin; 
		/// Speed of the boss
		float _speed; 
		/// Damage Modifier. Depending on the level, the boss will do more or less damage.
		//float _damageModifier; -- not used, as the bosses are hard enough as is
		/// Shield Modifier.  Depending on the level, the boss will take more or less damage.
		float _shieldModifier;
		/// Will modify how random the shooting is depending on the boss type.
		int _shotModifier;
		/// Determines if the enemy is alive
		bool _isAlive; 
		/// Determines if moving up, forward, down
		bool _isThruster; 
		/// Will show HP and shields on screen
		bool showHealth; 
		/// Will update xVelocity and yVelocity in a random direction according to frames/sec
		int moveCounter; 
		/// Cannon damage points
		float _cannonDmg; 
		/// How fast up or down, left or right
		float _xVelocity, _yVelocity; 
		/// The max the ship can move
		float _maxVelocity; 
		/// This will determine what type of enemey image we will use from the array
		int imageSelection; 
		/// This will cause a frame delay for our explosion animation
		int frameCount = 1; 
		/// Array of images for the boss
		ALLEGRO_BITMAP *image[11]; 
		/// Start at index 1 for explosion animation, go to index 11 and stop.
		int deathAnimation = 1; 
		/// Sample for the cannon sound fx
		ALLEGRO_SAMPLE *cannon = NULL;
		/// Sample for the explode sound fx
		ALLEGRO_SAMPLE *explode = NULL;
		/// Sample ID for cannon
		ALLEGRO_SAMPLE_ID cannonID; 
		/// Sample ID for the explosion
		ALLEGRO_SAMPLE_ID explodeID; 
		/// Boss Drawable Bullets
		std::vector<std::shared_ptr<drawable>> draw_ptr; 
		/// Boss Moveable Bullets
		std::vector<std::shared_ptr<moveable>> move_ptr; 
		/// Fire boss cannon
		void fireCannon(); 
		/// For the shield colour cycle
		bool colourCycle; 
		/// Will trigger dealth animation if set to true
		bool startDeathAnimation; 
	public:
		/// Ctor with X and Y coordinates, and level indicator (ie, 1, 2, 3)
		boss(float x, float y, int lv);
		/// Dtor
		virtual ~boss();
		/// Paint function
		void paint();
		/// Render function
		void render();
		float getX(){ return _x;}
		float getY(){ return _y;}
		float getHealth(){ return _health;}
		float getShields(){ return _shields;}
		bool getIsAlive(){ return _isAlive;}
		float getCannonDmg(){return _cannonDmg;}
		/// Vector of bullets for the Boss
		std::vector<std::shared_ptr<enemy_bullet>> boss_bullets; 
		/// This function will impact either shields or health with dmg value
		void takeDamage(float dmg);
};

#endif /* BOSS_H */ 