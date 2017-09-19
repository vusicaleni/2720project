/*
 * enemy.cpp
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

#include "enemy.h"

enemy::enemy(float x, float y)
{
	// X and Y coordinates for the enemy
	_x = x;
	_y = y;
	_isAlive = true; // The enemy is alive until it dies
	_health = 64;
	_speed = rand() % 5 + 2; // speed can vary between 2 and 4
	showHealth = true;
	
	
	// Load bitmap for enemy
	imageSelection = rand() % 5; // Randomizer for what bitmap we use
	_laserDmg = rand() % 19 + 5; // Randomize their damage
	
	switch (imageSelection){
		case 0:
			image[0] =  al_load_bitmap("res/enemy0.png"); // Orange 1
			
			break;
		case 1: 
			image[0] =  al_load_bitmap("res/enemy1.png"); // Orange 2
		
			break;
		case 2:
			image[0] =  al_load_bitmap("res/enemy2.png"); // Black Probe
			
			break;
		case 3:
			image[0] =  al_load_bitmap("res/enemy3.png"); // Small White
			
			break;
		case 4: 
			image[0] = al_load_bitmap("res/enemy4.png"); // Small Red
			break;
	}
	
	// Load the explosion images
	image[1] = al_load_bitmap("res/explode1.png");
	image[2] = al_load_bitmap("res/explode2.png");
	image[3] = al_load_bitmap("res/explode3.png");
	image[4] = al_load_bitmap("res/explode4.png");
	image[5] = al_load_bitmap("res/explode5.png");
	image[6] = al_load_bitmap("res/explode6.png");
	image[7] = al_load_bitmap("res/explode7.png");
	image[8] = al_load_bitmap("res/explode8.png");
	image[9] = al_load_bitmap("res/explode9.png");
	
	
	
	laser = al_load_sample("res/laser.ogg"); // Laser sound fx
	explode = al_load_sample("res/explode.ogg"); // Explode sound fx

}

enemy::~enemy()
{
	for (int i = 0; i < 2; i++)
	{
		al_destroy_bitmap(image[i]); // Clean up all images
	}
	
	enemy_bullets.clear();
	draw_ptr.clear();
	move_ptr.clear();
	
	al_destroy_sample(laser);
	al_destroy_sample(explode);
}

void enemy::render()
{
		//if (_x < 0) _isAlive = false;
		
		float moveModifier = -0.15f; // How many pixels the enemy moves (right)
		_x += moveModifier * _speed; 
		
		
		int randNum = rand() % 100 + 1; // This will randomize the blasters
		int randNum2 = rand() % 100 + 1; // This will randomize the movement
		
		if (randNum2 <= 1)
		{
			// 1% chance of moving up, makings sure it won't travel off the screen
			if (_y > 0)_y -= 2;
			else _y += 2;
		}
		else if (randNum2 >= 99)
		{
			// 1% chance of moving down, makings sure it won't travel off the screen
			if (_y + 64 < ScreenHeight) _y += 2;
			else _y -= 2;
		}
	
		
		if (randNum < 3 && _x < ScreenWidth && _x > 0) 
		{
			fireBlaster(); // 2% chance that the blasters fire during render, must be on screen.
		}
		
		if (_x < -64) _isAlive = false; // if they go off screen, they get killed.
		
		std::vector<std::shared_ptr<moveable>>::iterator it;
		for (it = move_ptr.begin(); it != move_ptr.end(); it++){
			(*it)->render(); // Move the enemy bullets on the screen.
		}
}

void enemy::paint()
{
	if (_isAlive)
	{
		if (showHealth)
		{
			if (_health > 54 && _health <= 64)
			{
				al_draw_filled_rectangle(_x, _y, _x + _health, _y + 2, al_map_rgb(0,255,0)); //Show health bar
			}
			else if (_health > 44 && _health <= 54)
			{
				al_draw_filled_rectangle(_x, _y, _x + _health, _y + 2, al_map_rgb(0,200,0)); 
			}
			else if (_health > 34 && _health <= 44)
			{
				al_draw_filled_rectangle(_x, _y, _x + _health, _y + 2, al_map_rgb(255,222,0)); 
			}
			else if (_health > 24 && _health <= 34)
			{
				al_draw_filled_rectangle(_x, _y, _x + _health, _y + 2, al_map_rgb(255,98,0)); 
			}
			else if (_health > 0 && _health <= 24)
			{
				al_draw_filled_rectangle(_x, _y, _x + _health, _y + 2, al_map_rgb(255,0,0)); 
			}
		}
		
		if (_x < ScreenWidth && _x > -64)
		{
			al_draw_bitmap(image[0], _x, _y, 0); // Using the image with the thrusters active
		}
		
		
		std::vector<std::shared_ptr<drawable>>::iterator it; 
		for (it = draw_ptr.begin(); it != draw_ptr.end(); it++){
			(*it)->paint(); // Draw the enemy bullets onto the screen.
		}
	}
	else{
		if (_x < ScreenWidth && _x > -64)
		{
			if (frameCount < 60) frameCount++; // 60 frames / second, so the death animation is around 10 frames. Will update bitmap 10x / sec
			if (frameCount % 6 == 0)
			{
				if (deathAnimation != 10)
				{
					al_draw_bitmap(image[deathAnimation++], _x, _y, 0); // Using the image with the thrusters active
				}
			}
			else 
			{
				if (deathAnimation != 10) al_draw_bitmap(image[deathAnimation], _x, _y, 0);
			}
			
		}
	}
}

void enemy::fireBlaster()
{
	if (_isAlive && _x < ScreenWidth)
	{
		//showHealth = false; // Disable health bar firing weapon
		al_play_sample(laser, 0.4f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&laserID);
		auto ptr = std::make_shared<enemy_bullet>(_x + 20, _y + 30);
		move_ptr.push_back(ptr);
		draw_ptr.push_back(ptr);
		enemy_bullets.push_back(ptr);
	}

}

void enemy::takeDamage(float dmg)
{	if (_isAlive)
	{
		if (_x < ScreenWidth)
		{
			//al_play_sample(zap, 0.35f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&zapID);
			_health -= dmg; // If on screen then take damage.
			_x++;
		}
	
		if (_health <= 0)
		{
			 al_play_sample(explode, 1.5f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&explodeID); // Play explosion sound fx
			_isAlive = false;
			
			int itemDrop = rand() % 100;
			
			if (itemDrop <= 29)
			{
				_isItem = true;
			}
		}
	}
}
