	#define SDL2
	#include "..\framework\creativeframework.cpp"

	const int32 maxEntities = 150;

	struct Entity
	{
		int32 x = random(windowWidth);
		int32 y = random(windowHeight);
		int32 maxDiameter = 50;
		real32 angle = randomf() * 2.f;
		real32 speed = (randomf() + 0.1f) / 10.f;
		Color col = colorHSL(Color{ random(255), 255, random(255) });
		//ColorRGBA col = { tCol.r, tCol.g, tCol.b, random(20,255) };
		void updateAndDraw()
		{
			real32 d3 = 10 + (sin(angle + PI) * maxDiameter / 2) + maxDiameter / 2;
			angle += speed;
			fill(col);
			circle(x, y, (int32)d3);
		}

	} *entities;

void setup()
{
	screen(960, 540, false, "Scaling with sin function");
	strokeWeight(0);
	fill(255, 204, 0);
	entities = (Entity *)malloc(sizeof(Entity) * maxEntities);

	for (int32 i = 0; i < maxEntities; i++)
		entities[i] = Entity();
}

void updateAndDraw()
{
	clear(40, 80, 180);

	for (int32 i = 0; i < maxEntities; i++)
	{
		entities[i].updateAndDraw();
	}
}

void shutdown()
{
	free(entities);
}