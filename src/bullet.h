/*
 * bullet.h
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

#ifndef BULLET_H
#define BULLET_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "drawable.h"
#include "moveable.h"
#include "global.h"

/// This is what our ship class fires
class bullet : public drawable, public moveable{
	public:
		/// Ctor with X and Y coordinates
		bullet(float x, float y); 
		/// Dtor
		virtual ~bullet();
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
		/// Starting X and Y coordinates
		float _x, _y;
		/// Width and height of bullet
		float _width, _height; 
		/// x Velocity 
		float _xVel; 
		/// For the colour cycle of the bullet
		int _red; 
		/// Is it alive or dead
		bool _isAlive = true; 
};

#endif /* BULLET_H */ 