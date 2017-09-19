/*
* gamemanager.cpp
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

#include "gamemanager.h"


gamemanager::gamemanager()
{
	// Ctor
	GameName = "CORE COLLAPSE";
	srand (static_cast <unsigned> (time(0))); // Seed randomizer
	musicOn = true; // Toggle music -- on or off
	soundOn = true; // Toggle sound -- on or off

	// Init
	levelCount = 0; // Starting level
	_score = 0;
	bossBattle = false;
	endCounter = 0;
	
	gameState = 0;
	menuState = 0;
	
	warpCounter = 0;
	isWarp = false;
	
	// For Debug
	debug = false;
}

gamemanager::~gamemanager()
{
	// Dtor
	//delete player1;
	al_destroy_display(screen);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_uninstall_joystick();
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(mgImage);
	al_destroy_bitmap(fgImage);
	al_destroy_sample_instance(songInstance);
	al_destroy_sample(sample_devine);
	al_destroy_sample(sample_infinity2b);
	al_destroy_sample(sample_infinity2a);
	al_destroy_sample(sample_infinity);
	al_destroy_sample(sample_battle);
	al_destroy_sample(sample_brothers);
	al_destroy_sample(sample_lostkingdom);
	al_destroy_sample(sample_overclocked);
	al_destroy_sample(sample_pixelrain);
	al_destroy_sample(sample_outcome);
	al_destroy_sample(sample_rushing);
	al_destroy_sample(menu_up);
	al_destroy_sample(menu_down);
	al_destroy_sample(menu_select);
	al_destroy_sample(boom);
	al_destroy_font(font);
	al_destroy_font(font_title);
	al_destroy_font(font_ui);
	al_destroy_bitmap(BG.image);
	al_destroy_bitmap(MG.image);
	al_destroy_bitmap(FG.image);
	al_destroy_bitmap(fade);
	enemies.clear();
	endboss.clear();
	particle_fx.clear();
	item_collection.clear();
}

void gamemanager::gameInit()
{
	
	/// Install Allegro 5 libraries etc.
	allegro_ptr->initAllegro(); // Initialize Allegro 5
	allegro_ptr->initDisplay(screen, ScreenWidth, ScreenHeight); // Initialize display
	allegro_ptr->initSound(); // Initialize sound
	allegro_ptr->initAddons(); // Initialize Allegro 5 addons
	allegro_ptr->initControls(); // Install keyboard/joystick
	

	
	/// Sound
	al_reserve_samples(15); // Reserve around 15 samples, should be enough
	/// Load a few samples
	menu_up = al_load_sample("res/menu_up.ogg"); 
	menu_down = al_load_sample("res/menu_down.ogg"); 
	menu_select = al_load_sample("res/menu_select.ogg"); 
	boom = al_load_sample("res/boom.ogg");
	
	sample_pixelrain = al_load_sample("res/pixelrain.ogg"); // Load the first song sample
	sample_battle = al_load_sample("res/battle.ogg"); // Load the battle music
	sample_rushing = al_load_sample("res/rushing.ogg"); // Load the warp music
	sample_lostkingdom = al_load_sample("res/lostkingdom.ogg"); // LV1 Music
	sample_infinity2a = al_load_sample("res/infinity2a.ogg"); // LV2 Music
	sample_infinity2b = al_load_sample("res/infinity2b.ogg"); // LV3 Music
	sample_brothers = al_load_sample("res/brothers.ogg"); // LV4 Music
	sample_infinity = al_load_sample("res/infinity.ogg"); // LV5 Music
	sample_overclocked = al_load_sample("res/overclocked.ogg"); // LV6 Music
	
	/// Font
	font = al_load_font("res/8bit2.ttf", 18, 0);
	font_title = al_load_font("res/DeadSpace.ttf", 110, 0);
	font_ui = al_load_font("res/8bit2.ttf", 8, 0);
	
	/// Timer / Events / Queue
	event_queue = al_create_event_queue(); // Initialize queue
	timer = al_create_timer(1.0 / FPS); // Timer info linked to fps
	
	
	///al_register_event_source(event_queue, al_get_display_event_source(screen));
	al_register_event_source(event_queue, al_get_keyboard_event_source()); // Register keyboard to the queue
	al_register_event_source(event_queue, al_get_timer_event_source(timer)); // Register events to timer
	al_register_event_source(event_queue, al_get_joystick_event_source()); // Register joystick events
	
	fade = al_load_bitmap("res/fade.png"); // Load the fade image
	
	al_start_timer(timer); // Start timer
	
	/// Controller input
	try
	{
		// Try to find the joystick 
		joy = al_get_joystick(0);
		if (joy == NULL) throw 0;
		std::cout << "※ Controller added." << std::endl;
	}
	catch(int n)
	{
		std::cout << "※ Controller " << n << " was not found." << std::endl;
	}
	
	gameState = SplashScreen; // Set gameState to splashscreen
	
	mainLoop();// Start Main mainLoop
}

void gamemanager::initMusic(ALLEGRO_SAMPLE* s)
{
	// Reinitialize the sample required for the background sound, not sound effects.
	songInstance = al_create_sample_instance(s);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
	al_play_sample_instance(songInstance);
}

void gamemanager::playMusic(int lv){
	// How to pick the music sample
	if (musicOn)
	{
		switch (lv)
		{
			case 0:
				initMusic(sample_pixelrain); // Splash screen / menu music
				break;
			case 1:
				initMusic(sample_lostkingdom); // Level music 1
				break;
			case 2: 
				initMusic(sample_infinity2a); // Level music 2
				break;
			case 3: 
				initMusic(sample_infinity2b); // Level music 3
				break;
			case 4:
				initMusic(sample_brothers); // Level music 4
				break;
			case 5:
				initMusic(sample_infinity); // Level music 5
				break;
			case 6:
				initMusic(sample_overclocked); // Level music 6
				break;
			case 7:
				initMusic(sample_devine); // Game over music
				break;
			case 8:
				initMusic(sample_outcome); // Win music
				break;
			case 9: 
				initMusic(sample_battle); // Boss battle music
				break;
			case 10:
				initMusic(sample_rushing); // Warp music
				break;
		}
	}
}

void gamemanager::initBackground(Background &backg, float x, float y, float velx, float vely, int width, int height, 
								int dirx, int diry, bool bg, ALLEGRO_BITMAP *image)
{
	// Scrolling background variable assignment
	backg.x = x; // X Location
	backg.y = y; // Y Location
	backg.velX = velx; // X velocity for scrolling
	backg.velY = vely; // Y velocity if scrolling vertically
	backg.width = width; // width of the bitmap
	backg.height = height; // height of the bitmap
	backg.dirX = dirx; // dirx and diry will control the direction of the scroll
	backg.dirY = diry;
	backg.isBG = bg; // Is it a background that will paint during warp
	backg.image = image; // background image bitmap
}

void gamemanager::updateBackground(Background &backg)
{
	// HORIZONTAL SCROLL, stop once it has fully traversed the screen from right to left.
	if (!isWarp)
	{
		if (!stopScroll) backg.x += backg.velX * backg.dirX; // scroll
	
		if(backg.x + backg.width <= 1440)
		{ 
			stopScroll = true; // stop scroll when the image has moved from left to right.
		}
	}
	else backg.x += backg.velX * backg.dirX;
	
}

void gamemanager::updateBackgroundScroll(Background &backg)
{
	// HORIZONTAL SCROLL, Scroll forever
	backg.x += backg.velX * backg.dirX; // scroll
	if(backg.x + backg.width <= 0)
	{
		backg.x = 0; // reset stars background.
	}
}

void gamemanager::paintBackground(Background &backg)
{
	// Background paint
	al_draw_bitmap(backg.image, backg.x, backg.y, 0);
	if (backg.isBG) // Loop background paint scroll.
	{
		if(backg.x + backg.width <= ScreenWidth)
		{
			al_draw_bitmap(backg.image, backg.x + backg.width, backg.y, 0);
		}
	}
	
}

void gamemanager::loadEnemies()
{
	float startX = ScreenWidth + 200; // Start x for enemies, larger than ScreenWidth at least.
	enemyAmount = 50; // This is how many enemies will spawn
	
	// Load the enemies into the vector
	
	for (int i = 0; i < enemyAmount; i++)
	{
		auto ptr = std::make_shared<enemy>(rand() % 1800 + startX,rand() % 656 + 1);
		enemies.push_back(ptr);
	}
}

void gamemanager::loadBoss(int lv)
{
	/// This function will load a boss into the endboss vector based on integer lv (current level).
	
	if (bossBattle == false)
	{
		bossBattle = true;
		al_stop_sample_instance(songInstance);
		playMusic(9);
		if (lv == 1)
		{
			auto boss1 = std::make_shared<boss>(ScreenWidth - 64, ScreenHeight/2, 1);
			endboss.push_back(boss1);
		}
		else if(lv == 2)
		{
			auto boss2 = std::make_shared<boss>(ScreenWidth - 64, ScreenHeight/2, 2);
			endboss.push_back(boss2);
		}
		else if (lv == 3)
		{
			auto boss3 = std::make_shared<boss>(ScreenWidth - 64, ScreenHeight/2, 3);
			endboss.push_back(boss3);
		}
		else if (lv == 4)
		{
			auto boss4 = std::make_shared<boss>(ScreenWidth - 64, ScreenHeight/2, 4);
			endboss.push_back(boss4);
		}
		else if (lv == 5)
		{
			auto boss5 = std::make_shared<boss>(ScreenWidth - 64, ScreenHeight/2, 5);
			endboss.push_back(boss5);
		}
		else if (lv == 6)
		{
			auto boss6 = std::make_shared<boss>(ScreenWidth - 64, ScreenHeight/2, 6);
			endboss.push_back(boss6);
		}
	}
	

}

void gamemanager::loadLevel(int lv)
{
	/// This function loads the level/increments to the next level. Integer lv indicates the current level. *Usually we pass levelCount into this function
	// Clear out the vectors and prepare for level loading
	enemies.clear();
	endboss.clear();
	particle_fx.clear();
	item_collection.clear();
	stopScroll = false; // reset the variable to make sure the background scrolls.
	
	//bgImage = NULL; 
	//mgImage = NULL; 
	//fgImage = NULL; 
	
	if (lv == 0)
	{
		levelCount = 1; // Increase the level from 0 to 1
		if (musicOn) 
		{
		
		al_stop_sample_instance(songInstance);
		playMusic(levelCount);
		}
	
		// Background
		bgImage = al_load_bitmap("res/stars.png"); // Initialize image for background
		initBackground(BG, 0, 0, 0.15f, 0, 3840, ScreenHeight, -1, 0, true, bgImage); // Initialize the scrolling background
		mgImage = al_load_bitmap("res/earth.png"); // Initialize image for the middleground
		initBackground(MG, 0, 0, 0.25f, 0, 3840, ScreenHeight, -1, 0, false, mgImage); // Initialize the scrolling background
		
		loadEnemies();
	}
	else if (lv == 1)
	{
		levelCount = 2; // Increase the level 
		if (musicOn) 
		{
		
		al_stop_sample_instance(songInstance);
		playMusic(levelCount);
		}
	
		// Background
		bgImage = al_load_bitmap("res/stars.png"); // Initialize image for background
		initBackground(BG, 0, 0, 0.15f, 0, 3840, ScreenHeight, -1, 0, true, bgImage); // Initialize the scrolling background
		mgImage = al_load_bitmap("res/mars.png"); // Initialize image for the middleground
		initBackground(MG, 0, 0,0.25f, 0, 3840, ScreenHeight, -1, 0, false, mgImage); // Initialize the scrolling background
		
		loadEnemies();
	}
	else if (lv == 2)
	{
		levelCount = 3; // Increase the level 
		if (musicOn) 
		{
		
		al_stop_sample_instance(songInstance);
		playMusic(levelCount);
		}
	
		// Background
		bgImage = al_load_bitmap("res/stars.png"); // Initialize image for background
		initBackground(BG, 0, 0, 0.15f, 0, 3840, ScreenHeight, -1, 0, true, bgImage); // Initialize the scrolling background
		mgImage = al_load_bitmap("res/jupiter.png"); // Initialize image for the middle ground
		initBackground(MG, 0, 0, 0.25f, 0, 3840, ScreenHeight, -1, 0, false, mgImage); // Initialize the scrolling background
		fgImage = al_load_bitmap("res/asteroids.png"); // Initialize image for the foreground
		initBackground(FG, 0, 0, 0.35f, 0, 3840, ScreenHeight, -1, 0, false, fgImage); // Initialize the scrolling background
		
		loadEnemies();
	}
	else if (lv == 3)
	{
		levelCount = 4; // Increase the level 
		if (musicOn) 
		{
		
		al_stop_sample_instance(songInstance);
		playMusic(levelCount);
		}
		
		// Background
		bgImage = al_load_bitmap("res/stars.png"); // Initialize image for background
		initBackground(BG, 0, 0, 0.15f, 0, 3840, ScreenHeight, -1, 0, true, bgImage); // Initialize the scrolling background
		mgImage = al_load_bitmap("res/saturn.png"); // Initialize image for the middleground
		initBackground(MG, 0, 0, 0.25f, 0, 3840, ScreenHeight, -1, 0, false, mgImage); // Initialize the scrolling background
		
		loadEnemies();
	}
	else if (lv == 4)
	{
		levelCount = 5; // Increase the level 
		if (musicOn) 
		{
			
		al_stop_sample_instance(songInstance);
		playMusic(levelCount);
		}
		
		// Background
		bgImage = al_load_bitmap("res/stars.png"); // Initialize image for background
		initBackground(BG, 0, 0, 0.15f, 0, 3840, ScreenHeight, -1, 0, true, bgImage); // Initialize the scrolling background
		mgImage = al_load_bitmap("res/uranus.png"); // Initialize image for the middleground
		initBackground(MG, 0, 0, 0.25f, 0, 3840, ScreenHeight, -1, 0, false, mgImage); // Initialize the scrolling background
		
		loadEnemies();
	}
	else if (lv == 5)
	{
		levelCount = 6; // Increase the level 
		if (musicOn) 
		{
		
		al_stop_sample_instance(songInstance);
		playMusic(levelCount);
		}

		// Background
		bgImage = al_load_bitmap("res/stars.png"); // Initialize image for background
		initBackground(BG, 0, 0, 0.15f, 0, 3840, ScreenHeight, -1, 0, true, bgImage); // Initialize the scrolling background
		mgImage = al_load_bitmap("res/neptune.png"); // Initialize image for the middleground
		initBackground(MG, 0, 0, 0.25f, 0, 3840, ScreenHeight, -1, 0, false, mgImage); // Initialize the scrolling background
		
		loadEnemies();
	}
	else if (lv == 6)
	{
		gameState = WinGame;
	}
	else if(lv == 99)
	{
		// WARP
		isWarp = true; 
		if (musicOn) 
		{
			al_stop_sample_instance(songInstance);
			playMusic(10); //  Play warp music
		}
	}
	//fadeValue = 1.0f; // Reset fade value
}

void gamemanager::warp()
{
	/// WARP EFFECT: Will gradually speed up backgrounds until warpCounter = desired value.
	FG.velX += 0.10f;
	MG.velX += 0.08f;
	BG.velX += 0.06f;
	
	/// Warp will delay the cutscene for loading the next planet
	warpCounter++; // Increment
	if (warpCounter > 1380 && fadeValue < 300.0f) fadeValue += 2;
	if (warpCounter == 1500)
	{
		loadLevel(levelCount); // Progress to next level.
		isWarp = false; // Stop warp
		fadeValue = 200.0f; // Reset Fade to default state (black screen)
		warpCounter = 0; // Reset warpCounter
	}
}

void gamemanager::checkEnemyStatus(std::vector<std::shared_ptr<enemy>> enemies)
{
	enemyCounter = 0; // Reset enemy counter
	std::vector<std::shared_ptr<enemy>>::iterator e;
	for (e = enemies.begin(); e != enemies.end(); e++)
	{
		if ((*e)->getIsAlive() == false && bossBattle == false)
		{
			enemyCounter++;
		}
		if (enemyCounter >= enemyAmount && bossBattle == false)
		{
			// Load the boss
			loadBoss(levelCount);
			enemyCounter = 0;
		}
	}

}

void gamemanager::checkPlayerCollision(ship& ship, std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b)
{
	/// Check for collisions from the enemey (their bullets / cannon / missiles -- whatever)
	// Run through all the enemies
	std::vector<std::shared_ptr<enemy>>::iterator e;
	for (e = enemies.begin(); e != enemies.end(); e++)
	{
		if ((*e)->getIsAlive() == true)
		{
			// BULLET DAMAGE
			// Check each bullet that the enemy fired on the screen for a collision with the player
			std::vector<std::shared_ptr<enemy_bullet>>::iterator it;
			for (it = (*e)->enemy_bullets.begin(); it != (*e)->enemy_bullets.end(); it++)
			{
				if ((*it)->getIsAlive() == true) // Their bullet is on the screen and alive
				{
					// Two conditions, if shield is active or not.
					if (ship.getShields() > 10) // If the shields are over 10, make the collision bounds the shields
					{
						if ((*it)->getX() <= ship.getX() + 80 && (*it)->getX() >= ship.getX() + 80 - (*it)->getWidth()) // X collision
						{
							if ((*it)->getY() + 2 >= ship.getY() + 10 && (*it)->getY() + 2 <= ship.getY()  + 60) // Y collision
							{
								auto s = std::make_shared<particle>(); 
								s->initParticles((*it)->getX(), (*it)->getY(), rand() % 10 + 1, 4,3); // Type 3 is shields particle effect
								particle_fx.push_back(s);
								auto s1 = std::make_shared<particle>(); 
								s1->initParticles((*it)->getX(), (*it)->getY(), rand() % 10 + 1, 5,3);
								particle_fx.push_back(s1);
								
								(*it)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
								ship.takeDamage((*e)->getLaserDmg()); // The enemy will take damage if the collision occurs.
							}
						}
					}
					else if (ship.getShields() < 10 && ship.getIsAlive() == true)// Make the collision bounds the ship
					{
						if ((*it)->getX() <= ship.getX() + 40 && (*it)->getX() >= ship.getX() + 40 - (*it)->getWidth()) // X collision
						{
							if ((*it)->getY() + 2 >= ship.getY() + 19 && (*it)->getY() + 2 <= ship.getY()  + 43) // Y collision
							{
								auto h = std::make_shared<particle>(); 
								h->initParticles((*it)->getX(), (*it)->getY(), rand() % 10 + 1, 5,0); 
								particle_fx.push_back(h);
								
								(*it)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
								ship.takeDamage((*e)->getLaserDmg()); // The enemy will take damage if the collision occurs.
							}
						}
					}
				}
			}
		}
	}
	if (bossBattle == true)
	{
		// Find the boss and detect collision with ship
		std::vector<std::shared_ptr<boss>>::iterator e;
		for (e = b.begin(); e != b.end(); e++)
		{
			if ((*e)->getIsAlive() == true) // Make sure the boss is alive
			{
				// Check each bullet that the enemy fired on the screen for a collision with the player
				std::vector<std::shared_ptr<enemy_bullet>>::iterator it;
				for (it = (*e)->boss_bullets.begin(); it != (*e)->boss_bullets.end(); it++)
				{
					if ((*it)->getIsAlive() == true) // Their bullet is on the screen and alive
					{
						// Two conditions, if shield is active or not.
						if (ship.getShields() > 10) // If the shields are over 10, make the collision bounds the shields
						{
							if ((*it)->getX() <= ship.getX() + 80 && (*it)->getX() >= ship.getX() + 80 - (*it)->getWidth()) // X collision
							{
								if ((*it)->getY() + 2 >= ship.getY() + 10 && (*it)->getY() + 2 <= ship.getY()  + 60) // Y collision
								{
									auto b = std::make_shared<particle>(); 
									b->initParticles((*it)->getX(), (*it)->getY(), rand() % 10 + 1, 4,3); 
									particle_fx.push_back(b);
									auto b1 = std::make_shared<particle>(); 
									b1->initParticles((*it)->getX(), (*it)->getY(), rand() % 10 + 1, 5,3);
									particle_fx.push_back(b1);
									
									(*it)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
									ship.takeDamage((*e)->getCannonDmg()); // The enemy will take damage if the collision occurs.
								}
							}
						}
						else if (ship.getShields() < 10 && ship.getIsAlive() == true)// Make the collision bounds the ship
						{
							if ((*it)->getX() <= ship.getX() + 40 && (*it)->getX() >= ship.getX() + 40 - (*it)->getWidth()) // X collision
							{
								if ((*it)->getY() + 2 >= ship.getY() + 19 && (*it)->getY() + 2 <= ship.getY()  + 43) // Y collision
								{
									auto b3 = std::make_shared<particle>(); 
									b3->initParticles((*it)->getX(), (*it)->getY(), rand() % 10 + 1, 5,0); 
									particle_fx.push_back(b3);
									
									(*it)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
									ship.takeDamage((*e)->getCannonDmg()); 
								}
							}
						}
					}
				}
			}
		}
	}
}

void gamemanager::checkEnemyCollision(ship& ship, std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b)
{
	/// Check weapons for collisions on enemy.
	// Run through all the enemies
	std::vector<std::shared_ptr<enemy>>::iterator e;
	for (e = enemies.begin(); e != enemies.end(); e++)
	{
		if ((*e)->getIsAlive() == true) // Make sure the enemy that is firing is alive
		{
			// MISSILES
			std::vector<std::shared_ptr<missile>>::iterator m;
			for (m = ship.player_missiles.begin(); m != ship.player_missiles.end(); m++)
			{
				if ((*m)->getIsAlive() == true) // Check if missile is on screen.
				{
					// This will correctly adjust the collision bounds of the image
					if ((*m)->getX() + 10 >= (*e)->getX() + 19 && (*m)->getX() + 10 <= (*e)->getX() + 19 + (*m)->getWidth()) // X collision
					{
						if ((*m)->getY() + 2 >= (*e)->getY() + 20 && (*m)->getY() + 2 <= (*e)->getY()  + 40)  // Y collision
						{
							if ((*e)->getIsAlive() == true)
							{
								// HIT - Player bullet collision on enemy
								_score += 150; // Bullet collision worth 150 points
								al_play_sample(boom, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
								
								auto p = std::make_shared<particle>(); 
								p->initParticles((*m)->getX(), (*m)->getY(), 75, 5,0); 
								particle_fx.push_back(p);
								
								if ((*e)->getHealth() - ship.getMissileDmg() < 0) // If the enemy dies, get 1000 points
								{
									_score += 1000;
									auto p1 = std::make_shared<particle>(); 
									p1->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 50, 1,0); // Create the particle fx in the centre of the enemy bitmap
									particle_fx.push_back(p1);
									// We will do this a couple times to randomize the particles as much as possible.
									auto p2 = std::make_shared<particle>(); 
									p2->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 50, 2,0); 
									particle_fx.push_back(p2);
									auto p3 = std::make_shared<particle>(); 
									p3->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 50, 3,0); 
									particle_fx.push_back(p3);
								}
								(*m)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
								(*e)->takeDamage(ship.getMissileDmg()); // The enemy will take damage if the collision occurs.
							}
						}
					}
				}
			}
			// BULLETS
			// Check each bullet that we fired on the screen for a collision
			std::vector<std::shared_ptr<bullet>>::iterator it;
			for (it = ship.player_bullets.begin(); it != ship.player_bullets.end(); it++)
			{
				if ((*it)->getIsAlive() == true) // If the bullet is on the screen
				{
					// This will correctly adjust the collision bounds of the image
					if ((*it)->getX() + 10 >= (*e)->getX() + 19 && (*it)->getX() + 10 <= (*e)->getX() + 19 + (*it)->getWidth()) // X collision
					{
						if ((*it)->getY() + 2 >= (*e)->getY() + 20 && (*it)->getY() + 2 <= (*e)->getY()  + 40)  // Y collision
						{
							if ((*e)->getIsAlive() == true)
							{
								// HIT - Player bullet collision on enemy
								_score += 150; // Bullet collision worth 150 points
								
								auto p = std::make_shared<particle>(); 
								p->initParticles((*it)->getX(), (*it)->getY(), rand() % 5 + 1, 5,0); 
								particle_fx.push_back(p);
								
								if ((*e)->getHealth() - ship.getBlasterDmg() < 0) // If the enemy dies, get 1000 points
								{
									_score += 1000;
									auto p1 = std::make_shared<particle>(); 
									p1->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 15, 1,0); // Create the particle fx in the centre of the enemy bitmap
									particle_fx.push_back(p1);
									// We will do this a couple times to randomize the particles as much as possible.
									auto p2 = std::make_shared<particle>(); 
									p2->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 15, 2,0); 
									particle_fx.push_back(p2);
									auto p3 = std::make_shared<particle>(); 
									p3->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 15, 3,0); 
									particle_fx.push_back(p3);
								}
								(*it)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
								(*e)->takeDamage(ship.getBlasterDmg()); // The enemy will take damage if the collision occurs.
							}
						}
					}
				}
			}
		}
	if (bossBattle == true)
	{
		bossCounter = 0;
		// Find the boss and detect collision.
		std::vector<std::shared_ptr<boss>>::iterator e;
		for (e = b.begin(); e != b.end(); e++)
		{
			if ((*e)->getIsAlive() == true) bossCounter++; // Keeps track of how many bosses are alive.
			
			if (bossCounter == 0)
			{
				// endCounter will delay the level from changing from changing right away.
				endCounter++; // Increment the endCounter
				
				if (endCounter == 6000)//Counts to 6000, this will show complete death animation of boss then load level.
				{	
					_score += 75000;
					ship.isWarping = true;
					loadLevel(99); // Load the warp level
					endCounter = 0;
					ship.resetShip(); // Reset ship location and free resources
					bossBattle = false; // Reset
					break;
				}
			}
			
			// MISSILES
			std::vector<std::shared_ptr<missile>>::iterator m;
			for (m = ship.player_missiles.begin(); m != ship.player_missiles.end(); m++)
			{
				if ((*m)->getIsAlive() == true) // If the bullet is on the screen
				{
					// This will correctly adjust the collision bounds of the image
					if ((*m)->getX() + 10 >= (*e)->getX() + 19 && (*m)->getX() + 10 <= (*e)->getX() + 19 + (*m)->getWidth()) // X collision
					{
						if ((*m)->getY() + 2 >= (*e)->getY() + 20 && (*m)->getY() + 2 <= (*e)->getY()  + 40)  // Y collision
						{
							// HIT - Player missile collision on enemy
							
							auto x = std::make_shared<particle>(); 
							x->initParticles((*m)->getX(), (*m)->getY(), 75, 5,0); 
							particle_fx.push_back(x);
							 
							al_play_sample(boom, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
							_score += 150; // Bullet collision worth 150 points
							
							(*m)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
							(*e)->takeDamage(ship.getMissileDmg()); // The enemy will take damage if the collision occurs.
							
							if ((*e)->getHealth() <= 0)
							{
								// Particles
								auto bossdie = std::make_shared<particle>(); 
								bossdie->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 5,0); 
								particle_fx.push_back(bossdie);
								auto bossdie2 = std::make_shared<particle>(); 
								bossdie2->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 2,0); 
								particle_fx.push_back(bossdie2);
								auto bossdie3 = std::make_shared<particle>(); 
								bossdie3->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 4,0); 
								particle_fx.push_back(bossdie3);
								auto bossdie4 = std::make_shared<particle>(); 
								bossdie4->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 6,0); 
								particle_fx.push_back(bossdie4);
								break;
							}
						}
					}
				}
			}
			
			// BULLETS
			std::vector<std::shared_ptr<bullet>>::iterator it;
			for (it = ship.player_bullets.begin(); it != ship.player_bullets.end(); it++)
			{
				if ((*it)->getIsAlive() == true) // If the bullet is on the screen
				{
					// This will correctly adjust the collision bounds of the image
					if ((*it)->getX() + 10 >= (*e)->getX() + 19 && (*it)->getX() + 10 <= (*e)->getX() + 19 + (*it)->getWidth()) // X collision
					{
						if ((*it)->getY() + 2 >= (*e)->getY() + 20 && (*it)->getY() + 2 <= (*e)->getY()  + 40)  // Y collision
						{
							// HIT - Player bullet collision on enemy
							
							auto x = std::make_shared<particle>(); 
							x->initParticles((*it)->getX(), (*it)->getY(), rand() % 5 + 1, 5,0); 
							particle_fx.push_back(x);
							
							_score += 150; // Bullet collision worth 150 points
							
							(*it)->deactivate(); // This disables the bullet, it will no longer paint to the screen/move
							(*e)->takeDamage(ship.getBlasterDmg()); // The enemy will take damage if the collision occurs.
							
							if ((*e)->getHealth() <= 0)
							{
								// Particles
								auto bossdie = std::make_shared<particle>(); 
								bossdie->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 5,0); 
								particle_fx.push_back(bossdie);
								auto bossdie2 = std::make_shared<particle>(); 
								bossdie2->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 2,0); 
								particle_fx.push_back(bossdie2);
								auto bossdie3 = std::make_shared<particle>(); 
								bossdie3->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 4,0); 
								particle_fx.push_back(bossdie3);
								auto bossdie4 = std::make_shared<particle>(); 
								bossdie4->initParticles((*e)->getX() + 32, (*e)->getY() + 32, 300, 6,0); 
								particle_fx.push_back(bossdie4);
								break;
							}
						}
					}
				}
			}
		}
	}
	}
}
	
void gamemanager::renderAllItems(std::vector<std::shared_ptr<item>> items, ship& ship)
{
	std::vector<std::shared_ptr<item>>::iterator i;
	for (i = items.begin(); i != items.end(); i++)
	{
		(*i)->render(); // Render all items
		if (ship.getX() + 60 >= (*i)->getX() + 32 && ship.getX() <= (*i)->getX() + 34)
		{
			if (ship.getY() + 60 >= (*i)->getY() + 32 && ship.getY() <= (*i)->getY() + 34)
			{
				if ((*i)->getIsAlive() == true)
				{
					// If it is a health item, boost health
					ship.boostHealth(rand() % 25 + 1);
					(*i)->deactivate();
				}
				
			}
		}
	}
}
void gamemanager::paintAllItems(std::vector<std::shared_ptr<item>> items)
{
	std::vector<std::shared_ptr<item>>::iterator i;
	for (i = items.begin(); i != items.end(); i++)
	{
		(*i)->paint(); // Paint all items
	}
}
void gamemanager::renderAllEnemies(std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b)
{
	std::vector<std::shared_ptr<enemy>>::iterator e;
	for (e = enemies.begin(); e != enemies.end(); e++)
	{
		if ((*e)->getIsAlive() == true)
		(*e)->render(); // Render all enemies
		
		if ((*e)->getIsItem() == true && ((*e)->_hasSpawned == false))
		{
			(*e)->_hasSpawned = true;
			auto item_ptr = std::make_shared<item>((*e)->getX(), (*e)->getY());
			item_collection.push_back(item_ptr);
		}
	}
	
		
	if (bossBattle == true)
	{
		std::vector<std::shared_ptr<boss>>::iterator it;
		for (it = b.begin(); it != b.end(); it++)
		{
			if ((*it)->getIsAlive() == true)
			(*it)->render(); // Render boss
		}
	}
}

void gamemanager::paintAllEnemies(std::vector<std::shared_ptr<enemy>> enemies, std::vector<std::shared_ptr<boss>> b)
{
	std::vector<std::shared_ptr<enemy>>::iterator e;
	for (e = enemies.begin(); e != enemies.end(); e++)
	{
		(*e)->paint(); // Paint all enemies
	}
	
		
	if (bossBattle == true)
	{
		std::vector<std::shared_ptr<boss>>::iterator it;
		for (it = b.begin(); it != b.end(); it++)
		{
			(*it)->paint(); // // paint boss
		}
	}
	
}

void gamemanager::renderParticles(std::vector<std::shared_ptr<particle>> particles)
{
	std::vector<std::shared_ptr<particle>>::iterator p;
	for (p = particles.begin(); p != particles.end(); p++)
	{
		(*p)->render(); // Render the particle fx
	}
	
}

void gamemanager::paintParticles(std::vector<std::shared_ptr<particle>> particles)
{
	std::vector<std::shared_ptr<particle>>::iterator p;
	for (p = particles.begin(); p != particles.end(); p++)
	{
		(*p)->paint(); // Paint the particle fx
	}
	
}
void gamemanager::paintUI(ship& ship)
{
	int health = ship.getHealth(); // HP
	int shields = ship.getShields(); // Shield points
	
	al_draw_filled_rectangle(1,1,ScreenWidth,28,al_map_rgba(50,50,50,5)); // Rectangle for the UI
	al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 5, ALLEGRO_ALIGN_LEFT, "     HP: %i" , health); // Displaying Hit Points
	al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 15, ALLEGRO_ALIGN_LEFT, "SHIELDS: %i", shields); // Displaying Shield Points
	al_draw_textf(font_ui, al_map_rgb(255, 255, 255), ScreenWidth - 10 , 12, ALLEGRO_ALIGN_RIGHT, "SCORE: %i", _score); 
	
	// Get the HP of the ship and paint accordingly
	if (ship.getHealth() > 10)
	{
		if (ship.getHealth() > 90 && ship.getHealth() <= 105)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(12, 255, 0, 60),2);
		}
		else if(ship.getHealth() > 80 && ship.getHealth() <= 90)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(72, 205, 0, 60),2);
		}
		else if(ship.getHealth() > 70 && ship.getHealth() <= 80)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(126, 255, 0, 60),2);
		}
		else if(ship.getHealth() > 60 && ship.getHealth() <= 70)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(186, 255, 0, 60),2);
		}
		else if(ship.getHealth() > 50 && ship.getHealth() <= 60)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(228, 255, 0, 60),2);
		}
		else if(ship.getHealth() > 40 && ship.getHealth() <= 50)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(255, 246, 0, 60),2);
		}
		else if(ship.getHealth() > 30 && ship.getHealth() <= 40)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(255, 204, 0, 60),2);
		}
		else if(ship.getHealth() > 20 && ship.getHealth() <= 30)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(255, 150, 0, 60),2);
		}
		else if(ship.getHealth() > 10 && ship.getHealth() <= 20)
		{
			al_draw_rectangle(150, 6, 150 + ship.getHealth()* 10, 9, al_map_rgba(255, 36, 0, 60),2);
		}
	}
	else 
	{
		al_draw_rectangle(150, 6, 150 + 5, 9, al_map_rgba(255, 36, 0, 60),2);
	}
	// Get the shield level, and paint accordingly
	if (ship.getShields() > 10)
	{
		if (ship.getShields() > 90 && ship.getShields() <= 105)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(0, 150, 255, 60),2);
		}
		else if(ship.getShields() > 80 && ship.getShields() <= 90)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(0, 84, 255, 60),2);
		}
		else if(ship.getShields() > 70 && ship.getShields() <= 80)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(0, 0, 255, 60),2);
		}
		else if(ship.getShields() > 60 && ship.getShields() <= 70)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(54, 0, 255, 60),2);
		}
		else if(ship.getShields() > 50 && ship.getShields() <= 60)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(150, 0, 255, 60),2);
		}
		else if(ship.getShields() > 40 && ship.getShields() <= 50)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(234, 0, 255, 60),2);
		}
		else if(ship.getShields() > 30 && ship.getShields() <= 40)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(255, 0, 222, 60),2);
		}
		else if(ship.getShields() > 20 && ship.getShields() <= 30)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(255, 0, 156, 60),2);
		}
		else if(ship.getShields() > 10 && ship.getShields() <= 20)
		{
			al_draw_rectangle(150, 17, 150 + ship.getShields()* 10, 20, al_map_rgba(255, 0, 12, 60),2);
		}
	}
	else 
	{
		al_draw_rectangle(150, 17, 150 + 5, 20, al_map_rgba(255, 0, 0, 60),2);
	}
	
	if (debug == true)
	{
		al_draw_filled_rectangle(28, 290, 200, 380, al_map_rgba(10,10,10,50));
		al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 300, ALLEGRO_ALIGN_LEFT, "Game State: %i" , gameState); // Game State
		al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 310, ALLEGRO_ALIGN_LEFT, "Player X %f" , ship.getX()); // X coordinate
		al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 320, ALLEGRO_ALIGN_LEFT, "Player Y: %f" , ship.getY()); // Y coordinate
		al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 330, ALLEGRO_ALIGN_LEFT, "Player isAlive: %i" , ship.getIsAlive()); // isAlive
		al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 340, ALLEGRO_ALIGN_LEFT, "Enemy Count: %i" , enemyCounter); // 
		al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 350, ALLEGRO_ALIGN_LEFT, "Boss Count: %i" , bossCounter); // 
		al_draw_textf(font_ui, al_map_rgb(255, 255, 255), 30 , 360, ALLEGRO_ALIGN_LEFT, "Level Count: %i" , levelCount); // 
		
	}
}
void gamemanager::mainLoop()
{	
	// MAIN LOOP FOR CONTROLS ETC
	for (int i = 0; i < 255; i++)
	{
		al_draw_text(font, al_map_rgb(i, i, i), ScreenWidth/ 2 , ScreenHeight/2, ALLEGRO_ALIGN_CENTER, "Loading");
		al_flip_display();
	}
	
	if (musicOn) playMusic(levelCount);
	
	al_clear_to_color(al_map_rgb(0,0,0));

	ship player1; // Make a player1 ship
	
	while(!done)
	{
		ALLEGRO_JOYSTICK_STATE joystate; // For the joystick/controller state
		
		// MENU STATES ------------------------------------------------------------------------------------------------------------------------------------
		if (menuState == NewGame)
		{
			// Draw Menu 	
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "NEW GAME");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OPTIONS");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "HI SCORES");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 175, ALLEGRO_ALIGN_CENTER, "EXIT");
			al_flip_display();
			
			while(menuState == NewGame)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}				
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Options;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Exit;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Start a new game
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = 0; // Return menuState to default state
						loadLevel(levelCount);// Load the first level
						gameState = StartGame;
					break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Options;
						break;
					}
					else if(joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Exit;
						break;
					}
					else if(joystate.button[14])
					{
						// Start a new game
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = 0; // Return menuState to default state
						loadLevel(levelCount);// Load the first level
						gameState = StartGame;
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == Options)
		{
			// Draw Menu 	
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "NEW GAME");
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OPTIONS");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "HI SCORES");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 175, ALLEGRO_ALIGN_CENTER, "EXIT");
			al_flip_display();
			
			while(menuState == Options)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = HiScore;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = NewGame;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Enter Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFx; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = HiScore;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = NewGame;
						break;
					}
					else if (joystate.button[14])
					{
						// Enter Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFx; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == HiScore)
		{
			// Draw Menu 	
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "NEW GAME");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OPTIONS");
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "HI SCORES");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 175, ALLEGRO_ALIGN_CENTER, "EXIT");
			al_flip_display();
			
			while(menuState == HiScore)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Exit;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Options;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// NOT IMPLEMENTED YET
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Exit;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Options;
						break;
					}
					else if(joystate.button[14])
					{
						// NOT IMPLEMENTED YET
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == Exit)
		{
			// Draw Menu 	
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "NEW GAME");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OPTIONS");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "HI SCORES");
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 175, ALLEGRO_ALIGN_CENTER, "EXIT");
			al_flip_display();
			
			while(menuState == Exit)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = NewGame;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = HiScore;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Exit game
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						al_stop_timer(timer); 
						done = true;
						exit(1);
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = NewGame;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = HiScore;
						break;
					}
					else if(joystate.button[14])
					{
						// Exit game
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						al_stop_timer(timer); 
						done = true;
						exit(1);
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == SoundFx)
		{
			// Draw Menu 	
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "SOUND FX");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "MUSIC");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
			al_flip_display();
			
			while(menuState == SoundFx)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}				
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Music;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = OptionsBack;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Enter SoundFx Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOn; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Music;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = OptionsBack;
						break;
					}
					else if(joystate.button[14])
					{
						// Enter SoundFx Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOn; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == Music)
		{
			// Draw Menu 	
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "SOUND FX");
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "MUSIC");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
			al_flip_display();
			
			while(menuState == Music)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = OptionsBack;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFx;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Enter SoundFx Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOn; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = OptionsBack;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFx;
						break;
					}
					else if(joystate.button[14])
					{
						// Enter SoundFx Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOn; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == OptionsBack)
		{
			// Draw Menu 	
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "SOUND FX");
			al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "MUSIC");
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
			al_flip_display();
			
			while(menuState == OptionsBack)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFx;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Music;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Enter SoundFx Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = NewGame; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFx;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = Music;
						break;
					}
					else if(joystate.button[14])
					{
						// Enter SoundFx Options Menu
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = NewGame; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == SoundFxOn)
		{
			// Draw Menu 	
			if (soundOn)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			} 
			else
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			
			while(menuState == SoundFxOn)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOff;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxBack;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Toggle SoundFx On
						soundOn = true;
						menuState = SoundFxOn; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOff;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxBack;
						break;
					}
					else if(joystate.button[14])
					{
						// Toggle SoundFx On
						soundOn = true;
						menuState = SoundFxOn; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == SoundFxOff)
		{
			// Draw Menu 	
			if (soundOn)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON *");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			else
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			while(menuState == SoundFxOff)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxBack;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOn;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Toggle SoundFx Off
						soundOn = false;
						menuState = SoundFxOff; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxBack;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOn;
						break;
					}
					else if(joystate.button[14])
					{
						// Toggle SoundFx Off
						soundOn = false;
						menuState = SoundFxOff; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == SoundFxBack)
		{
			// Draw Menu 	
			if (soundOn)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			else
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF *");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}	
			
			while(menuState == SoundFxBack)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOn;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOff;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Go back to Options / Sound Fx highlighted
						menuState = SoundFx; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (soundOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOn;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (soundOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = SoundFxOff;
						break;
					}
					else if(joystate.button[14])
					{
						// Go back to Options / Sound Fx highlighted
						menuState = SoundFx; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == MusicOn)
		{
			// Draw Menu 	
			if (musicOn)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			else
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			
			while(menuState == MusicOn)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
					
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (musicOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOff;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (musicOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicBack;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Toggle Music On
						if (musicOn == false) al_play_sample_instance(songInstance);
						musicOn = true;
						menuState = MusicOn; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (musicOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOff;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (musicOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicBack;
						break;
					}
					else if(joystate.button[14])
					{
						// Toggle Music On
						if (musicOn == false) al_play_sample_instance(songInstance);
						musicOn = true;
						menuState = MusicOn; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == MusicOff)
		{
			// Draw Menu 	
			if (musicOn)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON *");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			else
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			
			while(menuState == MusicOff)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
	
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (musicOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicBack;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (musicOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOn;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						// Toggle Music Off
						if (musicOn == true) al_stop_sample_instance(songInstance);
						musicOn = false;
						menuState = MusicOff; 
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (musicOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicBack;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (musicOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOn;
						break;
					}
					else if(joystate.button[14])
					{
						// Toggle Music Off
						if (musicOn == true) al_stop_sample_instance(songInstance);
						musicOn = false;
						menuState = MusicOff; 
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (menuState == MusicBack)
		{
			// Draw Menu 	
			if (musicOn)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON *");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			else
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "ON");
				al_draw_text(font, al_map_rgb(100, 100, 100), ScreenWidth/ 2 , ScreenHeight/2 + 125, ALLEGRO_ALIGN_CENTER, "OFF *");
				al_draw_text(font, al_map_rgb(255, 255, 255), ScreenWidth/ 2 , ScreenHeight/2 + 150, ALLEGRO_ALIGN_CENTER, "BACK");
				al_flip_display();
			}
			
			while(menuState == MusicBack)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) // Keyboard Arrow Down
					{
						if (musicOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOn;
						break;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) // Keyboard Arrow Up
					{
						if (musicOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOff;
						break;
					}
					else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						menuState = SoundFx;
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[6]) // Controller D-Pad Down
					{ 
						if (musicOn) al_play_sample(menu_down, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOn;
						break;
					}
					else if (joystate.button[4]) // Controller D-Pad Up
					{
						if (musicOn) al_play_sample(menu_up, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						menuState = MusicOff;
						break;
					}
					else if(joystate.button[14]) // X on the controller
					{
						menuState = SoundFx;
						break;
					}
				}
			al_clear_to_color(al_map_rgb(0,0,0));
			}
		}
		else if (menuState == Continue)
		{
			// Draw Menu 	
			
			while(menuState == Continue)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				// NOT IMPLEMENTED YET
				
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					
					
					
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}

		// GAME STATES ------------------------------------------------------------------------------------------------------------------------------------
		if (gameState == SplashScreen)
		{
			// Show the intro 
			for (int i = 0; i < 255; i++)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font, al_map_rgb(i, i, i), ScreenWidth/ 2  , ScreenHeight/2 - 50, ALLEGRO_ALIGN_CENTER,"© 2015 U OF L - CPSC 2720");
				al_draw_text(font, al_map_rgb(i, i, i), ScreenWidth/ 2 , ScreenHeight/2 - 20, ALLEGRO_ALIGN_CENTER, "TM AND © 2015 N/A");
				al_draw_text(font, al_map_rgb(i, i, i), ScreenWidth/ 2 , ScreenHeight/2 + 10, ALLEGRO_ALIGN_CENTER, "LICENSED BY");
				al_draw_text(font, al_map_rgb(i, i, i), ScreenWidth/ 2 , ScreenHeight/2 + 40, ALLEGRO_ALIGN_CENTER, "N/A");
				al_flip_display();
			}
			al_rest(1);
			al_clear_to_color(al_map_rgb(0,0,0));
			
			for (int j = 0; j < 255; j++)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font, al_map_rgb(0, j, 0), ScreenWidth/ 2 , ScreenHeight/2 - 40, ALLEGRO_ALIGN_CENTER, "GAME DESIGN BY:");
				al_draw_text(font, al_map_rgb(j, j, j), ScreenWidth/ 2 , ScreenHeight/2 - 10, ALLEGRO_ALIGN_CENTER, "WILL HLEUCKA - VU LENI - NAIF SAUD");
				al_flip_display();
			}
			al_rest(1);
			al_clear_to_color(al_map_rgb(0,0,0));
			
			for (int k = 0; k < 255; k+=5)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(k, k, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_flip_display();
			}
			al_clear_to_color(al_map_rgb(0,0,0));
			
			for (int l = 255; l > 0; l-=15)
			{
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_title, al_map_rgb(255, l, 0), ScreenWidth/ 2 , ScreenHeight/2 - 100, ALLEGRO_ALIGN_CENTER, GameName);
				al_flip_display();
			}
			
			for (int m = 0; m < 255; m+=15)
			{
				
				al_draw_text(font, al_map_rgb(m, 0, 0), ScreenWidth/ 2 , ScreenHeight/2 + 100, ALLEGRO_ALIGN_CENTER, "PRESS START OR 'S'");
				al_draw_text(font, al_map_rgb(m, m, m), ScreenWidth/ 2 , ScreenHeight - 100, ALLEGRO_ALIGN_CENTER, "© 2015");
				al_flip_display();
				
			}
			
			while (gameState == SplashScreen)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (ev.keyboard.keycode == ALLEGRO_KEY_S)
					{
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						gameState = 0;
						menuState = NewGame;
						break;
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{	
					if (joystate.button[3]) // Start button on controller
					{
						if (soundOn) al_play_sample(menu_select, 1.0f, 0.0f, 1.0f,ALLEGRO_PLAYMODE_ONCE,&ID);
						gameState = 0;
						menuState = NewGame;
						break;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
		}
		else if (gameState == StartGame)
		{	
			while (gameState == StartGame)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				
				player1.controls(ev, joystate, timer, gameState); // Read player1 inputs etc.
				
			if (ev.type == ALLEGRO_EVENT_TIMER)
				{
					
					// Detect warp
					if (isWarp)
					{
						warp(); // This will delay our cutscene warp
					}
					else
					{
						player1.isWarping = false;
						if (fadeValue > 2) fadeValue -= 1; // Fade in
				}
					
					// CHECK IF PLAYER IS ALIVE
					if (player1.getIsAlive() == false)
					{
						// endCounter will delay the gameState from changing right away.
						if (endCounter == 0)
						{
							// Spawn Particles
							auto end1 = std::make_shared<particle>(); 
							end1->initParticles(player1.getX() + 32, player1.getY() + 32, 300, 5,0); // Create the particle fx in the centre of the ship bitmap
							particle_fx.push_back(end1); // Particle effect
						}
						
						endCounter++; // Increment the endCounter
						
						if (endCounter == 100) // Counts to 100, this will show complete death animation of ship (player1).
						{
							al_stop_timer(timer);
							gameState = EndGame;
							player1.resetVar(); // Reset the ship variables to play new game using same ship object.
						}
					}
					// CHECK FOR COLLISIONS ---------------------------------------------------------------------------------
					checkPlayerCollision(player1, enemies, endboss); // Checking to see if their bullets hit our ship
					checkEnemyCollision(player1, enemies, endboss); // Checking to see if our bullets hit the enemy
					
					// RENDERING --------------------------------------------------------------------------------------------
					player1.render(); // Render Ship
					renderAllEnemies(enemies, endboss); // Render all the enemies 
					
					renderParticles(particle_fx); // Render the particle effects
					renderAllItems(item_collection, player1);// Render all items
					
					updateBackgroundScroll(BG); // Update Background (Scroll the stars)
					updateBackground(MG); // Update whatever level MG we are using.
					
					if (levelCount == 3) // If level 3, move the asteroids
					{
						updateBackground(FG); // Asteroids update
					}
					
					// Update enemy count
					checkEnemyStatus(enemies);
					
					redraw = true; // Maintain fps
				}
				
				if (redraw && al_is_event_queue_empty(event_queue))
				{
					redraw = false;
					
					// DRAWING -----------------------------------------------------------------------------------------------
					
					paintBackground(BG); // Paint the background
					paintBackground(MG); // Paint the middle-ground
					
					if (levelCount == 3) // If level 3, paint the asteroids
					{
						paintBackground(FG);
					}
					
					player1.paint(); // Paint player ship
					paintAllEnemies(enemies, endboss); // Paint all the enemies on the screen 
					
					paintParticles(particle_fx); // Paint the particles
					paintAllItems(item_collection); // Paint all items
					
					al_draw_tinted_bitmap(fade, al_map_rgba(1*fadeValue,1*fadeValue,1*fadeValue,fadeValue),0,0,0); // Fader
					
					paintUI(player1); // Paint the player HUD
					al_flip_display(); 
					// Clear to buffer screen
					al_clear_to_color(al_map_rgb(0,0,0));
				}
			}
		}
		else if (gameState == EndGame)
		{
			al_stop_sample_instance(songInstance);
			sample_devine = al_load_sample("res/devine.ogg");
			playMusic(7); // Game over music
			
			// You lost the game, show score and return to title screen.
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_textf(font, al_map_rgb(255, 0, 13), ScreenWidth/ 2 , ScreenHeight / 2, ALLEGRO_ALIGN_CENTER, "YOU DIED. FINAL SCORE %i", _score);
			al_draw_text(font, al_map_rgb(255, 255, 13), ScreenWidth/ 2 , ScreenHeight / 2 + 100, ALLEGRO_ALIGN_CENTER, "PRESS X OR ENTER TO CONTINUE.");
			al_flip_display(); 
			
			while(gameState == EndGame)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						resetGame();
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[14])
					{
						resetGame();
					}
				}
			}
		}
		else if (gameState == WinGame)
		{
			al_stop_sample_instance(songInstance);
			sample_outcome = al_load_sample("res/outcome.ogg");
			playMusic(8); // Win music
			
			// You win the game, show score and return to title screen.
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_textf(font, al_map_rgb(255, 0, 13), ScreenWidth/ 2 , ScreenHeight / 2, ALLEGRO_ALIGN_CENTER, "CONGRATULATIONS YOU WIN! FINAL SCORE %i", _score);
			al_draw_text(font, al_map_rgb(255, 255, 13), ScreenWidth/ 2 , ScreenHeight / 2 + 100, ALLEGRO_ALIGN_CENTER, "PRESS X OR ENTER TO CONTINUE.");
			al_flip_display(); 
			
			while(gameState == WinGame)
			{
				ALLEGRO_EVENT ev; // ev stores details about the event
				al_wait_for_event(event_queue, &ev); // Wait for something to happen
				
				if (joy != NULL)
				{ 
					// Make sure the controller is present before getting joystick state
					al_get_joystick_state(joy, &joystate);
				}
				// Key / Button Presses
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
					{
						resetGame();
					}
				}
				else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
				{
					if(joystate.button[14])
					{
						resetGame();
					}
				}
			}
		}
	}
}

void gamemanager::resetGame()
{	
	/// Resets the game
	levelCount = 0; // Starting level
	_score = 0;
	bossBattle = false;
	endCounter = 0;
	gameState = 0;
	menuState = NewGame;
	enemies.clear();
	endboss.clear();
	particle_fx.clear();
	item_collection.clear();
	al_start_timer(timer);
}
