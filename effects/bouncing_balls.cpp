/*	Bouncing balls
	by Martin Fairbanks
*/
	#define SDL2
	#include "framework\creativeframework.cpp"
	#define MAX_BALLS 300
	float32 x = 100;
	float32 y = 100;
	float32 xspeed = 1.5;
	float32 yspeed = 1;
	int32 radius = 20;
	
	struct Balls
	{
		vec2 position{ 100.0f, 100.0f };
		vec2 velocity{ 1.5f, 1.0f };
		int32 radius = 15;
		Color col;
	} balls[MAX_BALLS];


void setup()
{
	screen(960, 540, false, "balls");
	for (int i = 0; i < MAX_BALLS; i++)
	{
		balls[i].position.x = random(0, screenWidth - 10);
		balls[i].position.y = random(0, screenHeight - 50);
		balls[i].velocity.x = random(2, 6);
		balls[i].velocity.y = random(2, 3);
		balls[i].col.r = random(0, 255);
		balls[i].col.g = random(0, 255);
		balls[i].col.b = random(0, 255);
	}
}

void updateAndDraw(uint32 t)
{
	vec2 pos{ 0.0f, 0.0f };
	vec2 center{ (float32)screenWidth/2, (float32)screenHeight/2 };
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos += center;
	clear(purple);
	setColor(green);
	strokeWeight(4);

	for (int i = 0; i < MAX_BALLS; i++)
	{
		balls[i].position += balls[i].velocity;

		setColor(balls[i].col);
		if ((balls[i].position.x > screenWidth) || (balls[i].position.x  < 0))
		{
			balls[i].velocity.x = balls[i].velocity.x * -1;
		}
		if ((balls[i].position.y > screenHeight) || (balls[i].position.y  < 0))
		{
			balls[i].velocity.y = balls[i].velocity.y * -1;
		}
		if (mouseIsPressed)
			rect(balls[i].position.x, balls[i].position.y, balls[i].radius+10, balls[i].radius+10);
		else
			circle(balls[i].position.x, balls[i].position.y, balls[i].radius);
	}

	uploadPixels();
}

void shutdown() { }
