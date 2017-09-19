/*
 * testboss.h
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

#ifndef TESTBOSS_H
#define TESTBOSS_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "../src/boss.h"
#include <iostream>

class testBoss : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(testBoss);
	CPPUNIT_TEST(takeDamageShields);
	CPPUNIT_TEST(takeDamageHP);
	CPPUNIT_TEST(testDeath);
	CPPUNIT_TEST_SUITE_END();
	public:
		void setUp() {
			boss_object = new boss(1,1,1); // Boss at x=1,y=1 and level 1
		}

		void tearDown() {
			delete boss_object;
		}
		/// Test taking damage directly to shields
		void takeDamageShields(){
			boss_object->takeDamage(50);
			/// Note that when the level 1 boss takes damage, the damage modifier is applied. 
			/// Therefore, it will take 50 damage / 1.3f (shield modifier) 
			/// The boss should take 38.461538462 damage = ~61.5 HP
			CPPUNIT_ASSERT(61 < boss_object->getShields() && 62 > boss_object->getShields());
		}
		
		void takeDamageHP(){
			/// Take 150 damage.
			boss_object->takeDamage(50);
			boss_object->takeDamage(50);
			boss_object->takeDamage(50);
			/// This should result in the HP taking damage, rather than the shields.
			CPPUNIT_ASSERT(0 > boss_object->getShields());
			CPPUNIT_ASSERT(50 == boss_object->getHealth());
			
		}
		
		void testDeath(){
			boss_object->takeDamage(100);
			boss_object->takeDamage(100);
			/// The boss should be dead, getHealth should return a value < 0. This will start the death animation sequence
			/// for the boss object. Once the death animation sequence is finished, _isAlive value will be false. 
			CPPUNIT_ASSERT(0 <= boss_object->getHealth());
		}
		
	private:
		boss* boss_object;
};

#endif /* TESTBOSS_H */ 
