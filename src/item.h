/*
 * item.h
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


#ifndef ITEM_H
#define ITEM_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>  

#include "drawable.h"
#include "moveable.h"
#include "global.h"
/// Item class (dropped by enemy)
class item : public drawable, public moveable{
private:
	/// X and Y coordinates
	float _x, _y;
	/// Is the item alive
	bool _isAlive;
	/// Item type will get a random value and from that, it will select the bitmap / properties.
	int itemType;
	/// For the back and forth movement (the range)
	int xMax, xMin;
	/// A counter to keep track of the movement
	int moveCounter;
	/// Toggles the item moving back and forth.
	bool down;
	/// Allegro bitmap
	ALLEGRO_BITMAP *items = NULL;
public:
	/// Ctor -  with x and y coordinates
	item(float x, float y);
	/// Dtor
	virtual ~item();
	/// Virtual render (move)
	void render();
	/// Virtual paint (draw)
	void paint();
	/// Is the item alive
	bool getIsAlive(){ return _isAlive;}
	float getX(){ return _x;}
	float getY(){ return _y;}
	/// Deactivate the item, no longer draw/move
	void deactivate(){_isAlive = false;}
};


#endif /* ITEM_H */ 
