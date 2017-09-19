#include "particle.h"

particle::particle()
{
	
}

particle::~particle()
{
	particles.clear();
}

void particle::initParticles(float x, float y, int numParticles, int colour, int type)
{

	
	// Load the particle vector
	for (int i = 0; i < numParticles; i++)
	{
		part P; // The particle
		
		int randDirection = rand() % 8 + 1; // This will randomize the direction that the particle will be moving.
		
		
		int randMax = 8; // This will be the upper bound for our randomization of direction speed 
		
		switch(randDirection)
		{
			case 1: // NE
				P._dirX = -(rand() % randMax + 1);
				P._dirY = -(rand() % randMax + 1);
				break;
			case 2: // N
				P._dirX = 0;
				P._dirY = -(rand() % randMax + 1);
				break;
			case 3: // NW
				P._dirX = rand() % randMax + 1;
				P._dirY = -(rand() % randMax + 1);
				break;
			case 4: // W
				P._dirX = rand() % randMax + 1;
				P._dirY = 0;
				break;
			case 5: // SW
				P._dirX = rand() % randMax + 1;
				P._dirY = rand() % randMax + 1;
				break;
			case 6: // S
				P._dirX = 0;
				P._dirY = rand() % randMax + 1;
				break;
			case 7: // SE
				P._dirX = -(rand() % randMax + 1);
				P._dirY = rand() % randMax + 1;
				break;
			case 8: // E
				P._dirX = -(rand() % randMax + 1);
				P._dirY = 0;
				break;
		}
		
		// Setup
		P._x = x;
		P._y = y;
		P._colour = colour;
	
		P._isAlive = true;
		P._width = rand() % 3 + 1;
		P._height = P._width;
		P._type = type;
		// Type of particle effect 0 = explosion, 1 = thrusters, 2 = missiles, 3 = shields
		if (P._type == 0)
		{
			P._life = 350;
		}
		else if (P._type == 1)
		{
			P._life = 15;
		}
		else if (P._type == 2)
		{
			P._life = 10;
		}
		else if (P._type == 3)
		{
			P._life = 15;
		}
		particles.push_back(P);
	}
}

void particle::render()
{
	std::vector<part>::iterator it;
	for (it = particles.begin(); it != particles.end(); it++)
	{
		if ((*it)._isAlive)
		{
			// If the _x or _y coordinates go off screen, die
			if ((*it)._x < 0 || (*it)._x > ScreenWidth) (*it)._isAlive = false;
			if ((*it)._y < 0 || (*it)._y > ScreenHeight) (*it)._isAlive = false;
			
			if ((*it)._type == 0) // Explosion
			{
				if ((*it)._dirX > 0){
				int explosionModifier = 7; // Higher is smaller
				// Move the particles
				(*it)._x -= (*it)._dirX/explosionModifier;
				(*it)._y += (*it)._dirY/explosionModifier;
				// Increase velocity
				(*it)._dirX += 0.2f;
				//(*it)._dirY += 0.08f;
				}
				else (*it)._isAlive = false;
			}
			else if ((*it)._type == 1) // Thrusters
			{
				if ((*it)._dirX > 0){
					(*it)._x -= (*it)._dirX/5;
					(*it)._y -= (*it)._dirY/3;
					// Increase velocity
					(*it)._dirX += (*it)._dirX * 1.15f;
				}
				else (*it)._isAlive = false;
				
			}
			else if ((*it)._type == 2) // Missiles
			{
				// Move the particles
				if ((*it)._dirX > 0){
					(*it)._x -= 20;
					(*it)._y -= (*it)._dirY/4;
				}
				else (*it)._isAlive = false;
				
			}
			else if ((*it)._type == 3) // Shields
			{
					(*it)._y -= (*it)._dirY/10;
					(*it)._x += rand() % 10 + 1;
					if ((*it)._dirY > 0)
					{
						(*it)._dirY += 1;
					}
					else 
					{
						(*it)._dirY -= 1;
					}
			}
			
			(*it)._life--;
			
			// If the particle life is less than 0, it dies.
			if ((*it)._life <= 0)
			{
				(*it)._isAlive = false;
			}
		}
	}
}

void particle::paint()
{
	std::vector<part>::iterator it;
	for (it = particles.begin(); it != particles.end(); it++)
	{
		if ((*it)._isAlive)
		{
			float randAlpha = rand() % 100 + 10; // Randomize the alpha value every time they are drawn to screen.
			switch ((*it)._colour)
			{
				case 1: // Orange
					al_draw_filled_rectangle((*it)._x, (*it)._y,(*it)._x + (*it)._width, (*it)._y + (*it)._height, al_map_rgba(255, 150, 0, randAlpha)); 
	
					break;
				case 2: // Yellow
					al_draw_filled_rectangle((*it)._x, (*it)._y,(*it)._x + (*it)._width, (*it)._y + (*it)._height, al_map_rgba(240, 255, 0, randAlpha)); 
					
					break;
				case 3: // Red
					al_draw_filled_rectangle((*it)._x, (*it)._y,(*it)._x + (*it)._width, (*it)._y + (*it)._height, al_map_rgba(255, 10, 0, randAlpha)); 
					
					break;
				case 4: // Blue 
					al_draw_filled_rectangle((*it)._x, (*it)._y,(*it)._x + (*it)._width, (*it)._y + (*it)._height, al_map_rgba(0, 150, 255, randAlpha)); 
					
					break;
				case 5: // White-ish-grey
					al_draw_filled_rectangle((*it)._x, (*it)._y,(*it)._x + (*it)._width, (*it)._y + (*it)._height, al_map_rgba(175, 175, 175, randAlpha));
					
					break;
				case 6: // Lime Green
					al_draw_filled_rectangle((*it)._x, (*it)._y,(*it)._x + (*it)._width, (*it)._y + (*it)._height, al_map_rgba(40, 255, 0, randAlpha)); 
					
					break;
			}
			
		}
	}
}
