/*	Bouncing balls
	by Martin Fairbanks
*/
	#define SDL2
	#include "../../framework/creativeframework.cpp"

	const int32 numBalls = 10;

	struct Balls
	{
		vec2 position{ 100.0f, 100.0f };
		vec2 velocity{ 1.5f, 1.0f };
		real32 radius = 80.f;
		Color col;
	} balls[numBalls];

void setup()
{
	screen(960, 540, false, "bouncing balls");
	for (int i = 0; i < numBalls; i++)
	{
		balls[i].radius = random(50);
		balls[i].position.x = random(balls[i].radius + 10, windowWidth - balls[i].radius - 10);
		balls[i].position.y = random(balls[i].radius + 10, windowHeight - balls[i].radius - 10);
		balls[i].velocity.x = random(2, 6);
		balls[i].velocity.y = random(2, 3);
		balls[i].col.r = random(0, 255);
		balls[i].col.g = random(0, 255);
		balls[i].col.b = random(0, 255);
		balls[i].col.a = 255;
	}

	noStroke();
}

void updateAndDraw()
{
	clear(purple);
	
	for (int i = 0; i < numBalls; i++)
	{
		balls[i].position += balls[i].velocity;

		if ((balls[i].position.x + balls[i].radius > windowWidth) || (balls[i].position.x - balls[i].radius < 0))
		{
			balls[i].velocity.x = balls[i].velocity.x * -1;
		}
		if ((balls[i].position.y + balls[i].radius > windowHeight) || (balls[i].position.y - balls[i].radius < 0))
		{
			balls[i].velocity.y = balls[i].velocity.y * -1;
		}

		fill(balls[i].col);
		circle(balls[i].position.x, balls[i].position.y, balls[i].radius);
	}
}

void shutdown() { }
