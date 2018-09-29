/*	Dynamic objects
	by Martin Fairbanks

	C style - using malloc/realloc to manage memory
*/

#define SDL2	
#include "../framework/creativeframework.cpp"

int32 numBalls = 50;

struct Balls
{
	vec2 position{ 0.0f, 0.0f };
	vec2 velocity{ 0.0f, 0.0f };
	real32 radius = 0;
	Color strokeCol;
	Color fillCol;
} *balls;

void createNewBall(Balls *balls, int i)
{
	balls[i].radius = (real32)random(60);
	balls[i].position.x = random(balls[i].radius * 2, windowWidth - balls[i].radius * 2);
	balls[i].position.y = random(balls[i].radius * 2, windowHeight - balls[i].radius * 2);
	balls[i].velocity.x = random(2, 10);
	balls[i].velocity.y = random(2, 7);
	balls[i].strokeCol.r = random(0, 255);
	balls[i].strokeCol.g = random(0, 255);
	balls[i].strokeCol.b = random(0, 255);
	balls[i].strokeCol.b = 255;
	balls[i].fillCol.r = random(0, 255);
	balls[i].fillCol.g = random(0, 255);
	balls[i].fillCol.b = random(0, 255);
	balls[i].fillCol.a = random(0, 255);
}

void setup()
{
	screen(960, 540, false, "Left click or A key: add ball. Right click or D key: delete ball.");
	
	balls = (Balls *)malloc(numBalls * sizeof(Balls));
	for (int i = 0; i < numBalls; i++)
	{
		createNewBall(balls, i);
	}
}

void updateAndDraw()
{
	clear(purple);
	
	if (mouseUp(MOUSE_LEFT) || joyAButton || keyHit(SDL_SCANCODE_A))
	{
		numBalls++;
		balls = (Balls *)realloc(balls, numBalls * sizeof(Balls));
		createNewBall(balls, numBalls - 1);
	}

	if (mouseUp(MOUSE_RIGHT) || joyBButton || keyHit(SDL_SCANCODE_D))
	{
		if (numBalls != 1)
		{
			numBalls--;
			balls = (Balls *)realloc(balls, numBalls * sizeof(Balls));
		}
	}

	for (int i = 0; i < numBalls; i++)
	{
		balls[i].position += balls[i].velocity;

		if ((balls[i].position.x + balls[i].radius > windowWidth) || (balls[i].position.x - balls[i].radius  < 0))
		{
			balls[i].velocity.x = balls[i].velocity.x * -1;
		}
		if ((balls[i].position.y + balls[i].radius > windowHeight) || (balls[i].position.y - balls[i].radius  < 0))
		{
			balls[i].velocity.y = balls[i].velocity.y * -1;
		}

		stroke(balls[i].strokeCol);
		fill(balls[i].fillCol);
		circle(balls[i].position.x, balls[i].position.y, balls[i].radius);
	}

	stroke(white);
	print(0, 0, 30, "Ball count: %d", numBalls);
}

void shutdown()
{
	free(balls);
}
