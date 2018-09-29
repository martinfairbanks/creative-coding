/*	Dynamic objects	
	by Martin Fairbanks

	C style - using malloc/realloc to manage memory 
	double or half the allocated memory depending on how many objects created
*/

	#define SDL2	
	#include "../framework/creativeframework.cpp"

	int32 numEntities = 1;
	int32 allocatedEntities = 100;
	uint32 tempTime;

	struct Entity
	{
		vec2 position{ 0.0f, 0.0f };
		vec2 velocity{ 0.0f, 0.0f };
		real32 radius = 0;
		Color strokeCol;
		Color fillCol;
	} *entity;

void createNewEntity(Entity *entity, int i)
{
	entity[i].radius = random(20);
	entity[i].position.x = mouseX;
	entity[i].position.y = mouseY;
	entity[i].velocity.x = 0;
	entity[i].velocity.y = 0;
	entity[i].strokeCol.r = random(0, 255);
	entity[i].strokeCol.g = random(0, 255);
	entity[i].strokeCol.b = random(0, 255);
	entity[i].strokeCol.a = 255;
	entity[i].fillCol.r = 255;//random(0, 255);
	entity[i].fillCol.g = 0; random(0, 255);
	entity[i].fillCol.b = 255;// random(0, 255);
	entity[i].fillCol.a = 150;
}

void deleteFirstEntity()
{
	for (int i = 0; i < numEntities; i++)
	{
		entity[i] = entity[i + 1];
	}
	numEntities--;
}

void setup()
{
	screen(960, 540, false, "Drag the mouse to create more circles, use right mouse button to delete");
	entity = (Entity *)malloc(allocatedEntities * sizeof(Entity));
	tempTime = SDL_GetTicks();
}

void updateAndDraw()
{
	clear(0);

	if (mouseDragged)
	{
		numEntities++;

		//if we need more memory
		if (numEntities >= allocatedEntities)
		{
			//double the allocated memory
			allocatedEntities *= 2;
			entity = (Entity *)realloc(entity, allocatedEntities * sizeof(Entity));
			createNewEntity(entity, numEntities - 1);
		}
		createNewEntity(entity, numEntities - 1);
	}

	if (mouseUp(MOUSE_RIGHT) && numEntities != 1)
		numEntities--;
	
	//if we have to much memory
	if (numEntities > 100 && numEntities < allocatedEntities / 2)
	{
		//half the allocated memory
		allocatedEntities /= 2;
		entity = (Entity *)realloc(entity, allocatedEntities * sizeof(Entity));
	}

	for (int i = 0; i < numEntities; i++)
	{
		entity[i].position += vec2{ randomf(-1, 1), randomf(-1, 1) };

		if ((entity[i].position.x + entity[i].radius > windowWidth) || (entity[i].position.x - entity[i].radius  < 0))
		{
			entity[i].velocity.x = entity[i].velocity.x * -1;
		}
		if ((entity[i].position.y + entity[i].radius > windowHeight) || (entity[i].position.y - entity[i].radius  < 0))
		{
			entity[i].velocity.y = entity[i].velocity.y * -1;
		}

		stroke(entity[i].strokeCol);
		fill(entity[i].fillCol);
		circle(entity[i].position.x, entity[i].position.y, entity[i].radius);
	}

	if (numEntities > 20 && (tempTime + 20) < SDL_GetTicks())
	{
		deleteFirstEntity();
		tempTime = SDL_GetTicks();
	}

	stroke(white);
	print(0, 0, 30, "Ball count: %d", numEntities);
}

void shutdown()
{
	free(entity);
}
