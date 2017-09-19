/*
 * allegro.h
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


#ifndef ALLEGRO_H
#define ALLEGRO_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <exception>

/// This class will set up Allegro 5
class allegro
{	        
	public:
		/// Ctor 
		allegro();
		/// Dtor
		~allegro();
		/// Install allegro library
		void initAllegro(); 
		/// Creates our display with ALLLEGR_DISPLAY object, width and height
		void initDisplay(ALLEGRO_DISPLAY* d, int w, int h); 
		/// Setup sound addon for music and effects
		void initSound(); 
		 /// Initialize controls (keyboard or joystick)
		void initControls();
		 /// Setup addons
		void initAddons();
		/// Custom allegro message box function
		void allegroMessage(const char* window_title, const char* title, const char* message);
	private:
};

#endif /* ALLEGRO_H */ 
