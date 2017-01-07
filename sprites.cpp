	//#define SDL2	
	//#define WIN_32	
	#include "framework\creativeframework.cpp"

	Sprite myTexture;
	Sprite mySprite;
	real32 xAngle = 0;

	uint8 enemy[11 * 8] =
	{
		0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,
		0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0
	};

void setup()
{
	screen(960, 540, false, "creative coding");
	myTexture.loadTexture("data/invader.jpeg");
	mySprite.load("data/animation.png", 128, 82);
}

void updateAndDraw(uint32 t)
{
	xAngle += 0.1;

	clear(Color::navy);
	setColor(Color::c64orange);
	softSprite(sprite0, 100, 100, 16, 16);
	setColor(Color::turquoise);
	softSprite(enemy, 200, 200, 11, 8);
	uploadPixels();

	/* set alpha and blending
	/* blend modes -> SDL_BLENDMODE_NONE, SDL_BLENDMODE_BLEND, SDL_BLENDMODE_ADD, SDL_BLENDMODE_MOD */
	myTexture.setBlendMode(SDL_BLENDMODE_BLEND);
	myTexture.setAlpha(100);

	/* modulate texture rgb */
	mySprite.setColor(0, 255, 0);

	/* rotate image */
	myTexture.drawEx((screenWidth - myTexture.width) / 2, (screenHeight - myTexture.width) / 2, NULL, xAngle, NULL);

	/* animate */
	int currentFrame = int(((SDL_GetTicks() / 100) % 6));

	mySprite.setColor(0, 0, 255);
	mySprite.setAlpha(128);
	mySprite.draw(290, 110, currentFrame);
	mySprite.setColor(255, 0, 0);
	mySprite.setAlpha(255);
	mySprite.draw(300, 100, currentFrame);

	/* rotate image 45 degrees and flip it */
	myTexture.draw(400, 180, 100, 100, 45.0f, SDL_FLIP_VERTICAL);
}

void shutdown() 
{
	myTexture.clean();
	mySprite.clean();
}