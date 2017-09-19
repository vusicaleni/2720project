/*
 * particle.h
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

#ifndef PARTICLE_H
#define PARTICLE_H

#include <memory>
#include <vector>
#include <stdlib.h>  

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "drawable.h"
#include "moveable.h"
#include "global.h"

/// Structure for the particle effects
struct part{
	/// Starting X and Y coordinates
	float _x, _y;
	/// Width of particle
	int _width;
	/// Height of particle
	int _height;
	/// Direction X and Y, this will determine the speed/angle of the particle
	float _dirX, _dirY;
	/// Particle life - when this variable is < 0, particles won't draw/move anymore
	float _life;
	/// This variable will get a random value between  1 - 8, corresponding with direction NE, N, NW, W, SW, S, SE, E
	int _direction;
	/// This will be assigned a random colour value. Yellow; Orange; Red; Blue; White
	int _colour;
	/// Is the particle alive or not
	bool _isAlive; 
	/// Type of particle effect 0 = explosion, 1 = thrusters, 2 = missiles, 3 = shields
	int _type;
};

/// Particles for explosions effects, collisions, etc.
class particle : public drawable, public moveable{	
	private:		
		/// Vector of particles "part"
		std::vector<part> particles;
	public:
		/// Ctor
		particle();
		/// Dtor
		virtual ~particle();
		/// Initialize the particle vector. This will require the X and Y coordinates of the object exploding, also the number of particles to generate and colour 1 = orange, 2 = yellow, 3 = red, 4 = blue, 5 = white
		void initParticles(float x, float y, int numParticles, int colour, int type);
		/// Paint function
		void paint();
		/// Render function
		void render();
};


#endif /* PARTICLE_H */
