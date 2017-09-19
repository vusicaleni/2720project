/*
 * ship.cpp
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


#include "ship.h"

ship::ship()
{
	_isAlive = true;
	// Starting x and y coordinates for the ship
	_x = 5;
	_y = ScreenHeight / 2;
	thrust = 0.025f; // Thrust will increment us every time we change direction
	_maxVelocity = 5; // Max velocity controls how fast our ship can move
	xVelocity = yVelocity = 0; // Set to 0
	
	_health = 100; // Health Points
	_shields = 100; // Shield max
	_shieldMin = -30; // From -30 to 0 the shields are disabled
	_shieldMax = 100; // Shields 1 - 100
	
	_missileDmg = 45.0f; //  Missile damage points
	
	// Red ship
	image[0] = al_load_bitmap("res/redship0.png"); // No thruster image
	image[1] = al_load_bitmap("res/redship1.png"); // Thruster image
	image[2] = al_load_bitmap("res/redship2.png"); // Fire blaster without thruster
	image[3] = al_load_bitmap("res/redship3.png"); // Fire blaster with thruster
	
	doubleblaster = al_load_sample("res/doubleblaster.ogg"); // Double blaster sound fx
	thruster = al_load_sample("res/thruster.ogg"); // Thruster sound fx
	shieldhit = al_load_sample("res/shieldhit.ogg"); // Shield hit sound fx's
	explode = al_load_sample("res/explode2.ogg"); // Explode sound fx
	missile_fx = al_load_sample("res/missile.ogg"); // Missile sound fx
	font = al_load_font("res/8bit2.ttf", 18, 0);
}

ship::~ship()
{
	for (int i = 0; i < maxFrame; i++)
	{
		al_destroy_bitmap(image[i]); // Clean up all images
	}
	player_bullets.clear();
	draw_ptr.clear();
	move_ptr.clear();
	al_destroy_font(font);
	al_destroy_sample(shieldhit);
	al_destroy_sample(doubleblaster);
	al_destroy_sample(thruster);
	al_destroy_sample(missile_fx);
	al_destroy_sample(explode);
}

void ship::paint()
{
	if (_isAlive == true)
	{
		if (isThruster || isWarping || isReverse)
		{
			if (_isFiring)
			{
				al_draw_bitmap(image[3], _x, _y, 0); // Thrusters image + firing

			}
			else al_draw_bitmap(image[1], _x, _y, 0); // Thrusters image
		}
		else
		{
			if (_isFiring)
			{
				al_draw_bitmap(image[2], _x, _y, 0); // No Thrusters + firing

			}
			al_draw_bitmap(image[0], _x, _y, 0); // Without thrusters image
		}
	
		if (colourCycle == true) // If collision with bullet, then paint shields
		{
			
			if (_shields > 90 && _shields <= 100)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(0, 150, 255, 60), 3);
			}
			else if(_shields > 80 && _shields <= 90)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(0, 84, 255, 60), 3); 
			}
			else if(_shields > 70 && _shields <= 80)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(0, 18, 255, 60), 3);
			}
			else if(_shields > 60 && _shields <= 70)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(54, 0, 255, 60), 3);
			}
			else if(_shields > 50 && _shields <= 60)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(150, 0, 255, 60), 3);
			}
			else if(_shields > 40 && _shields <= 50)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(234, 0, 255, 60), 3); 
			}
			else if(_shields > 30 && _shields <= 40)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(255, 0, 222, 60), 3); 
			}
			else if(_shields > 20 && _shields <= 30)
			{
			
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(255, 0, 156, 60), 3); 
			}
			else if(_shields > 10 && _shields <= 20)
			{
				
				al_draw_circle(_x + 32, _y + 32, 40, al_map_rgba(255, 0, 12, 60), 3); 
			}
			//colourCycle = false;
		}
		
		std::vector<std::shared_ptr<drawable>>::iterator it; 
		for (it = draw_ptr.begin(); it != draw_ptr.end(); it++){
			(*it)->paint(); // Draw the bullets onto the screen.
		}
		// Particles
		std::vector<std::shared_ptr<particle>>::iterator p;
		for (p = thruster_fx.begin(); p != thruster_fx.end(); p++)
		{
			(*p)->paint(); // Paint the particle fx
		}
		
		_isFiring = false;
	}
}
void ship::render()
{
	if (_isAlive == true)
	{
		// Shield regeneration 
		if (_shields <=  _shieldMax)
		{
			_shields += 0.3f;
			
		}
		
		
		// Speed and velocity --------------------------------------------------------------------------------
		float decelerate = 0.04f; // If the xVelocity is > 0, the thrusters will decelerate until it reaches 0.
		float accelerate = 0.03f; // If the xVelocity is < 0, the thrusters boost to until it reaches 0.
		float trigger = 0.02f; // Will engage thrusters if xVel > or < trigger (lower is better)
		
		// This will make sure we stay on the screen.
		if (_x <= 0)
		{
			xVelocity = 0;
			_x += 5;
		}
		else if (_x + 64 > ScreenWidth)
		{
			xVelocity = 0;
			_x -= 5;
		}
		if (_y <= 0)
		{
			yVelocity = 0;
			_y += 5;
		}
		else if (_y + 64 > ScreenHeight)
		{
			yVelocity = 0;
			_y -= 5;
		}
		
		// Changes the thruster state, on or off. This will change the animation of the sprite.
		if (xVelocity > trigger * 15 || yVelocity > trigger * 15 || yVelocity < -trigger * 15)
		{
			isThruster = true;
		}
		else if (xVelocity < -trigger * 15)
		{
			isReverse = true;
		}
		else
		{
			isThruster = false;
			isReverse = false;
		}	

		// Particles 
		if (isThruster || isWarping) // If the ship is using thrusters or warping use particle fx
		{
			if (!isReverse)
			{
				auto t = std::make_shared<particle>(); 
				t->initParticles(_x + 14, _y + 29, 2, 1, 1); // Type 1 for thruster effects
				thruster_fx.push_back(t);
				auto t1 = std::make_shared<particle>(); 
				t1->initParticles(_x+ 14, _y + 35, 2, 1, 1); 
				thruster_fx.push_back(t1);
				auto t2 = std::make_shared<particle>(); 
				t2->initParticles(_x + 14, _y + 29, 2, 5, 1); // Type 1 for thruster effects
				thruster_fx.push_back(t2);
				auto t3 = std::make_shared<particle>(); 
				t3->initParticles(_x+ 14, _y + 35, 2, 5, 1); 
				thruster_fx.push_back(t3);
			}
			else 
			{
					auto t = std::make_shared<particle>(); 
					t->initParticles(_x + 14, _y + 29, 1, 1, 1); // Type 1 for thruster effects
					thruster_fx.push_back(t);
					auto t1 = std::make_shared<particle>(); 
					t1->initParticles(_x+ 14, _y + 35, 1, 1, 1); 
					thruster_fx.push_back(t1);
			}
		}
		
		// Auto-thrusters (decrease or increase velocity when controls are not active) trigger = when they activate
		if (xVelocity > trigger)
		{
			
			xVelocity -= decelerate;
		}
		if(xVelocity < trigger)
		{
			
			xVelocity += accelerate;
		}
		if (yVelocity > trigger)
		{ 	
			
			yVelocity -= decelerate;
		}
		if(yVelocity < trigger)
		{
			
			yVelocity += accelerate;
		}
		
		
		// Rendering ship here
		
		_x += xVelocity;
		_y += yVelocity;   
		
		// Bullets
		std::vector<std::shared_ptr<moveable>>::iterator it;
		for (it = move_ptr.begin(); it != move_ptr.end(); it++){
			(*it)->render(); // Move the bullets on the screen.
		}
		
		// Render the particles
		std::vector<std::shared_ptr<particle>>::iterator p;
		for (p = thruster_fx.begin(); p != thruster_fx.end(); p++)
		{
			(*p)->render(); // Render the particle fx
		}
	}
}

void ship::controls(ALLEGRO_EVENT& ev, ALLEGRO_JOYSTICK_STATE& joystate, ALLEGRO_TIMER* timer, int& gamestate)
{
	// Thruster Sound FX
	if ((isThruster || isWarping || isReverse) && thrusterFx == false)
	{
		al_play_sample(thruster, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_LOOP,&thrusterID);
		thrusterFx = true;
	}
	else if (isThruster == false && isReverse == false && !isWarping && thrusterFx == true)
	{
		al_stop_sample(&thrusterID);
		thrusterFx = false;
	}
  
	if (ev.type == ALLEGRO_EVENT_KEY_UP)
	{
		switch (ev.keyboard.keycode)
		{
			case ALLEGRO_KEY_P:
				 if (!pause){
					al_draw_text(font, al_map_rgb(0, 255, 0), ScreenWidth/ 2  , ScreenHeight/2 - 50, ALLEGRO_ALIGN_CENTER,"PAUSE");
					al_flip_display();
					al_stop_sample(&thrusterID);
					al_stop_timer(timer);
					pause = true;
					
					}
					else{
					al_start_timer(timer);
					pause = false;
				}
				break;
		}
	}
	else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		float modifier = 1.25f;
		
		switch (ev.keyboard.keycode)
		{
			case ALLEGRO_KEY_UP:
				yVelocity -= modifier;
				break;
			case ALLEGRO_KEY_DOWN:
				yVelocity += modifier;
				break;
			case ALLEGRO_KEY_LEFT:
				xVelocity -= modifier;
				break;
			case ALLEGRO_KEY_RIGHT:
				xVelocity += modifier;
				break;
			case ALLEGRO_KEY_SPACE:
				fireBlaster();
				break;
			case ALLEGRO_KEY_F:
				fireMissile();
				break;
		}
	}
	else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
	{
		// JOYSTICK BUTTONS ------------------------------------------------------------------------------------
		// L2 8; R2 9
		// L1 10; R1 11
		// UP 4; LEFT 7; DOWN 6; Right 5
		// Square 15; X 14; Cicle 13; Triangle 12
		// Select 0; Start 3        
		// L3 1; R3 2
	  
		float modifier = 1.25f;
		
		
		// Digital Pad ++++++++++++++++++++++++++++++++++++
		if (joystate.button[4])
		{ // D-UP
			yVelocity -= modifier;
		}
		else if(joystate.button[7])
		{ // D-LEFT
			xVelocity -= modifier;
		}
		else if(joystate.button[6])
		{ // D-DOWN
			yVelocity += modifier;
		}
		else if(joystate.button[5])
		{ // D-RIGHT
			xVelocity += modifier;
		}
		// Joystick Buttons +++++++++++++++++++++++++++++++
		else if (joystate.button[15])
		{ // Square Button
			
		}
		else if(joystate.button[14])
		{ // X Button
			fireBlaster();
			
		}
		else if(joystate.button[13])
		{ // Circle Button
			fireMissile();
		}
		else if(joystate.button[12])
		{ // Triangle Button
		  
		}
		else if(joystate.button[8])
		{ // L2
			xVelocity -= modifier;
		}
		else if(joystate.button[9])
		{ // R2
			xVelocity += modifier;
		}
		else if(joystate.button[11])
		{ // R1
			fireBlaster();
		}
		else if(joystate.button[3])
		{ // Start button
			if (!pause)
			{
				al_stop_sample(&thrusterID);
				al_draw_text(font, al_map_rgb(0, 255, 0), ScreenWidth/ 2  , ScreenHeight/2 - 50, ALLEGRO_ALIGN_CENTER,"PAUSE");
				al_flip_display();
				al_stop_timer(timer);
				pause = true;
			
			}
			else{
				al_start_timer(timer);
				pause = false;
			}
		}
	}
	else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)
	{
		if (joystate.button[4])
		{ // D-UP
			// Not implemented
		}
		else if(joystate.button[7])
		{ // D-LEFT
			// Not implemented
		}
		else if(joystate.button[6])
		{ // D-DOWN
			// Not implemented
		}
		else if(joystate.button[5])
		{ // D-RIGHT
			// Not implemented
		}
	}
	else if (ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
	{	
		// JOYSTICK MOVEMENT ------------------------------------------------------------------------------------
		float threshold = 0.1f; // Threshold until analog sticks activate
		float modifier = 10; // Converts axis input from analog sticks 
		
		// LEFT ANALOG STICK
		if (joystate.stick[0].axis[0] > threshold || joystate.stick[0].axis[0] < -threshold) // LEFT STICK X AXIS
		{
			if (joystate.stick[0].axis[0] > threshold) // ANALOG-RIGHT
			{
				xVelocity += joystate.stick[0].axis[0] / modifier;
			}
			else if(joystate.stick[0].axis[0] < -threshold) // ANALOG-LEFT
			{
				xVelocity += joystate.stick[0].axis[0] / modifier;
			}
		}
		if (joystate.stick[0].axis[1] > threshold || joystate.stick[0].axis[1] < -threshold) // LEFT STICK Y AXIS
		{ 
			if (joystate.stick[0].axis[1] > threshold) // ANALOG-DOWN
			{
				yVelocity += joystate.stick[0].axis[1] / modifier;
			}
			else if(joystate.stick[0].axis[1] < -threshold) // ANALOG-UP
			{
				yVelocity += joystate.stick[0].axis[1] / modifier;
			}
		}
		
		// RIGHT ANALOG STICK
		if (joystate.stick[1].axis[0] > threshold || joystate.stick[1].axis[0] < -threshold) // LEFT STICK X AXIS
		{
			if (joystate.stick[1].axis[0] > threshold) // ANALOG-RIGHT
			{
				xVelocity += joystate.stick[1].axis[0] / modifier;
			}
			else if(joystate.stick[1].axis[0] < -threshold) // ANALOG-LEFT
			{
				xVelocity += joystate.stick[1].axis[0] / modifier;
			}
		}
		if (joystate.stick[1].axis[1] > threshold || joystate.stick[1].axis[1] < -threshold) // LEFT STICK Y AXIS
		{ 
			if (joystate.stick[1].axis[1] > threshold) // ANALOG-DOWN
			{
				yVelocity += joystate.stick[1].axis[1] / modifier;
			}
			else if(joystate.stick[1].axis[1] < -threshold) // ANALOG-UP
			{
				yVelocity += joystate.stick[1].axis[1] / modifier;
			}
		}
	}
}
void ship::fireBlaster()
{
	_blasterDmg = rand() % 15 + 5; // Blaster damage randomizer
	_isFiring = true;
	al_play_sample(doubleblaster, 0.8f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&doubleblasterID);
	// Top gun
	auto ptr = std::make_shared<bullet>(_x + 41, _y + 21);
	move_ptr.push_back(ptr);
	draw_ptr.push_back(ptr);
	player_bullets.push_back(ptr);
	// Bottom gun
	auto ptr2 = std::make_shared<bullet>(_x + 41, _y + 41);
	move_ptr.push_back(ptr2);
	draw_ptr.push_back(ptr2);    
	player_bullets.push_back(ptr2);
}

void ship::fireMissile()
{
	al_play_sample(missile_fx, 0.8f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&missileID);
	
	if (bottom)
	{
		auto m = std::make_shared<missile>(_x + 38.6,  _y + 43.7, false);
		move_ptr.push_back(m);
		draw_ptr.push_back(m);
		player_missiles.push_back(m);
		bottom = false;
	}
	else 
	{
		auto m1 = std::make_shared<missile>(_x + 38.6, _y + 20.4, true);
		move_ptr.push_back(m1);
		draw_ptr.push_back(m1);
		player_missiles.push_back(m1);	
		bottom = true;
	}
	
}

void ship::takeDamage(float dmg)
{
	if (_isAlive)
	{
		if (_shields > 0) // If the shields are > 0, absorb damage, else take damage directly.
		{
			colourCycle = true;
				
			_shields -= dmg; // If shields active, take hit on shields.
			al_play_sample(shieldhit, 0.6f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&shieldID);
		}
		
		if (_shields <= 10 && _health > 0)
		{
			if (_shields > _shieldMin)
			{
				_shields -= dmg; // This will disable the shields, taking additional damage until they reach _shieldMin.
			}
			_x--;
			_health -= dmg; // Take damage to ship
		}
		
		
		if (_health <= 0)
		{
			// Die
			al_play_sample(explode, 1.5f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&explodeID); // Play explosion sound fx
			al_stop_sample(&thrusterID); // Kill thruster sound fx
			_isAlive = false;
			std::cout << "You are dead." << std::endl;
		}
	}
}

void ship::resetVar()
{
	// Resets a few variables so that we can start a new game using the same ship object.
	player_bullets.clear();
	player_missiles.clear();
	thruster_fx.clear();
	draw_ptr.clear();
	move_ptr.clear();
	_isAlive = true;
	// Starting x and y coordinates for the ship
	_x = 5;
	_y = ScreenHeight / 2;
	_health = 100; // Health Points
	_shields = 100; // Shield max
	
}
