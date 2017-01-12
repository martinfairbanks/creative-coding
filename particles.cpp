	//#define SDL2	
	#include "framework\creativeframework.cpp"

void setup()
{
	screen(960, 540, true, "creative coding");
	particleTexture = loadTexture("data/particle.png");
	particles.setupParticles(40000);
}

void updateAndDraw(uint32 t)
{
	if (mouseY > (screenHeight / 2))
		particles.blendMode(1);
	else
		particles.blendMode(2);

	color.r = int32(sin(t/800) * 127) + 128;
	color.g = int32(cos(t/5000) * 127) + 128;
	color.b = int32(sin(t/5000) * 127) + 128;

	clear(color);
	uploadPixels();

	particles.updateAndDraw();
}

void shutdown() 
{
	free(particles.particleList);
}