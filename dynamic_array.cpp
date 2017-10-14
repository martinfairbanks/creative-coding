/*	Dynamic array	
	by Martin Fairbanks
*/
	#define SDL2	
	#include "framework\creativeframework.cpp"
	
	Array entities;

	struct Entity
	{
		vec2 position{ 0.0f, 0.0f };
		vec2 velocity{ 0.0f, 0.0f };
		int32 radius = 0;
		Color col;
		Color fillCol;

		void clicked()
		{
			//check distance -> sqrt((mouseX - position.x) * (mouseX - position.x) + (mouseY - position.y) * (mouseY - position.y))
			int32 d = dist(mouseX, mouseY, position.x, position.y);
			//if the distance is less than the radius then the mouse cursor is inside the circle
			if (d < radius)
			{
				fillCol = Color(random(255), random(255), random(255));
			}
		}

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

	Entity* create()
	{
		Entity *ball = (Entity *)malloc(sizeof(Entity));
		ball->radius = random(10, 60);
		ball->position.x = random(ball->radius * 2, windowWidth - ball->radius * 2);
		ball->position.y = random(ball->radius * 2, windowHeight - ball->radius * 2);
		ball->velocity.x = random(2, 12);
		ball->velocity.y = random(1, 6);
		ball->col.r = random(0, 255);
		ball->col.g = random(0, 255);
		ball->col.b = random(0, 255);
		ball->fillCol.r = random(0, 255);
		ball->fillCol.g = random(0, 255);
		ball->fillCol.b = random(0, 255);
		return ball;
	}

void setup()
{
	screen(960, 540, false, "A key: add ball, D key: delete ball");

	strokeWeight(4);
	entities.reserve(4);

	for (int32 i = 0; i < 4; i++)
	{
		entities.push(create());
	}
}

void updateAndDraw(uint32 t)
{
	clear(purple);

	if (joyAButton || keyPressed(SDL_SCANCODE_A))
	{
		entities.push(create());
	}

	if (joyBButton || keyPressed(SDL_SCANCODE_D))
	{
		entities.splice(entities.length-1);
	}
	
	for (int32 i = 0; i < entities.length; i++)
	{
		Entity *entity = (Entity *)entities.get(i);
		if (mouseIsPressed)
		{
			entity->clicked();
		}
		entity->updateAndDraw();
	}

	uploadPixels();
	char message[256];
	sprintf_s(message, "Ball count: %d", entities.length);
	print(message, 0, windowHeight - 40, 30);
}

void shutdown()
{
	entities.freeMem();
}
