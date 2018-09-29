/*	Dynamic array	
	by Martin Fairbanks
*/
	#define SDL2	1 
	#include "..\framework\creativeframework.cpp"

	Array entities;

	struct Entity
	{
		vec2 position{ 0.0f, 0.0f };
		vec2 velocity{ 0.0f, 0.0f };
		int32 radius = 0;
		Color strokeCol;
		Color fillCol;

		void clicked()
		{
			//check distance -> sqrt((mouseX - position.x) * (mouseX - position.x) + (mouseY - position.y) * (mouseY - position.y))
			int32 d = (int32)dist((real32)mouseX, (real32)mouseY, position.x, position.y);
			//if the distance is less than the radius then the mouse cursor is inside the circle
			if (d < radius)
			{
				fillCol = Color{random(255), random(255), random(255), 255};
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

			stroke(strokeCol);
			fill(fillCol);
			circle(position.x, position.y, (real32)radius);
			//rect((int32)position.x, (int32)position.y, radius, radius);
		}
	};

	Entity* create()
	{
		Entity *ball = (Entity *)malloc(sizeof(Entity));
		ball->radius = random(10, 60);
		ball->position.x = (real32)random(ball->radius * 2, windowWidth - ball->radius * 2);
		ball->position.y = (real32)random(ball->radius * 2, windowHeight - ball->radius * 2);
		ball->velocity.x = (real32)random(2, 12);
		ball->velocity.y = (real32)random(1, 6);
		ball->strokeCol.r = random(255);
		ball->strokeCol.g = random(255);
		ball->strokeCol.b = random(255);
		ball->strokeCol.a = 255;
		ball->fillCol.r = random(255);
		ball->fillCol.g = random(255);
		ball->fillCol.b = random(255);
		ball->fillCol.a = 255;
		return ball;
	}

void setup()
{
	screen(960, 540, false, "A key: add ball, D key: delete ball");

	//strokeWeight(4);
	entities.reserve(4);
	
	for (int32 i = 0; i < 4; i++)
	{
		entities.push(create());
	}
}

void updateAndDraw()
{
	clear(purple);

	if (joyAButton || keyHit(SDL_SCANCODE_A))
	{
		entities.push(create());
	}

	if (joyBButton || keyHit(SDL_SCANCODE_D))
	{
		entities.splice(entities.length-1);
	}
	
	for (int32 i = 0; i < entities.length; i++)
	{
		Entity *entity = (Entity *)entities.get(i);
		if (mouseClick(MOUSE_LEFT))
		{
			entity->clicked();
		}
		entity->updateAndDraw();
	}
	
	stroke(white);
	print(0, 0, 30,"Ball count: %d", entities.length);
}

void shutdown()
{
	entities.freeMem();
}
