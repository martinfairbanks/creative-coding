/*	Dynamic objects
	by Martin Fairbanks

	C style - using malloc/realloc to manage memory
*/

#define SDL2	
#include "framework\creativeframework.cpp"

int32 numBalls = 50;

struct Balls
{
	vec2 position{ 0.0f, 0.0f };
	vec2 velocity{ 0.0f, 0.0f };
	int32 radius = 0;
	Color col;
	Color fillCol;
} *balls;

void createNewBall(Balls *balls, int i)
{
	balls[i].radius = random(60);
	balls[i].position.x = random(balls[i].radius * 2, windowWidth - balls[i].radius * 2);
	balls[i].position.y = random(balls[i].radius * 2, windowHeight - balls[i].radius * 2);
	balls[i].velocity.x = random(2, 10);
	balls[i].velocity.y = random(2, 7);
	balls[i].col.r = random(0, 255);
	balls[i].col.g = random(0, 255);
	balls[i].col.b = random(0, 255);
	balls[i].fillCol.r = random(0, 255);
	balls[i].fillCol.g = random(0, 255);
	balls[i].fillCol.b = random(0, 255);
}

void setup()
{
	screen(960, 540, false, "A key: add ball, D key: delete ball");
	balls = (Balls *)malloc(numBalls * sizeof(Balls));
	strokeWeight(4);

	for (int i = 0; i < numBalls; i++)
	{
		createNewBall(balls, i);
	}
}

void updateAndDraw(uint32 t)
{
	clear(purple);

	if (mouseReleased || joyAButton || keyPressed(SDL_SCANCODE_A))
	{
		numBalls++;
		balls = (Balls *)realloc(balls, numBalls * sizeof(Balls));
		createNewBall(balls, numBalls - 1);
	}

	if (joyBButton || keyPressed(SDL_SCANCODE_D))
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

		stroke(balls[i].col);
		fill(balls[i].fillCol);
		circle(balls[i].position.x, balls[i].position.y, balls[i].radius);
	}

	uploadPixels();
	char message[256];
	sprintf_s(message, "Ball count: %d", numBalls);
	stroke(white);
	print(message, 0, windowHeight - 40, 30);
}

void shutdown()
{
	free(balls);
}
