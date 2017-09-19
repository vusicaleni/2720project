/*
 * gamemanager.h
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


#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <memory>
#include <vector>
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h> 
#include <ctime>

#include "global.h"
#include "allegro.h"
#include "ship.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_bullet.h"
#include "missile.h"
#include "boss.h"
#include "particle.h"
#include "item.h"

class ship;

/// Game state enumeration
enum {
	/// We will set gamestate to any one of these states.
	SplashScreen = 1, 
	StartGame,
	EndGame,
	WinGame
};
/// Menu state enumeration
enum {
	NewGame = 1, // Start Game is selected
	Options,
	HiScore,
	Exit,
	SoundFx,
	Music,
	OptionsBack,
	SoundFxOn,
	SoundFxOff,
	SoundFxBack,
	MusicOn,
	MusicOff,
	MusicBack,
	Continue
};

/// Background image structure
struct Background{
	/// Background X location
	float x; 
	/// Background Y location
	float y; 
	/// X velocity of the scroll
	float velX; 
	/// Y velocity of the scroll
	float velY; 
	/// X direction, for example: West & East
	int dirX;
	/// Y direction, for example: North & South
	int dirY; 
	/// Width of the background bitmap image
	int width; 
	/// Height of the background bitmap image
	int height; 
	/// This bool will only paint BG during background if set true
	bool isBG;
	/// Background bitmap image
	ALLEGRO_BITMAP *image; 
};

/// Game engine, the heart of our program.
class gamemanager
{
	public:
		/// Ctor
		gamemanager();
		/// Dtor
		~gamemanager();
		/// Initialize the game
		void gameInit(); 
	private:
		/// Pointer to the allegro class we made 
		std::shared_ptr<allegro> allegro_ptr; 
		/// Vector of enemies
		std::vector<std::shared_ptr<enemy>> enemies; 
		/// Vector of end level boss
		std::vector<std::shared_ptr<boss>> endboss; 
		/// Vector of particles for effects
		std::vector<std::shared_ptr<particle>> particle_fx;
		/// Vector for items
		std::vector<std::shared_ptr<item>> item_collection;
		/// Display object
		ALLEGRO_DISPLAY* screen;
		/// Event queue
		ALLEGRO_EVENT_QUEUE *event_queue = NULL;
		/// Timer
		ALLEGRO_TIMER *timer = NULL;
		/// Joystick, for PS3 controller input
		ALLEGRO_JOYSTICK *joy;
		/// Background
		ALLEGRO_BITMAP *bgImage = NULL; 
		/// Middle-ground
		ALLEGRO_BITMAP *mgImage = NULL; 
		/// Foreground
		ALLEGRO_BITMAP *fgImage = NULL;
		/// Fade image and value
		ALLEGRO_BITMAP *fade = NULL;
		float fadeValue = 200.0f; // Higher = black screen
		/// Fonts that we use in our game
		ALLEGRO_FONT *font = NULL;
		ALLEGRO_FONT *font_title = NULL;
		ALLEGRO_FONT *font_ui = NULL;
		/// ID for sound fx
		ALLEGRO_SAMPLE_ID ID; 
		/// Song instance will control the background music
		ALLEGRO_SAMPLE_INSTANCE *songInstance = NULL;
		/// Audio samples
		ALLEGRO_SAMPLE *sample_infinity = NULL;
		ALLEGRO_SAMPLE *sample_devine = NULL;
		ALLEGRO_SAMPLE *sample_infinity2a = NULL;
		ALLEGRO_SAMPLE *sample_infinity2b = NULL;
		ALLEGRO_SAMPLE *sample_battle = NULL;
		ALLEGRO_SAMPLE *sample_brothers = NULL;
		ALLEGRO_SAMPLE *sample_lostkingdom = NULL;
		ALLEGRO_SAMPLE *sample_overclocked = NULL;
		ALLEGRO_SAMPLE *sample_outcome = NULL;
		ALLEGRO_SAMPLE *sample_pixelrain = NULL;
		ALLEGRO_SAMPLE *sample_rushing = NULL;
		/// Sound effects
		ALLEGRO_SAMPLE *menu_down = NULL;
		ALLEGRO_SAMPLE *menu_up = NULL;
		ALLEGRO_SAMPLE *menu_select = NULL;
		ALLEGRO_SAMPLE *boom = NULL; // Missile explosion sound fx
		/// Name of game
		const char* GameName;
		/// Player score
		int _score; 
		/// Keeps track of the current level. Also will be incremented to proceed to next level.
		int levelCount; 
		/// Warp counter -- basically just delays the level load between planets
		int warpCounter;
		/// Whether we are currently warping or not.
		bool isWarp;
		/// The game state ex, SplashScreen, Menu, NewGame etc
		int gameState; 
		/// The menu state. Changes the menu depending on the selection. Will redraw the menu on the screen.
		int menuState;
		/// How many enemies will spawn in the game
		int enemyAmount;
		/// End counter will delay the next scene of our game by a set value
		int endCounter;
		/// Int variable to delay boss death animation. When it reaches 200, change gamestate to EndGame
		int bossCounterDelay;
		/// How many enemies are isAlive
		int enemyCounter = 0;
		/// How many bosses are isAlive
		int bossCounter = 0;
		/// Main boolean controlling the game
		bool done; 
		/// This variable is only used when the enemies are defeated and the boss battle begins or stops
		bool bossBattle; 
		/// Redraw bool for updating screen
		bool redraw;
		/// Toggles music
		bool musicOn;
		/// Toggles sound
		bool soundOn; 
		/// Stops the scrolling feature of the background.
		bool stopScroll; 
		/// Shows debug info on screen
		bool debug;
		/// Adds a controller or joystick
		void addController(ALLEGRO_JOYSTICK *j,  int number);
		/// Main game loop, this is where the majority of the stuff happens. It will control/monitor our gamestate too.
		void mainLoop(); 
		/// SplashScreen loop
		void introLoop(); 
		/// Menu loop
		void menuLoop(); 
		/// Changes the menu selection variable
		void menuSelection();  
		/// This function will redraw the menu depending on the selection
		void changeMenuSelection(); 
		/// Load new level. 1, 2, 3 etc. We will most likely use levelCount
		void loadLevel(int lv); 
		/// This will initialize the background image. We can pass in BG, MG and FG with different variables. 
		void initBackground(Background &backg, float x, float y, float velx, float vely, int width, int height, int dirx, int diry, bool bg, ALLEGRO_BITMAP *image);
		/// Scroll background just once instead of loop
		void updateBackground(Background &backg); 
		/// Scroll background in a loop
		void updateBackgroundScroll(Background &backg); 
		/// Paint the background
		void paintBackground(Background &backg); 
		/// Initializes the sample instance
		void initMusic(ALLEGRO_SAMPLE* s); 
		/// Plays music according to level
		void playMusic(int lv); 
		/// Load up the vector of enemies
		void loadEnemies(); 
		/// Loads the boss in accordance to the level
		void loadBoss(int lv); 
		/// Check the status of isAlive on enemies
		void checkEnemyStatus(std::vector<std::shared_ptr<enemy>> enemies); 
		/// Checks for enemy bullet collisions with player
		void checkPlayerCollision(ship& ship, std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b); 
		/// Checks for player bullet collisions with enemies
		void checkEnemyCollision(ship& ship, std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b); 
		/// Renders all items
		void renderAllItems(std::vector<std::shared_ptr<item>> items, ship& ship);
		/// Paints all items
		void paintAllItems(std::vector<std::shared_ptr<item>> items);
		/// Renders all of the enemies + boss
		void renderAllEnemies(std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b); 
		/// Paints all of the enemies + boss
		void paintAllEnemies(std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b); 
		/// Renders all the particles for the particle fx
		void renderParticles(std::vector<std::shared_ptr<particle>> particles);
		/// Paints particles for particle fx
		void paintParticles(std::vector<std::shared_ptr<particle>> particles);
		/// Paints the User Interface - Score, Shields, HP
		void paintUI(ship& ship); 
		/// Resets the variables and dumps the collections for a new game
		void resetGame();
		/// Warp -- this is the warp cutscene
		void warp();
		/// Background 
		Background BG; 
		/// Middle-ground
		Background MG;
		/// Foreground
		Background FG; 
};	

#endif /* GAMEMANAGER_H */ 
