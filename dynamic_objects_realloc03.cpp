/*	Dynamic objects	
	by Martin Fairbanks

	C style - using malloc/realloc to manage memory 
	double or half the allocated memory depending on how many objects created
*/

	#define SDL2	
	#include "framework\creativeframework.cpp"

	int32 numEntities = 1;
	int32 allocatedEntities = 100;
	uint32 tempTime;

	struct Entity
	{
		vec2 position{ 0.0f, 0.0f };
		vec2 velocity{ 0.0f, 0.0f };
		int32 radius = 0;
		Color col;
		ColorRGBA fillCol;
	} *entity;

void createNewEntity(Entity *entity, int i)
{
	entity[i].radius = random(20);
	entity[i].position.x = mouseX;
	entity[i].position.y = mouseY;
	entity[i].velocity.x = 0;
	entity[i].velocity.y = 0;
	entity[i].col.r = random(0, 255);
	entity[i].col.g = random(0, 255);
	entity[i].col.b = random(0, 255);
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
	//strokeWeight(4);
	noStroke();
	tempTime = globalTime;
}

void updateAndDraw(uint32 t)
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

	if (mouseButton[RIGHT] && numEntities != 1)
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

		stroke(entity[i].col);
		fill(entity[i].fillCol);
		circle(entity[i].position.x, entity[i].position.y, entity[i].radius);
	}

	if (numEntities > 20 && (tempTime + 20) < globalTime)
	{
		deleteFirstEntity();
		tempTime = globalTime;
	}

	uploadPixels();
	char message[256];
	sprintf_s(message, "Circle count: %d", numEntities);
	stroke(white);
	print(message, 0, windowHeight - 40, 30);
}

void shutdown()
{
	free(entity);
}
