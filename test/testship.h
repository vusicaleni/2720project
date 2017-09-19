/*
 * testship.h
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

#ifndef TESTSHIP_H
#define TESTSHIP_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "../src/ship.h"
#include <iostream>

class testShip : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(testShip);
	CPPUNIT_TEST(takeDamageShields);
	CPPUNIT_TEST(takeDamageHealth);
	CPPUNIT_TEST(testBoostHealth);
	CPPUNIT_TEST(testSurvive);
	CPPUNIT_TEST_SUITE_END();
	public:
		void setUp() {
			player = new ship();
		}

		void tearDown() {
			delete player;
		}
		/// I will test taking damage to the ship. Either the shields or the hp will be directly affected. 
		void takeDamageShields(){
			player->takeDamage(50);
			/// Note that the ship will take damage directly to the shields because it is > 0.
			CPPUNIT_ASSERT(100 == player->getHealth());
			CPPUNIT_ASSERT(50 == player->getShields());
		}
		/// I will test taking damage directly to HP
		void takeDamageHealth(){
			player->takeDamage(50);
			player->takeDamage(50);
			player->takeDamage(10);
			/// Note that the ship will take damage directly to the HP (Health Points) because the shields are < 0.
			CPPUNIT_ASSERT(40 == player->getHealth());
			CPPUNIT_ASSERT(0 > player->getShields());
		}
		/// Test the health item, it will restore HP (health points).
		void testBoostHealth(){
			player->boostHealth(200);
			/// Since the health is at 99 HP at this point, adding 200 should only restore maxHP, or 100 
			CPPUNIT_ASSERT(100 == player->getHealth());
		}
		/// Taking 99 damage should leave us with 1 HP, no shields
		void testSurvive(){
			player->takeDamage(99);
			CPPUNIT_ASSERT(1 == player->getHealth());
		}
			
		
	private:
		ship* player;
};

#endif /* TESTSHIP_H */ 
