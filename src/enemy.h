/*
 * enemy.h
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
 * 100
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>

#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>  

#include "enemy_bullet.h"
#include "drawable.h"
#include "moveable.h"
#include "global.h"
#include "item.h"

/// This is the enemy class. Most of our enemies are of this type.

class enemy : public drawable, public moveable{
	private:
		/// X & Y coordinates
		float _x, _y; 
		/// Enemy health points
		float _health; 
		/// Speed of enemy
		float _speed; 
		/// Determines if the enemy is alive
		bool _isAlive; 
		/// Laser damage points
		float _laserDmg; 
		/// Show the health bar or disable it
		bool showHealth; 
		/// Determine if spawned into item
		bool _isItem;
		/// This will determine what type of enemey image we will use from the array.
		int imageSelection; 
		/// This will cause a frame delay for our explosion animation
		int frameCount = 1; 
		/// Array of images for the enemy
		ALLEGRO_BITMAP *image[10]; 
		/// Start at index 1 for explosion animation, go to index 10 and stop.
		int deathAnimation = 1; 
		/// Laser sound fx
		ALLEGRO_SAMPLE *laser = NULL; 
		/// Zap sound fx
		ALLEGRO_SAMPLE *zap = NULL; 
		/// Explode sound fx
		ALLEGRO_SAMPLE *explode = NULL; 
		/// ID for zap sound fx
		ALLEGRO_SAMPLE_ID zapID; 
		/// ID for laser sound fx
		ALLEGRO_SAMPLE_ID laserID; 
		/// ID for explode sound fx
		ALLEGRO_SAMPLE_ID explodeID; 
		/// Enemy Drawable Bullets
		std::vector<std::shared_ptr<drawable>> draw_ptr; 
		/// Enemy Moveable Bullets
		std::vector<std::shared_ptr<moveable>> move_ptr; 
		/// This function will fire the blasters
		void fireBlaster();
	public:
		/// Ctor with X and Y coordinates
		enemy(float x, float y);
		/// Dtor
		virtual ~enemy();
		/// Render function
		void render(); 
		/// Paint function
		void paint(); 
		float getX(){ return _x;}
		float getY(){ return _y;}
		float getHealth(){ return _health;}
		bool getIsAlive(){ return _isAlive;}
		bool getIsItem(){ return _isItem;}
		float getLaserDmg(){ return _laserDmg;}
		/// Vector of bullets
		std::vector<std::shared_ptr<enemy_bullet>> enemy_bullets; 
		/// Adjust shields or health points by dmg value
		void takeDamage(float dmg); 
		/// Determines if item has spawned
		bool _hasSpawned;
	
};

#endif /* ENEMY_H */ 