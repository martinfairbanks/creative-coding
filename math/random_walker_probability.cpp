	/*	Random walkers and probability by Martin Fairbanks
		These exeamples are inspired by the intro chapter of Daniel Schiffman's excellent book The Nature of Code
		http://natureofcode.com/
	*/

	#define SDL2
	#include "..\framework\creativeframework.cpp"
	
	struct Walker
	{
		int32 x, y;

		Walker()
		{
			x = windowWidth / 2;
			y = windowHeight / 2;
		}

		void display()
		{
			pixel(x, y);
		}

		void step()
		{
			int32 choice = random(0, 3);

			if (choice == 0)
			{
				x++;
			}
			else if (choice == 1)
			{
				x--;
			}
			else if (choice == 2)
			{
				y++;
			}
			else
			{
				y--;
			}
		}
	} *walker;

void setup()
{
	screen(960, 540, false, "random walkers");
	clear(c64blue);
	fill(magenta);
	//walker = new Walker();
}

void updateAndDraw(uint32 t)
{
	//walker->step();
	//walker->display();
	stroke(magenta);

	static int32 scene = 0;
	static int32 x = center.x, y = center.y;
	static int32 stepX, stepY, prevX, prevY;
	static int32 x2 = 100, y2 = 200;
	int32 choice;
	real32 randomVal, stepsize;
	
	//a probability of 1%
	static float prob = 0.01;

	switch (scene)
	{
	case 0:
		//Random walker that moves randomly in four different directions

		//random number between 0-3
		choice = random(3);
		//1 in 4 (25%) chance the walker will take any given step
		if (choice == 0)
		{
			x++;
		}
		else if (choice == 1)
		{
			x--;
		}
		else if (choice == 2)
		{
			y++;
		}
		else
		{
			y--;
		}

		pixel(x, y);
		SDL_SetWindowTitle(window, "random walker - four directions");
		break;
		
	case 1:
		//Random walker that moves randomly in eight different directions + standing still
		//numbers between -1 and 1
		//three possible steps along the x- and y-axis (-1, 0, 1) = nine choices
		//1 in 9 (or 11.1%) chance the walker will take any given step
//		stepX = (rand() % 3) - 1;
//		stepY = (rand() % 3) - 1;
		stepX = random(-1, 1);
		stepY = random(-1, 1);
		x += stepX;
		y += stepY;

		circle(x, y, 1);
		SDL_SetWindowTitle(window, "random walker with nine different choices");
		break;

	case 2:
		//Random walker that moves randomly in four different directions, with a 40% chance to move to the left

		//random floating point value between 0 and 1
		randomVal = randomf();

		if (randomVal < 0.2)		//value between 0 and 0.2 = 20% chance to move to the right
			x++;
		else if (randomVal < 0.6)	//between 0.2 and 0.6 = 40% chance to move to the left
			x--;
		else if (randomVal < 0.8)	//between 0.6 and 0.8 = 20% chance
			y++;
		else						//greater than 0.8 = 20% chance
			y--;

		pixel(x, y);
		SDL_SetWindowTitle(window, "random walker with a 40% chance to move to the left");
		break;

	case 3:
		//2 random walkers, one has a 40% chance to move to the right
		//the other has 1% chance to walk at all and then 40% chance to move to the right

		//random floating point value between 0 and 1
		randomVal = randomf();

		if (randomVal < 0.4)
			x++;
		else if (randomVal < 0.6)
			x--;
		else if (randomVal < 0.8)
			y++;
		else
			y--;

		pixel(x, y);

		//if the random value is less than 0.01
		if (randomVal < prob)
		{
			randomVal = randomf();
			if (randomVal < 0.4)
				x2++;
			else if (randomVal < 0.6)
				x2--;
			else if (randomVal < 0.8)
				y2++;
			else
				y2--;

			stroke(green);
			pixel(x2, y2);
		}

		SDL_SetWindowTitle(window, "2 random walkers");
		break;

	case 4:
		//random walker: Lévy flight

		choice = random(3);

		if (choice == 0)
		{
			x++;
		}
		else if (choice == 1)
		{
			x--;
		}
		else if (choice == 2)
		{
			y++;
		}
		else if (choice == 3)
		{
			y--;
		}
		pixel(x, y);

		//random floating point value between 0 and 1
		randomVal = randomf();

		if (randomVal > 0.9)
		{
			stepX = (rand() % 20) - 10;
			stepY = (rand() % 20) - 10;

			line(x, y, x + stepX, y + stepY);
			x = x + stepX;
			y = y + stepY;
		}
		SDL_SetWindowTitle(window, "random walker: Levy flight");
		break;

	case 5:
		//random walker: Lévy flight with Monte Carlo algorithm
		prevX = x;
		prevY = y;

		stepX = random(-1, 1);
		stepY = random(-1, 1);

		stepsize = montecarlo()*20;
		stepX *= stepsize;
		stepY *= stepsize;

		x += stepX;
		y += stepY;

		x = constrain(x, 0, windowWidth - 1);
		y = constrain(y, 0, windowHeight - 1);

		line(prevX, prevY, x, y);

		SDL_SetWindowTitle(window, "random walker: Levy flight with Monte Carlo algorithm");
		break;
	}
	
	if (mouseReleased)
	{
		clear(c64blue);
		scene++;
		x = center.x;
		y = center.y;
	}

	uploadPixels();
}

void shutdown()
{
	//delete walker;
}
