	/*	Dynamic objects 
		by Martin Fairbanks

		C++ style - Using vector,auto and new/delete to manage memory
	*/

	#define SDL2	
	#include "framework\creativeframework.cpp"
	#include <vector>

	int32 numBalls = 50;

	struct Entity
	{
		int32 radius = random(60);
		vec2 position{ random(radius * 2, windowWidth - radius * 2), random(radius * 2, windowHeight - radius * 2) };
		vec2 velocity{ random(2, 10), random(2, 7) };
		Color col{ random(0, 255), random(0, 255), random(0, 255) };
		ColorRGBA fillCol{ random(0, 255), random(0, 255), random(0, 255), 255 };

		void updateAndDraw()
		{
			position += velocity;
			if ((position.x + radius > windowWidth) || (position.x - radius  < 0))
			{
				velocity.x = velocity.x * -1;
			}

			if ((position.y + radius > windowHeight) || (position.y - radius  < 0))
			{
				velocity.y = velocity.y * -1;
			}

			stroke(col);
			fill(fillCol);
			circle(position.x, position.y, radius);
		}
	};
	
	std::vector<Entity *> balls;

void setup()
{
	screen(960, 540, false, "a key: add ball, d key: delete ball");

	strokeWeight(4);

	for (int32 i = 0; i < numBalls; i++)
		balls.push_back(new Entity());
}

void updateAndDraw(uint32 t)
{
	clear(purple);

	if (mouseReleased || joyAButton || keyPressed(SDL_SCANCODE_A))
	{
		numBalls++;
		balls.push_back(new Entity());
	}

	if (joyBButton || keyPressed(SDL_SCANCODE_D))
	{
		if (numBalls != 1)
		{
			numBalls--;
			balls.pop_back();
		}
	}
	for (auto &a : balls)
	{
		a->updateAndDraw();
	}

	uploadPixels();
	char message[256];
	sprintf_s(message, "Ball count: %d", numBalls);
	stroke(white);
	print(message, 0, windowHeight - 40, 30);
}

void shutdown()
{
	for (auto &a : balls)
	{
		delete a;
	}
}
