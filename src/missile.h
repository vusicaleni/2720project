/*
 * missile.h
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


#ifndef MISSILE_H
#define MISSILE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "drawable.h"
#include "moveable.h"
#include "global.h"
#include "bullet.h"
#include "particle.h"

/// Missile class
class missile : public drawable, public moveable
{
	/// Friends with bullet class
	friend class bullet; 
	public:
		/// Ctor
		missile(float x, float y, bool bottom);
		/// Dtor
		virtual ~missile();
		/// Virtual paint (draw)
		void paint();
		/// Virtual render (move)
		void render();
		/// Return x coordinate
		float getX(){return _x;}
		/// Return y coordinate 
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
		/// Image for missile
		ALLEGRO_BITMAP *missile_1;
		/// Vector of particles for effects
		std::vector<std::shared_ptr<particle>> missile_fx;
		/// x Velocity of missle (how fast it moves forward)
		float _xVel; 
		/// Is it alive or dead
		bool _isAlive = true; 
		/// Arch up - this will move the missile up and forward.
		bool archUp = true;
		/// Arch down - this will move the missile down and forward.
		bool archDown = false;
		/// moveForward - this will move the missile on a forward path 
		bool moveForward = false;
		/// Bool top/bottom - this will alternate shooting from the top and bottom of the ship
		bool _bottom;
		/// This will time the booleans for the projected path of the missile. 
		int moveCounter = 0;
};

#endif /* MISSILE_H */ 
