	/*	Noise by Martin Fairbanks
		Perlin noise produces a naturally ordered (smooth) sequence of pseudo-random numbers.
		We can think of one-dimensional Perlin noise as a linear sequence of values over time. 
		noise() returns a value between 0 and 1.
		These examples uses an implementation of Perlin Simplex Noise.

		Some of these exeamples are inspired by the intro chapter of Daniel Schiffman's excellent book The Nature of Code
		http://natureofcode.com/
	*/

	#define SDL2
	//#define OPENGL
	#include "..\framework\creativeframework.cpp"

	struct Mover
	{
		vec2 pos;
		vec2 time{0, 10000};

		void update()
		{
			pos.x = mapVal(noise(time.x), 0, 1, 0, windowWidth);
			pos.y = mapVal(noise(time.y), 0, 1, 0, windowHeight);

			//move forward through "time"
			time += vec2{ 0.01f, 0.01f };
		}

		void draw()
		{
			circle(pos.x, pos.y, 15);
		};

	} mover;

	struct Walker
	{
		real32 x, y;
		//if tx = ty the walker would just move diagonaly
		real32 tx = 0;
		real32 ty = 10000;  

		real32 prevX, prevY;

		void update()
		{
			prevX = x;
			prevY = y;

			//move according to noise values
			x = mapVal(noise(tx), 0, 1, 0, windowWidth);
			y = mapVal(noise(ty), 0, 1, 0, windowHeight);

			tx += 0.01;
			ty += 0.01;
		}

		void draw()
		{
			stroke(white);
			line(prevX, prevY, x, y);
		}

	} walker;

void setup()
{
	screen(960, 540, false, "Perlin noise");
	clear(c64blue);
	fill(green);
	walker.x = center.x;
	walker.y = center.y;
}

void updateAndDraw(uint32 t)
{
	static int32 scene = 0;
	real32 x, y, n, xoff;
	static real32 time=0;
	static real32 increment = 0.02;

	switch (scene)
	{
	case 0:
		//Perlin noise movement
		clear(c64blue);
		mover.update();
		mover.draw();
		SDL_SetWindowTitle(window, "Perlin noise movement");
		break;

	case 1:
		//Perlin noise walker
		walker.update();
		walker.draw();
		SDL_SetWindowTitle(window, "Perlin noise walker");
		break;
		
	case 2:
		n = noise(time);
		//map = value,current min, current max, new min, new max
		x = mapVal(n, 0, 1, 0, windowWidth);

		fill(green, 10);
		circle(x, 250, 20);
		time += 0.01;
		SDL_SetWindowTitle(window, "Perlin noise");
		break;

	case 3:
		//this draws a curve with the x-axis representing time
		clear(teal);
		xoff = time;
		noFill();
		
		//beginShape();
		for (int i = 0; i < windowWidth; i++)
		{
			y = noise(xoff)*windowHeight;
			xoff += 0.001;
			//vertex(i, y);
			pixel(i, y);
		}
		//endShape();
		time += 0.02;
		SDL_SetWindowTitle(window, "Perlin noise curve");
		break;

	case 4:
		colorMode(HSB);
		xoff = 0.0; // Start xoff at 0

		//calculate a noise values for every x,y coordinate in a 2D space
		for (int x = 0; x < windowWidth; x++)
		{
			xoff += increment;   
			float yoff = 0.0;
			for (int y = 0; y < windowHeight; y++)
			{
				yoff += increment;

				//calculate noise and scale by 255
				real32 hue = noise(xoff, yoff) * 255;
				real32 bright = noise(xoff + 1000, yoff + 500, xoff) * 255;

				stroke(hue, 255, bright);
				//pixelBuffer[x + y*windowWidth] = getColor(Color{ (int32)hue, 255,  (int32)bright });
				pixel(x, y);
			}
		}
		SDL_SetWindowTitle(window, "2D Perlin noise");
		colorMode(RGB);
		break;
	}

	if (mouseReleased)
	{
		clear(c64blue);
		scene++;
	}

	uploadPixels();
}

void shutdown() { }
