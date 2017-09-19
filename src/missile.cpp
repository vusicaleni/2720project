/*
 * missile.cpp
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


#include "missile.h"


missile::missile(float x, float y, bool bottom)
{
	_x = x; _y = y;
	_width = 15; 
	_height = 2;
	_xVel = 15.2f; // How fast the missile travels
	missile_1 = al_load_bitmap("res/missile.png"); // Bitmap for missle
	_bottom = bottom; // firing from the top or bottom of the ship
}


missile::~missile()
{
	al_destroy_bitmap(missile_1);
}

void missile::paint()
{
	// Draw missle
	if (_isAlive) 
	{
		//al_draw_filled_rectangle(_x, _y, _x + _width, _y + _height, al_map_rgb(255,0,12)); // Testing
		al_draw_bitmap(missile_1, _x, _y, 0); // Thrusters image + firing
	}
	
	// Particles
	std::vector<std::shared_ptr<particle>>::iterator p;
	for (p = missile_fx.begin(); p != missile_fx.end(); p++)
	{
		(*p)->paint(); // Paint the particle fx
	}
}

void missile::render()
{
	moveCounter++;
	
	float xVel = 3; // X path
	float yVel = 2; // Y path
	
	if (_bottom)
	{
		// Missile is being shot from the bottom of the ship
		if (archUp == true)
		{
			_x += xVel;
			_y -= yVel;
			if (moveCounter == 20)
			{
				moveCounter = 0;
				archUp = false; // Stop arching forward
				archDown = true; // Start arching downward
			}
		}
		else if (archDown == true)
		{
			auto s = std::make_shared<particle>(); 
			s->initParticles(_x, _y + 2, 1, 2,2); 
			missile_fx.push_back(s);
			auto s9 = std::make_shared<particle>(); 
			s9->initParticles(_x, _y + 2, 1, 3,2); 
			missile_fx.push_back(s9);
			
			_x += xVel*5;
			_y += yVel;
			
			if (moveCounter == 20)
			{
				moveCounter = 0;
				archDown = false; // Stop arching forward
				moveForward = true; // Start arching downward
			}
		}
		else if (moveForward == true)
		{
			if (_isAlive)
			{
				auto s1 = std::make_shared<particle>(); 
				s1->initParticles(_x, _y + 2, 1, 2,2); 
				missile_fx.push_back(s1);
				auto s8 = std::make_shared<particle>(); 
				s8->initParticles(_x, _y + 2, 1, 3,2); 
				missile_fx.push_back(s8);
				
				if (_x < ScreenWidth)
				{
					_x += _xVel * 1.25;
				}
				else
				{
					_isAlive = false;
					moveForward = false;
					moveCounter = 0;
				}
			}
		}
	}
	else 
	{
		// Missile is being shot from the top of the ship
		if (archUp == true)
		{
			_x += xVel;
			_y += yVel;
			
			if (moveCounter == 20)
			{
				moveCounter = 0;
				archUp = false; // Stop arching forward
				archDown = true; // Start arching downward
			}
		}
		else if (archDown == true)
		{
			auto s = std::make_shared<particle>(); 
			s->initParticles(_x, _y + 2, 2, 1,2); 
			missile_fx.push_back(s);
			auto s7 = std::make_shared<particle>(); 
			s7->initParticles(_x, _y + 2, 2, 3,2); 
			missile_fx.push_back(s7);
			
			_x += xVel*5;
			_y -= yVel;
			
			if (moveCounter == 20)
			{
				moveCounter = 0;
				archDown = false; // Stop arching forward
				moveForward = true; // Start arching downward
			}
		}
		else if (moveForward == true)
		{
			if (_isAlive)
			{
				auto s1 = std::make_shared<particle>(); 
				s1->initParticles(_x, _y + 2, 2, 1,2); 
				missile_fx.push_back(s1);
				auto s6 = std::make_shared<particle>(); 
				s6->initParticles(_x, _y + 2, 2, 3,2); 
				missile_fx.push_back(s6);
				
				if (_x < ScreenWidth)
				{
					_x += _xVel * 1.25;
				}
				else
				{
					_isAlive = false;
					moveForward = false;
					moveCounter = 0;
				}
			}
		}
	}

	// Render the particles
	std::vector<std::shared_ptr<particle>>::iterator p;
	for (p = missile_fx.begin(); p != missile_fx.end(); p++)
	{
		(*p)->render(); // Render the particle fx
	}
	
}

