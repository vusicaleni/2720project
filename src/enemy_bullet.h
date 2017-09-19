/*
 * enemey_bullet.h
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

#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "drawable.h"
#include "moveable.h"
#include "bullet.h"
#include "global.h"

/// This is what the enemy object shoots at you
class enemy_bullet : public drawable, public moveable {
	/// Friends with bullet class
	friend class bullet; 
	public:
		/// Ctor, x and y coordinates
		enemy_bullet(float x, float y);
		/// Dtor
		virtual ~enemy_bullet();
		/// Paint function
		void paint(); 
		/// Render function
		void render(); 
		float getX(){return _x;}
		float getY(){return _y;}
		float getWidth(){return _width;}
		bool getIsAlive(){return _isAlive;}
		/// Disables the bullet from being painted or rendered
		void deactivate(){_isAlive = false;}
	private:
		/// Starting x and y coordinates
		float _x, _y;
		/// Width and height
		float _width, _height; 
		/// x Velocity 
		float _xVel; 
		/// Is it alive or dead
		bool _isAlive = true; 
};


#endif /* ENEMY_BULLET_H */