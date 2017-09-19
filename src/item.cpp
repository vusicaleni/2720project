#include "item.h"

item::item(float x, float y)
{
	_x = x;
	_y = y;
	
	
	xMax = 5;
	xMin = -5;
	
	moveCounter = xMin;
	//itemType = rand() % 0 + 1;
	
	itemType = 0;
	
	switch(itemType)
	{
		case 0:
			items = al_load_bitmap("res/health.png"); // Will spawn a health item size 64x64 bitmap
			break;
	}

}

item::~item()
{
	al_destroy_bitmap(items);
}

void item::render()
{
	if (_isAlive)
	{
		// This will alternate the _x coordinate back and forth
		
		if (!down)
		{
			moveCounter++;
		}
		else
		{
			moveCounter--;
		}
		
		if (moveCounter == xMax)
		{
			down = true;
		}
		else if(moveCounter == xMin)
		{
			down = false;
		}
		
		_x += moveCounter / 2;
		_y += 1;
		
		if (_x < 0) _isAlive = false;
		if(_x > ScreenWidth + 64) _isAlive = false;
		
		if (_y < 0) _isAlive = false;
		if(_y > ScreenHeight + 64) _isAlive = false;
	}
}

void item::paint()
{
	if (_isAlive) al_draw_bitmap(items, _x, _y, 0); // Paint the item on the screen.
}