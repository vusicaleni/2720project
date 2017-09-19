/*
 * bullet.cpp
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

#include "bullet.h"

bullet::bullet(float x, float y)
{
	_x = x ; // x location
	_y = y ; // y location
	_width = 15; // Width of bullet
	_height = 2; // Height of bullet
	_xVel = 15.2f; // How fast the bullet travels
}

bullet::~bullet()
{
	
}

void bullet::paint()
{
	if (_isAlive) 
	{
		//al_draw_rectangle(_x,_y,_x + _width, _y + _height, al_map_rgb(rand() % 255 + 155,0,0), 1);
		al_draw_filled_rectangle(_x, _y, _x + _width, _y + _height, al_map_rgb(255,0,12));
	}
}

void bullet::render()
{
	if (_isAlive)
	{
		if (_x < ScreenWidth)
		{
			_x += _xVel;
		}
		else _isAlive = false;
	}
}