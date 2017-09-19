/*
 * testenemy.h
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

#ifndef TESTENEMY_H
#define TESTENEMY_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "../src/enemy.h"
#include <iostream>

class testEnemy : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(testEnemy);
	CPPUNIT_TEST(takeDamage);
	CPPUNIT_TEST(testDeath);
	CPPUNIT_TEST_SUITE_END();
	public:
		void setUp() {
			enemy_object = new enemy(1,1);
		}

		void tearDown() {
			delete enemy_object;
		}
		/// Test enemy taking damage.
		void takeDamage(){
			enemy_object->takeDamage(50);
			CPPUNIT_ASSERT(14 == enemy_object->getHealth());
		}
		/// Test killing an enemy
		void testDeath(){
			enemy_object->takeDamage(64);
			CPPUNIT_ASSERT(false == enemy_object->getIsAlive());
		}
	private:
		enemy* enemy_object;
};

#endif /* TESTENEMY_H */ 
