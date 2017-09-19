/*
 * ship.h
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

#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <vector>
#include <memory>
#include <math.h>  

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "drawable.h"
#include "moveable.h"
#include "bullet.h"
#include "missile.h"
#include "gamemanager.h"
#include "particle.h"

class gamemanager;

/// Player object
class ship : public drawable, public moveable{
	public:
		/// Ctor
		ship(); 
		/// Dtor
		virtual ~ship(); 
		/// Paint function
		void paint(); 
		/// Rendering function
		void render(); 
		/// This function will read our controls from keyboard or joystick input. It will update our movements or read button presses
		void controls(ALLEGRO_EVENT& ev,ALLEGRO_JOYSTICK_STATE& joystate, ALLEGRO_TIMER* timer, int& gamestate);
		/// Returns X coordinate
		float getX(){ return _x;}
		/// Returns Y coordinate
		float getY(){ return _y;}
		/// Blaster damage points
		float getBlasterDmg(){ return _blasterDmg;} 
		/// Missile damage points
		float getMissileDmg(){ return _missileDmg;}
		/// Return the health points of the ship
		float getHealth() {return _health;} 
		/// Return the shield amount
		float getShields(){return _shields;} 
		/// Returns if the player is alive
		bool getIsAlive(){return _isAlive;} 
		/// Vector of bullets
		std::vector<std::shared_ptr<bullet>> player_bullets; 
		/// Vector of missiles
		std::vector<std::shared_ptr<missile>> player_missiles;
		/// This function will impact either shield or health points with dmg value
		void takeDamage(float dmg);
		/// Clear the bullet vectors and get ready for next stage.
		void resetShip(){player_bullets.clear(); thruster_fx.clear(); player_missiles.clear(); draw_ptr.clear(); move_ptr.clear();} 
		/// Reset the ship variables
		void resetVar();
		/// Boost heath from item
		void boostHealth(int amt){if (_health <= 75) _health += amt; else if(_health > 75 && _health + amt < 100) _health += amt; else _health = 100;}
		/// Will display image with thruster if warping to next level.
		bool isWarping; 
	private:
		/// Vector of particles for effects
		std::vector<std::shared_ptr<particle>> thruster_fx;
		/// How many ship images we will store
		static const int maxFrame = 4; 
		/// This is the array of bitmap ship images
		ALLEGRO_BITMAP *image[maxFrame]; 
		/// Starting X and Y coordinates for the ship
		float _x, _y; 
		/// How big the ship is width/height
		int _width, _height; 
		/// Health points
		float _health; 
		/// Max value for shields
		float _shieldMax; 
		/// Min value for shileds
		float _shieldMin; 
		/// Shield points
		float _shields; 
		/// The max velocity the ship can move
		float _maxVelocity; 
		/// Blaster damage points
		float _blasterDmg;
		/// Missile damage points
		float _missileDmg;
		/// Is the ship currently firing a blaster (will change the image of the ship)
		bool _isFiring;
		/// Is the ship alive or dead
		bool _isAlive;
		/// For the shield colour cycle
		bool colourCycle; 
		/// This will alternate firing of missiles from top/bottom of the ship. False = bottom.
		bool bottom;
		/// ID for double blaster sound fx
		ALLEGRO_SAMPLE_ID doubleblasterID; 
		/// ID for the missile launcher
		ALLEGRO_SAMPLE_ID missileID;
		/// ID for thruster sound fx
		ALLEGRO_SAMPLE_ID thrusterID; 
		/// ID for shield sound fx
		ALLEGRO_SAMPLE_ID shieldID; 
		/// ID for explode sound fx
		ALLEGRO_SAMPLE_ID explodeID; 
		/// explode sound fx
		ALLEGRO_SAMPLE *explode = NULL;
		/// double blaster sound fx
		ALLEGRO_SAMPLE *doubleblaster = NULL; 
		/// missile sound fx
		ALLEGRO_SAMPLE *missile_fx = NULL;
		/// Thruster sound fx
		ALLEGRO_SAMPLE *thruster = NULL; 
		/// Shield hit sound fx
		ALLEGRO_SAMPLE *shieldhit = NULL; 
		/// Shield activated sound fx
		ALLEGRO_SAMPLE *shieldactivated = NULL;
		/// Drawable weapons
		std::vector<std::shared_ptr<drawable>> draw_ptr; 
		/// Moveable weapons
		std::vector<std::shared_ptr<moveable>> move_ptr; 
		/// Pause variable
		bool pause;
		/// Font, used only with PAUSE
		ALLEGRO_FONT *font = NULL; 
		/// If it is moving up, down or forward flag
		bool isThruster = false;
		/// If it is moving up, down or forwards it will use this boolean to play thruster sound fx
		bool thrusterFx = false;
		/// If it is moving backwards flag
		bool isReverse = false;
		/// Truster modifier for acceleration/deceleration
		float thrust; 
		/// How fast up or down, left or right
		float xVelocity, yVelocity; 
		/// Fire blaster function
		void fireBlaster();
		/// Fire missile function
		void fireMissile();
};


#endif /* SHIP_H */ 
