/*
 * testbullet.h
 * 
 * Copyright 2015 William Hleucka <william.hleucka@uleth.ca>
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

#ifndef TESTBULLET_H
#define TESTBULLET_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "../src/bullet.h"
#include "../src/enemy_bullet.h"
#include <iostream>

class testBullet : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(testBullet);
	CPPUNIT_TEST(testOffScreen);
	CPPUNIT_TEST(testOffScreenEnemyBullet);
	CPPUNIT_TEST_SUITE_END();
	public:
		void setUp() {
			lazer = new bullet(2000,0); // Create bullet that is off screen.
			lazer2 = new enemy_bullet(2000,0); // Create enemy bullet that is off screen.
		}

		void tearDown() {
			delete lazer;
			delete lazer2;
		}
		/// Render the bullet off screen, it should return false because it is out of bounds.
		void testOffScreen(){
			lazer->render();
			CPPUNIT_ASSERT(false == lazer->getIsAlive());
		}
		/// Render an enemy bullet off screen. Same as above.
		void testOffScreenEnemyBullet(){
			lazer2->render();
			CPPUNIT_ASSERT(false == lazer2->getIsAlive());
		}
	private:
		bullet* lazer;
		enemy_bullet* lazer2;
};

#endif /* TESTBULLET_H */ 
