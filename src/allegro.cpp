/*
 * allegro.cpp
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


#include "allegro.h"


allegro::allegro()
{
	// Constructor
}

allegro::~allegro()
{
	// Destructor
}

void allegro::initAllegro()
{
	// Initialize Allegro Library
	if (!al_init()) allegroMessage("Allegro5", "Error", "Failed to initialize Allegro5.");
    	else std::cout << "※ Allegro initialized." << std::endl;
}

void allegro::initDisplay(ALLEGRO_DISPLAY* d, int w, int h)
{
	// Initialize Allegro Display
	d = al_create_display(w, h);
	if (d == NULL) allegroMessage("Allegro5", "Error", "Failed to initialize display.");
	else std::cout << "※ Display initialized." << std::endl;
}

void allegro::initAddons()
{
	// Initialize Allegro Addons
	if (!al_init_primitives_addon()) allegroMessage("Allegro5", "Error", "Failed to initialize primatives addon.");
	else std::cout << "※ Primatives addon initialized." << std::endl;
	if(!al_init_image_addon()) allegroMessage("Allegro5", "Error", "Failed to initialize image addon.");
	else std::cout << "※ Image addon initialized." << std::endl;
	if(!al_init_acodec_addon()) allegroMessage("Allegro5", "Error", "Failed to initialize acodec addon.");
	else std::cout << "※ Acodec addon initialized." << std::endl;
	
	al_init_font_addon();
	if (!al_init_ttf_addon()) allegroMessage("Allegro5", "Error", "Failed to initialize true type fonts addon.");
	else std::cout << "※ True type fonts addon initialized." << std::endl;
}

void allegro::initSound()
{
	// Initialize Sound
	if(!al_install_audio()) allegroMessage("Allegro5", "Error", "Failed to initialize sound.");
	else std::cout << "※ Sound initialized." << std::endl;
}

void allegro::initControls()
{
	// Initialize Controls
	if (!al_install_keyboard()) allegroMessage("Allegro5", "Error", "Failed to initialize keyboard");
	else std::cout << "※ Keyboard initialized." << std::endl;
	if (!al_install_joystick()) allegroMessage("Allegro5", "Error", "Failed to initialize joystick");
	else std::cout << "※ Joystick initialized." << std::endl;
}

void allegro::allegroMessage(const char* window_title, const char* title, const char* message)
{
	al_show_native_message_box(NULL, window_title, title, message, NULL, ALLEGRO_MESSAGEBOX_WARN);
}
