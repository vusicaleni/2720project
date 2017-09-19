/*
 * boss.cpp
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

#include "boss.h"

boss::boss(float x, float y, int lv)
{
	_isAlive = true;
	showHealth = true;
	
	moveCounter = 1;
	
	_x = x;
	_y = y;
	_isThruster = false;
	_xVelocity = 0;
	_yVelocity = 0;

	imageSelection = lv; // The boss bitmap will be selected according to the level
	
	switch (imageSelection){
		case 1:
			// EARTH
			image[0] = al_load_bitmap("res/boss0.png");
			image[1] = al_load_bitmap("res/boss0_1.png");
			_cannonDmg = 17; // Boss attack cannon attack 
			_health = 100; // Boss HP
			_shields = 100; // Boss Shields
			_shieldMax = _shields; // Max value for shields
		        _shieldMin = -30; // Min value for shileds
		        _shieldModifier = 1.3f;
		        _maxVelocity = 7;
			_shotModifier = 3; // Will adjust how often the cannon fires.
			_speed = 1.3f; // Boss speed
			break;
		case 2:
			// MARS
			image[0] = al_load_bitmap("res/boss1.png");
			image[1] = al_load_bitmap("res/boss1_1.png");
			_cannonDmg = 19; // Boss attack cannon attack 
			_health = 100; // Boss HP
			_shields = 100; // Boss Shields
			_shieldMax = _shields; // Max value for shields
			_shieldMin = -20; // Min value for shileds
			 _shieldModifier = 1.5f; // How fast the shields regenerate.
			_maxVelocity = 5;
			_shotModifier = 4; // Will adjust how often the cannon fires.
			_speed = 1.9f; // Boss speed
			break;
		case 3:
			// JUPITER
			image[0] = al_load_bitmap("res/boss2.png");
			image[1] = al_load_bitmap("res/boss2_1.png");
			_cannonDmg = 20; // Boss attack cannon attack 
			_health = 100; // Boss HP
			_shields = 100; // Boss Shields
			_shieldMax = _shields; // Max value for shields
			_shieldMin = -10; // Min value for shileds
			_shieldModifier = 1.6f; // How fast the shields regenerate.
			_maxVelocity = 5;
			_shotModifier = 5; // Will adjust how often the cannon fires.
			_speed = 2.1f; // Boss speed
			break;
		case 4: 
			// SATURN
			image[0] = al_load_bitmap("res/boss3.png");
			image[1] = al_load_bitmap("res/boss3_1.png");
			_cannonDmg = 18; // Boss attack cannon attack 
			_health = 100; // Boss HP
			_shields = 100; // Boss Shields
			_shieldMax = _shields; // Max value for shields
			_shieldMin = -5; // Min value for shileds
			_shieldModifier = 1.5f; // How fast the shields regenerate.
			_maxVelocity = 8;
			_shotModifier = 5; // Will adjust how often the cannon fires.
			_speed = 2.2f; // Boss speed
			break;
		case 5: 
			// URANUS
			image[0] = al_load_bitmap("res/boss4.png");
			image[1] = al_load_bitmap("res/boss4_1.png");
			_cannonDmg = 18; // Boss attack cannon attack 
			_health = 100; // Boss HP
			_shields = 100; // Boss Shields
			_shieldMax = _shields; // Max value for shields
			_shieldMin = -5; // Min value for shileds
			_shieldModifier = 1.8f; // How fast the shields regenerate.
			_maxVelocity = 5;
			_shotModifier = 4; // Will adjust how often the cannon fires.
			_speed = 2.0f; // Boss speed
			break;
		case 6: 
			// FINAL BOSS - NEPTUNE
			image[0] = al_load_bitmap("res/boss5.png");
			image[1] = al_load_bitmap("res/boss5_1.png");
			_cannonDmg = 23; // Boss attack cannon attack 
			_health = 100; // Boss HP
			_shields = 100; // Boss Shields
			_shieldMax = _shields; // Max value for shields
			_shieldMin = -5; // Min value for shileds
			_shieldModifier = 2.0f; // How fast the shields regenerate.
			_maxVelocity = 12;
			_shotModifier = 5; // Will adjust how often the cannon fires.
			_speed = 2.1f; // Boss speed
			break;
	}
	
	// Load the explosion bitmaps
	image[2] = al_load_bitmap("res/explode1.png");
	image[3] = al_load_bitmap("res/explode2.png");
	image[4] = al_load_bitmap("res/explode3.png");
	image[5] = al_load_bitmap("res/explode4.png");
	image[6] = al_load_bitmap("res/explode5.png");
	image[7] = al_load_bitmap("res/explode6.png");
	image[8] = al_load_bitmap("res/explode7.png");
	image[9] = al_load_bitmap("res/explode8.png");
	image[10] =  al_load_bitmap("res/explode9.png");
	
	// Load samples
	cannon = al_load_sample("res/cannon.ogg"); // Cannon sound fx
	explode = al_load_sample("res/explode.ogg"); // Explode sound fx
}

boss::~boss()
{
	for (int i = 0; i < 2; i++)
	{
		al_destroy_bitmap(image[i]); // Clean up all images
	}
	
	boss_bullets.clear();
	draw_ptr.clear();
	move_ptr.clear();
	
	// Delete samples etc
	al_destroy_sample(explode);
	al_destroy_sample(cannon);
}

void boss::paint()
{
	if (startDeathAnimation == false)
	{
		if (_isThruster)
		{
			al_draw_bitmap(image[1], _x, _y, 0); // Thrusters
			
		}
		else al_draw_bitmap(image[0], _x, _y, 0); // Without thrusters
		
		if (showHealth)
		{
			int xMod = 20; // x modifier so that the bars display correctly
			int yMod = 10; // y modifier
			// Health bar
			if (_health > 80 && _health <= 105)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _health, _y - yMod + 2, al_map_rgb(0,255,0)); //Show health bar
			}
			else if (_health > 60 && _health <= 80)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _health, _y - yMod + 2, al_map_rgb(0,200,0)); 
			}
			else if (_health > 40 && _health <= 60)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _health, _y - yMod + 2, al_map_rgb(255,222,0));
			}
			else if (_health > 20 && _health <= 40)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _health, _y - yMod + 2, al_map_rgb(255,98,0)); 
			}
			else if (_health > 0 && _health <= 20)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _health, _y - yMod + 2, al_map_rgb(255,0,0)); 
			}
			
			yMod += 3; // Adjust for the shields
			
			// Shields bar
			if (_shields > 80 && _shields <= 105)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _shields, _y - yMod + 2, al_map_rgb(0,150,255)); //Show shields
			}
			else if (_shields > 60 && _shields <= 80)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _shields, _y - yMod + 2, al_map_rgb(0,18,255)); 
			}
			else if (_shields > 40 && _shields <= 60)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _shields, _y - yMod + 2, al_map_rgb(150,255,0));
			}
			else if (_shields > 20 && _shields <= 40)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _shields, _y - yMod + 2, al_map_rgb(255,222,0)); 
			}
			else if (_shields > 0 && _shields <= 20)
			{
				al_draw_filled_rectangle(_x - xMod, _y - yMod, _x - xMod + _shields, _y - yMod + 2, al_map_rgb(255,0,156)); 
			}
		}
		
		std::vector<std::shared_ptr<drawable>>::iterator it; 
		for (it = draw_ptr.begin(); it != draw_ptr.end(); it++){
			(*it)->paint(); // Draw the boss bullets (cannon) onto the screen.
		}
	}
	else
	{
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
			
			if (frameCount == 59) _isAlive = false;
		}
	}
}

void boss::render()
{
	if (_isAlive == true && startDeathAnimation == false)
	{
		moveCounter++;
		
		// Shield regeneration 
		if (_shields <=  _shieldMax)
		{
			_shields += 0.2f * _shieldModifier;
			
		}
		
		// Speed and velocity --------------------------------------------------------------------------------
		float decelerate = 0.04f; // If the xVelocity is > 0, the thrusters will decelerate until it reaches 0.
		float accelerate = 0.03f; // If the xVelocity is < 0, the thrusters boost to until it reaches 0.
		
		float trigger = 0.02f; // Will engage thrusters if xVel > or < trigger (lower is better)
		
		int direction = rand() % 4 + 1; // This will randomize the movement -- very basic
		
		if (moveCounter % 20 == 0) 
		{
			float modifier = 1.25f; // Will determine the speed in the direction of travel
			// Move 6 times / 2 seconds
			switch (direction)
			{
				case 1:
					// UP
					_yVelocity -= _speed * modifier;
					break;
				case 2: 
					// LEFT
					_xVelocity -= _speed * modifier;
					break;
				case 3: 
					// DOWN
					_yVelocity += _speed * modifier;
					break;
				case 4: 
					// RIGHT
					_xVelocity += _speed * modifier;
					break;
			}
		}
		
		// This will make sure we stay on the screen.
		if (_x <= 0)
		{
			_xVelocity = 0;
			_x += 5;
		}
		else if (_x + 64 > ScreenWidth)
		{
			_xVelocity = 0;
			_x -= 5;
		}
		if (_y <= 0)
		{
			_yVelocity = 0;
			_y += 5;
		}
		else if (_y + 64 > ScreenHeight)
		{
			_yVelocity = 0;
			_y -= 5;
		} 
		
		// And so will this. Basically we want the boss to stay within a certain bounds on the screen. This will help by adjusting
		// _x and _y (+/-)
		if (_x <= 256) _x += 2;
		if (_y <= 100) _y += 2;
		else if (_y >= 600) _y-= 2;
		
		// Changes the thruster state, on or off. This will change the animation of the sprite.
		if (_xVelocity > trigger * 15 || _yVelocity > trigger * 15 || _yVelocity < -trigger * 15)
		{
			_isThruster = true;
		}
		else
		{
			_isThruster = false;
		}
		
		// Autothrusters (decrease or increase velocity when controls are not active) trigger = when they activate
		
		if (_xVelocity > trigger)
		{
			
			_xVelocity -= decelerate;
		}
		
		if(_xVelocity < trigger)
		{
			
			_xVelocity += accelerate;
		}
		
		if (_yVelocity > trigger)
		{ 	
			
			_yVelocity -= decelerate;
		}
		
		if(_yVelocity < trigger)
		{
			_yVelocity += accelerate;
		}
		
		int randNum = rand() % 100 + 1; // This will randomize the cannon
		
		if (randNum < _shotModifier) // will randomize the shot for the boss.
		{
			fireCannon(); // _shotModifier% chance that the cannon fire during render (_shotModifier% - 60 updates/sec)
		}
		
		std::vector<std::shared_ptr<moveable>>::iterator it;
		for (it = move_ptr.begin(); it != move_ptr.end(); it++){
			(*it)->render(); // Move the boss bullets (cannon) on the screen.
		}
		
		// Rendering boss here
		_x += _xVelocity;
		_y += _yVelocity;   
	
	}
}

void boss::fireCannon()
{
	if (_isAlive == true && startDeathAnimation == false)
	{
		al_play_sample(cannon, 0.9f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&cannonID);
		// Top cannon
		auto ptr = std::make_shared<enemy_bullet>(_x + 20, _y + 6);
		move_ptr.push_back(ptr);
		draw_ptr.push_back(ptr);
		boss_bullets.push_back(ptr);
		// Bottom cannon
		auto ptr2 = std::make_shared<enemy_bullet>(_x + 20, _y + 58);
		move_ptr.push_back(ptr2);
		draw_ptr.push_back(ptr2);
		boss_bullets.push_back(ptr2);
	}
}

void boss::takeDamage(float dmg)
{
	if (_isAlive && startDeathAnimation == false)
	{
		if (_shields > 0) // If the shields are > 0, absorb damage, else take damage directly.
		{
			colourCycle = true;
			
			_shields -= dmg / _shieldModifier; // If shields active, take hit on shields / shield modifier
			//al_play_sample(shieldhit, 0.5f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&shieldID);
		}
		
		if (_shields <= 10 && _health > 0)
		{
			if (_shields > _shieldMin)
			{
				_shields -= dmg; // This will disable the shields, taking additional damage until they reach _shieldMin.
			}
			_health -= dmg; // Take damage to ship
		}
		
		
		if (_health <= 0)
		{
			 // Die
			 al_play_sample(explode, 1.5f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&explodeID); // Play explosion sound fx
			 startDeathAnimation = true;
			 // Good job bud.
		}
	}
}
