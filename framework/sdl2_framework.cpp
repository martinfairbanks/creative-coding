	/* TODO:	fix fullscreen switch, the screen coordinates gets messed up if you hit return to get to fullscreen
				
	*/

	/* Linking */
	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "SDL2main.lib")
	#pragma comment(lib, "SDL2_ttf.lib")
	#pragma comment(lib, "SDL2_image.lib")
	//#pragma comment(lib, "SOIL.lib")

	/* Includes */
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_image.h>
	#include <sstream>	//sprintf
	#include <map>		//for keyPressed
	//#include <SOIL.h>

	/* Globals */
	SDL_Renderer *renderer = 0;
	SDL_Window *window = 0;
	const int32 numFonts = 10;			//number of loaded fonts
	TTF_Font* font[numFonts]{};
	SDL_Texture *backbufferTexture;		//texture used for drawing using the pixelbuffer
	uint32 globalTime;

	const Uint8 *keystates = 0;
	std::map<int, bool> keyArray;
	SDL_GameController *controllerHandle;
	uint8 joyUp;
	uint8 joyDown;
	uint8 joyLeft;
	uint8 joyRight;
	uint8 joyStart;
	uint8 joyBack;
	uint8 joyLeftShoulder;
	uint8 joyRightShoulder;
	uint8 joyAButton;
	uint8 joyBButton;
	uint8 joyXButton;
	uint8 joyYButton;
	int32 joyStickX;
	int32 joyStickY;
	int32 joyRightStickX;
	int32 joyRightStickY;
	const int32 joyDeadZone = 8000;

	uint8 sprite0[16 * 16] =
	{
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0
	};


	struct Color
	{
		int32 r;
		int32 g;
		int32 b;
		Color() {};
		Color(int32 red, int32 green, int32 blue) { r = red; g = green; b = blue; }
	} color{ 0xff,0xff,0xff };

	Color black{ 0,0,0 };
	Color white{ 255,255,255 };
	Color c64red{ 104, 55, 43 };
	Color c64cyan{ 112, 164, 178 };
	Color c64purple{ 111, 61, 134 };
	Color c64green{ 88, 141, 67 };
	Color c64blue{ 53, 40, 121 };
	Color c64yellow{ 184, 199, 111 };
	Color c64orange{ 111, 79, 37 };
	Color c64brown{ 67, 57, 0 };
	Color c64lightred{ 154, 103, 89 };
	Color c64darkgrey{ 68, 68, 68 };
	Color c64grey{ 108, 108, 108 };
	Color c64lightgreen{ 154, 210, 132 };
	Color c64lightblue{ 108, 94, 181 };
	Color c64lightgrey{ 149, 149, 149 };
	Color red{ 255,0,0 };
	Color green{ 0,255,0 };
	Color blue{ 0,0,255 };
	Color yellow{ 255,255,0 };
	Color cyan{ 0,255,255 };
	Color magenta{ 255,0,255 };
	Color purple{ 128, 0, 128 };
	Color gray{ 128, 128, 128 };
	Color grey{ 192, 192, 192 };
	Color maroon{ 128, 0, 0 };
	Color darkgreen{ 0, 128, 0 };
	Color navy{ 0, 0, 128 };
	Color teal{ 0, 128, 128 };
	Color olive{ 128, 128, 0 };
	Color orange{ 255,127,50 };
	Color cornflowerblue{ 101, 156, 239 };
	Color azure{ 0, 127, 255 };
	Color turquoise{ 48, 213, 200 };
	Color gold{ 255, 215, 0 };
	Color silver{ 192, 192, 192 };
	Color pink{ 255, 192, 203 };

	enum ColorModes
	{
		RGB,
		HSB,
		HSL
	};
	
	int32 colorModeFlag = RGB;

	Color strokeColor{ 255, 255, 255 };
	Color fillColor{ 255, 255, 255 };

	void colorMode(int32 mode)
	{
		switch (mode)
		{
		case RGB:
			colorModeFlag = RGB;
			break;
		case HSB:
			colorModeFlag = HSB;
			break;
		case HSL:
			colorModeFlag = HSL;
			break;
		}
	}
	inline void fill()
	{
		fillFlag = true;
	}

	inline void noFill()
	{
		fillFlag = false;
	}

bool keyDown(int32 key)
{
	//returns true until keyup
	return (keystates[key] != 0);
}

bool keyPressed(int32 key)
{
	//returns true once, until keyup
	if (keyArray.find(key) == keyArray.end())
		keyArray[key] = false;

	if (keystates[key])
	{
		if (keyArray[key] == false)
		{
			keyArray[key] = true;
			return true;
		}
	}
	else 
		keyArray[key] = false;

	return false;
}

/* SDL drawing functions */
inline void clear_(Color col)
{
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 255);
	SDL_RenderClear(renderer);
}

inline void setColor_(uint8 r, uint8 g, uint8 b, uint8 a = 255)
{
	color.r = r;
	color.g = g;
	color.b = b;
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

inline void setColor_(Color col, uint8 a = 255)
{
	color = col;
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, a);
}

inline void pixel_(int32 x, int32 y)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

inline void pixel_(int32 x, int32 y, uint32 col)
{
	uint8 r = col >> 16;
	uint8 g = col  >> 8;
	uint8 b = col;
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
	SDL_RenderDrawPoint(renderer, x, y);
	//restore color to the one set with setColor
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
}

inline void pixel_(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
	SDL_RenderDrawPoint(renderer, x, y);
	//restore color to the one set with setColor
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
}

inline void pixel_(int32 x, int32 y, Color col)
{
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 0xff);
	SDL_RenderDrawPoint(renderer, x, y);
	//restore color to the one set with setColor
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
}

void line_(int32 x1, int32 y1, int32 x2, int32 y2)
{
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

inline void rect_(int32 x, int32 y, int32 width, int32 height)
{
	SDL_Rect rect = { x, y, width, height };
	if (fillFlag)
		SDL_RenderFillRect(renderer, &rect);
	else
		SDL_RenderDrawRect(renderer, &rect);
}

inline void fillCircleData_(int xc, int yc, int p, int pb, int pd, int radius, Color col)
{
	int a, b, c, d, e, f, g, h;
	int x = 0;
	int y = radius;
	while (x <= y)
	{
		a = xc + x;
		b = yc + y;
		c = xc - x;
		d = yc - y;
		e = xc + y;
		f = yc + x;
		g = xc - y;
		h = yc - x;
		if (b != pb) SDL_RenderDrawLine(renderer, a, b, c, b);
		if (d != pd) SDL_RenderDrawLine(renderer, a, d, c, d); 
		if (f != b)  SDL_RenderDrawLine(renderer, e, f, g, f); 
		if (h != d && h != f)  SDL_RenderDrawLine(renderer, e, h, g, h);
		pb = b;
		pd = d;
		if (p < 0) p += (x++ << 2) + 6;
		else p += ((x++ - y--) << 2) + 10;
	}
}

inline void circle_(int xc, int yc, int radius)
{
	if (xc + radius < 0 || xc - radius >= screenWidth || yc + radius < 0 || yc - radius >= screenHeight) return;

	if (fillFlag)
	{

		int p = 3 - (radius << 1);
		int pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times  (ensure initial value is outside the range)
		if (lineWidth >= 1)
		{
			fillCircleData_(xc, yc, p, pb, pd, radius, Color{ 0,0,0 });

			radius -= lineWidth;

			int p = 3 - (radius << 1);
			int pb = yc + radius + 1, pd = yc + radius + 1;
			fillCircleData_(xc, yc, p, pb, pd, radius, color);
		}
		else
		{
			fillCircleData_(xc, yc, p, pb, pd, radius, color);
		}
	}
	else
	{
		int32 x1 = radius;
		int32 y1 = 0;
		int32 err = 0;

		while (x1 >= y1)
		{
			SDL_RenderDrawPoint(renderer, xc + x1, yc + y1);
			SDL_RenderDrawPoint(renderer, xc + y1, yc + x1);
			SDL_RenderDrawPoint(renderer, xc - y1, yc + x1);
			SDL_RenderDrawPoint(renderer, xc - x1, yc + y1);
			SDL_RenderDrawPoint(renderer, xc - x1, yc - y1);
			SDL_RenderDrawPoint(renderer, xc - y1, yc - x1);
			SDL_RenderDrawPoint(renderer, xc + y1, yc - x1);
			SDL_RenderDrawPoint(renderer, xc + x1, yc - y1);

			y1 += 1;
			err += 1 + 2 * y1;
			if (2 * (err - x1) + 1 > 0)
			{
				x1 -= 1;
				err += 1 - 2 * x1;
			}
		}
	}
}

inline void softSprite(uint8 *spriteData, int32 x, int32 y, int32 width, int32 height, Color col = color)
{
	if ((x<0) || (x>screenWidth - 16) || (y<0) || (y>screenHeight - 16)) return;
	int32 c = col.r << 16 | col.g << 8 | col.b | 0xff000000;
	//uint8 *sprite = spriteMap[id];

	int32 index=0;
	int32 yOffset = y * screenWidth + x;
	for (int32 i = 0; i < height; i++)
	{
		for (int32 j = 0; j < width; j++)
		{
			if (spriteData[index++])
			{
				((uint32*)pixelBuffer)[yOffset + j] = c;
			}
		}
		yOffset += screenWidth;
	}
}


/* -----Textures----- */
SDL_Texture* loadTexture(char filename[])
{
	SDL_Texture* tex = IMG_LoadTexture(renderer, filename);

	if (tex == NULL)
	{
		LOG(IMG_GetError());
		return false;
	}
	return tex;
}

struct Sprite
{
	int32 xPos = 0;
	int32 yPos = 0;
	int32 width = 0;				//image dimensions
	int32 height = 0;
	int32 frameWidth = 0;			//frame dimensions in spritesheet
	int32 frameHeight = 0;
	SDL_Texture *texture = 0;		//pointer to the texture image
	
	bool Sprite::loadTexture(char filename[])
	{
		texture = IMG_LoadTexture(renderer, filename);

		if (texture == NULL)
		{
			LOG(IMG_GetError());
			return false;
		}
		else
		{
			//query texture to get it's width and height
			SDL_QueryTexture(texture, NULL, NULL, &width, &height);
		}

		return true;
	}

	bool Sprite::load(char fileName[], int32 w, int32 h)
	{
		if (loadTexture(fileName))
		{
			frameWidth = w;
			frameHeight = h;
			return true;
		}
		return false;
	}
	void Sprite::setColor(Uint8 red, Uint8 green, Uint8 blue)
	{
		//modulate texture rgb
		int32 test = SDL_SetTextureColorMod(texture, red, green, blue);
	}

	void Sprite::setBlendMode(SDL_BlendMode blending)
	{
		//set blending function
		SDL_SetTextureBlendMode(texture, blending);
	}

	void Sprite::setAlpha(Uint8 alpha)
	{
		//modulate texture alpha
		SDL_SetTextureAlphaMod(texture, alpha);
	}

	//draw texture at position x, y, with width and height
	void Sprite::draw(int32 x, int32 y, int32 w, int32 h)
	{
		//set draw area
		SDL_Rect dstRect = { x, y, w, h };

		SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	}

	//draw texture at position x, y preserving width and height
	void Sprite::draw(int32 x, int32 y)
	{
		draw(x, y, width, height);
	}

	//draw a frame from spritesheet at position x, y
	void Sprite::draw(int32 x, int32 y, int32 frame)
	{
		SDL_Rect dstRect = { x, y, frameWidth, frameHeight };
		int32 columns = width / frameWidth;

		SDL_Rect srcRect = { (frame%columns)*frameWidth, (frame / columns)*frameHeight,
							 frameWidth, frameHeight };

		SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
	}

	/*	draw texture at position x, y, with width and height
		angle -> an angle in degrees that indicates the rotation that will be applied to the destination
		flip -> flip value stating which flipping actions should be performed on the texture
				SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL
	*/
	void Sprite::draw(int32 x, int32 y, int32 w, int32 h, float64 angle, SDL_RendererFlip flip)
	{
		//set draw area
		SDL_Rect dstRect = { x ,y , w, h };

		SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, flip);
	}

	/*	frame ->	source rect in spritesheet or NULL for the entire texture
		center ->	a point indicating the point around which dstRect will be rotated 
					if NULL, rotation will be done aroud dstrect.w / 2, dstrect.h / 2
	
	*/
	void Sprite::drawEx(int32 x, int32 y, SDL_Rect* frame = NULL, float64 angle = 0.0f, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		//set draw area
		SDL_Rect dstRect = { x, y, width, height };

		//set frame draw area
		if (frame != NULL)
		{
			dstRect.w = frame->w;
			dstRect.h = frame->h;
		}

		//copy image to renderer
		SDL_RenderCopyEx(renderer, texture, frame, &dstRect, angle, center, flip);
	}

	void Sprite::clean()
	{
		//free texture
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
	}
};

inline void uploadPixels()
{
	SDL_UpdateTexture(backbufferTexture, NULL, pixelBuffer, screenWidth * sizeof(uint32));
	SDL_RenderCopy(renderer, backbufferTexture, NULL, NULL);
	//SDL_RenderPresent(renderer);
};

inline void flip()
{
	SDL_RenderPresent(renderer);
};

void print(const char *message, int x, int y, int32 fontSize = 12)
{
	int32 size = 0;
	int32 counter = 0;
	for (int32 i = 0; i < numFonts; i++)
	{
		if (fontSize == counter)
		{
			fontSize = i;
			break;
		}

		size = i;
		counter += 6;
	}
	SDL_Surface *surf = TTF_RenderText_Solid/*TTF_RenderText_Blended*/(font[size], message, SDL_Color{ (uint8)strokeColor.r, (uint8)strokeColor.g, (uint8)strokeColor.b });

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	//get width and height of texture
	int32 w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
}

void screen(int32 width, int32 height, bool32 fscreen, char *title)
{
	performanceFrequency = SDL_GetPerformanceFrequency();
	srand(SDL_GetTicks());

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	int32 tempWidth = screenWidth = width;
	int32 tempHeight = screenHeight = height;

	uint32 flags = SDL_WINDOW_SHOWN;//SDL_WINDOW_RESIZABLE
#if (_DEBUG)
	fullscreen = false;
#else
	fullscreen = true;
#endif
	fullscreen = fscreen;

	//set to zero to scale the window to desired resolution without changing the desktop resolution
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		tempWidth = 0;
		tempHeight = 0;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tempWidth, tempHeight, flags);
	if (vSync)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	else
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (fullscreen)
	{
		//scale window to desired resolution
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
	}

	backbufferTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		screenWidth, screenHeight);

	//pixelBuffer = new unsigned int[screenWidth * screenHeight];	
	pixelBuffer = (uint32 *)malloc(screenWidth * screenHeight * 4);
	//pixelBuffer = (uint32 *)VirtualAlloc(0, screenWidth * screenHeight * bytesPerPixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	
	SDL_Surface *surface = IMG_Load("data/icon.png");
	SDL_SetWindowIcon(window, surface);
	SDL_FreeSurface(surface);

	TTF_Init();
	int32 fontSize = 6;
	for (int32 i = 0; i < numFonts; i++)
	{
		font[i] = TTF_OpenFont("data/verdana.ttf", fontSize);
		//printf("The font max height is: %d\n", TTF_FontHeight(font[i]));
		fontSize += 6;
	}

	controllerHandle = SDL_GameControllerOpen(0);
	SDL_GetMouseState(&mouseX, &mouseY);

	for (int32 i = 0; i <= 2; i++)
	{
		mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
	}

	srand(SDL_GetTicks());
	
	SDL_SetRenderDrawColor(renderer, 0x65, 0x9C, 0xEF, 255);
	SDL_RenderClear(renderer);
}

void quit()
{
	shutdown();
	free(pixelBuffer);
	SDL_GameControllerClose(controllerHandle);
	for (int32 i = 0; i < numFonts; i++)
	{
		TTF_CloseFont(font[i]);
	}
	
	TTF_Quit();
	SDL_DestroyTexture(backbufferTexture);
	//SDL_DestroyTexture(particleTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int getWindowRefreshRate()
{
	//get refresh rate
	SDL_DisplayMode mode;
	int32 defaultRefreshRate = 60;

	//if we can't get the rate return the deafult one
	if (SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(window), &mode) != 0)
		return defaultRefreshRate;

	if (mode.refresh_rate == 0)
		return defaultRefreshRate;

	return mode.refresh_rate;
}

int main(int argc, char** argv)
{
	//Check for memory leak if debug build
#if defined(_DEBUG)        
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif	

	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "", "", NULL);
	setup();

	const float64 MAX_FRAME_TIME = 1000 / 60;
	int32 refreshRate = getWindowRefreshRate();
	uint32 totalFrames = 0;
	uint64 lastCounter = SDL_GetPerformanceCounter();
	uint64 lastCycleCount = __rdtsc();
	SDL_Event event;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_MOUSEMOTION:
				{
					//NOTE: this works in windowed and fullscreen mode
					mouseX = event.button.x;
					mouseY = event.button.y;
				} break;
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_RETURN:
						{
							if (fullscreen)
							{
								SDL_SetWindowFullscreen(window, SDL_FALSE);
								fullscreen = false;
							}
							else
							{
								SDL_SetWindowFullscreen(window, SDL_TRUE);
								fullscreen = true;
							}
						}
					} break;
				} break;

				case SDL_KEYUP:
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
						running = false;
					if (event.key.keysym.sym == SDLK_s)
					{
						//SOIL_save_screenshot("screenshot.bmp", SOIL_SAVE_TYPE_BMP, 0, 0, screenWidth, screenHeight);
						//LOG("Screenshot saved!\n");
					}
				} break;

				case SDL_QUIT:
				{
					running = false;
				} break;
			}
		}
		
		if (SDL_GameControllerGetAttached(controllerHandle))
		{
			joyUp = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_UP);
			joyDown = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			joyLeft = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			joyRight = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
			joyStart = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_START);
			joyBack = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_BACK);
			joyLeftShoulder = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
			joyRightShoulder = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			joyAButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_A);
			joyBButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_B);
			joyXButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_X);
			joyYButton = SDL_GameControllerGetButton(controllerHandle, SDL_CONTROLLER_BUTTON_Y);

			joyStickX = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_LEFTX);
			joyStickY = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_LEFTY);
			joyRightStickX = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_RIGHTX);
			joyRightStickY = SDL_GameControllerGetAxis(controllerHandle, SDL_CONTROLLER_AXIS_RIGHTY);
		}

		keystates = SDL_GetKeyboardState(NULL);
		//SDL_GetGlobalMouseState(&mouseX, &mouseY); //this dosen't work in fullscren with SDL_RenderSetLogicalSize
		/*
		for (int32 i = 0; i <= 3; i++)
		{
			mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
		}*/
		mouseReleased = false;
		for (int i = 1; i <= 3; i++)
		{
			mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
		
				if (mouseIsPressed && mouseButton[i])
				{
					mouseReleased = false;
				}
				else if (mouseIsPressed && !mouseButton[1] && !mouseButton[2] && !mouseButton[3])
				{
					mouseReleased = true;
				}
		
		
		}

		mouseIsPressed = false;
		for (int i = 1; i <= 3; i++)
		{
			mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
			if (mouseButton[i])
			{
				//mouseClick = true;
				mouseIsPressed = true;
			}
		}
		if (mouseIsPressed && (mouseX != prevMouseX || mouseX != prevMouseX))
			mouseDragged = true;
		else
			mouseDragged = false;

		/*mouseIsPressed = false;
		for (int32 i = 1; i <= 3; i++)
		{
			mouseButton[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i);
			if (mouseButton[i])
				mouseIsPressed = true;
		}*/

		/* the time it takes to draw one frame */
		uint32 timeDelta = SDL_GetTicks() - globalTime;
		if (timeDelta > MAX_FRAME_TIME)
			timeDelta = (uint32)MAX_FRAME_TIME;
		globalTime = SDL_GetTicks();

		updateAndDraw(timeDelta);

		//save the mouse coordinates for the previous frame, this is used for mousedrag
		prevMouseX = mouseX;
		prevMouseY = mouseX;

		uint64 endCycleCount = __rdtsc();
		uint64 endCounter = SDL_GetPerformanceCounter();
		uint64 counterElapsed = endCounter - lastCounter;
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;

		float64 msPerFrame = (((1000.0f * (float64)counterElapsed) / (float64)performanceFrequency));
		float64 fps = (float64)performanceFrequency / (float64)counterElapsed;
		float64 megaCyclesPerFrame = ((float64)cyclesElapsed / (1000.0f * 1000.0f));
		
#if defined(_DEBUG)        
		char message[256];
		sprintf_s(message, "%.03fms, %.03fFPS, %.03fMEGAc/f, RefreshRate: %d\0", msPerFrame, fps, megaCyclesPerFrame, refreshRate);
		//NOTE: temporary solution to the switch to fullscreen problem
		if (fullscreen)
			print(message, 1, screenHeight+200, 12);
		else
			print(message, 1, screenHeight-20, 12);

		//SDL_SetWindowTitle(window, message);
		totalFrames++;
#endif
		//update screen
		//uploadPixels();
		SDL_RenderPresent(renderer);

		lastCycleCount = endCycleCount;
		lastCounter = endCounter;
	}
	quit();
	return 0;
}

