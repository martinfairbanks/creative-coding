
	const int32 numFonts = 10;			//number of loaded fonts
	TTF_Font *font[numFonts] = {0};
	SDL_Texture *backbufferTexture = 0;		//texture used for drawing using the pixels
	

inline void uploadPixels()
{
	SDL_UpdateTexture(backbufferTexture, NULL, pixels, windowWidth * sizeof(uint32));
	SDL_RenderCopy(renderer, backbufferTexture, NULL, NULL);
	//SDL_RenderPresent(renderer);
};

#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL) 
void debugUpdateWindow()
{
	uploadPixels();
	SDL_RenderPresent(renderer);
}
#endif

inline void flip()
{
	SDL_RenderPresent(renderer);
};

int32 stringBatchCount = 0;

struct StringBatch
{
	char str[255];
	int32 x, y;
	int32 fontSize;	
	Color col;
};

StringBatch stringBatch[100];

void print(int32 x, int32 y, int32 fontSize, char *message)
{
	strcpy(stringBatch[stringBatchCount].str, message);
	stringBatch[stringBatchCount].x = x;
	stringBatch[stringBatchCount].y = y;
	stringBatch[stringBatchCount].fontSize = fontSize;
	stringBatch[stringBatchCount].col.r = strokeColor.r;
	stringBatch[stringBatchCount].col.g = strokeColor.g;
	stringBatch[stringBatchCount].col.b = strokeColor.b;
	stringBatch[stringBatchCount].col.a = strokeColor.a;
	
	stringBatchCount++;
}

void print(char *message, int32 x, int32 y, int32 fontSize = 12)
{
	print(x, y, fontSize, message);
}

void print(StringBatch str)
{
	int32 size = 0;
	int32 counter = 0;
	for (int32 i = 0; i < numFonts; i++)
	{
		if (str.fontSize == counter)
		{
			str.fontSize = i;
			break;
		}

		size = i;
		counter += 6;
	}
	SDL_Surface *surf = TTF_RenderText_Solid/*TTF_RenderText_Blended*/(font[size], str.str, SDL_Color{ (uint8)str.col.r, (uint8)str.col.g, (uint8)str.col.b });

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	//get width and height of texture
	int32 w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	SDL_Rect dst = { str.x, str.y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
}

void print(int32 x, int32 y, int32 fontSize, char *fmt, ...)
{
	char text[256];	
	va_list args;
	
	if (fmt == NULL)
		return;		

	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);

	print(text, x, y, fontSize);
}

void print(String message, int32 x, int32 y, int32 fontSize = 12)
{
	print(message.text, x, y, fontSize);
}


/* -----Textures----- */

SDL_Texture* loadSTBImageToTexture(char *filename)
{
	int32 x, y, comp;
	unsigned char *data;
	uint32_t rmask, gmask, bmask, amask;
	SDL_Surface *rv;

	FILE *file = fopen(filename, "rb");
	if (!file)
		return 0;

	data = stbi_load_from_file(file, &x, &y, &comp, 0);
	fclose(file);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	if (comp == 4)
		rv = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);
	else if (comp == 3)
		rv = SDL_CreateRGBSurface(0, x, y, 24, rmask, gmask, bmask, 0);
	else 
	{
		stbi_image_free(data);
		return 0;
	}

	memcpy(rv->pixels, data, comp * x * y);
	stbi_image_free(data);
	
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, rv);
	SDL_FreeSurface(rv);

	return texture;
}

SDL_Texture* loadTexture(char filename[])
{
	SDL_Texture* tex = loadSTBImageToTexture(filename);

	if (tex == NULL)
	{
		printLogError("Failed to load image.");
		return false;
	}
	return tex;
}

struct Sprite
{
	int32 width = 0;				//image dimensions
	int32 height = 0;
	int32 frameWidth = 0;			//frame dimensions in spritesheet
	int32 frameHeight = 0;
	SDL_Texture *texture = 0;		//pointer to the texture image
	real32 spriteScale = 1.0f;

	int32 frameTime;
	int32 numFrames = 0;
	int32 currentFrame = 0;
	real32 timeToUpdate = 0;
	bool32 animationDone = false;
	SDL_Rect sourceRect;
	vec2 origin{ 0, 0 };

	void setOrigin(int32 x, int32 y)
	{
		origin.x = (real32)x;
		origin.y = (real32)y;
	}

	void updateAnimation(real32 elapsedTime)
	{
		timeToUpdate += elapsedTime;
		if (timeToUpdate > frameTime)
		{
			currentFrame++;
			timeToUpdate = 0;
			animationDone = false;
			if (currentFrame < numFrames)
			{
				sourceRect.x += frameWidth;
			}
			else
			{
				sourceRect.x -= frameWidth * (numFrames - 1);
				currentFrame = 0;
				animationDone = true;
			}
		}
	}

	void drawAnimation(int32 x, int32 y, real32 angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect dstRect;
		dstRect.x = x;
		dstRect.y = y;
		dstRect.w = int32(sourceRect.w * spriteScale);
		dstRect.h = int32(sourceRect.h * spriteScale);

		SDL_RenderCopyEx(renderer, texture, &sourceRect, &dstRect, angle, NULL, flip);

	}

	void drawAnimation(int32 x, int32 y, SDL_Rect srcRect, real32 angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect dstRect;
		dstRect.x = (int32)(x - origin.x);
		dstRect.y = (int32)(y - origin.y);
		dstRect.w = (int32)(srcRect.w * spriteScale);
		dstRect.h = (int32)(srcRect.h * spriteScale);

		SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, angle, NULL, flip);
	}

	bool loadTexture(char filename[])
	{
		//TODO: replace this with stb_image
		texture = loadSTBImageToTexture(filename);

		if (texture == NULL)
		{
			printLogError("Failed to load image.");
			return false;
		}
		else
		{
			//query texture to get it's width and height
			SDL_QueryTexture(texture, NULL, NULL, &width, &height);
		}

		return true;
	}

	bool loadTextureSpriteSheet(char *filename, int sourceX, int sourceY,
		int w, int h, int fps, int num_frames, real32 scale = 1.0f)
	{

		frameTime = fps;
		numFrames = num_frames;
		currentFrame = 0;
		timeToUpdate = 0;
		animationDone = false;
		spriteScale = scale;

		//mySprite.loadTextureSpriteSheet("data/cavestory/MyChar.png", 0, 0, 16, 16, x, y, 100)
		//TODO: replace this with stb_image
		texture = loadSTBImageToTexture(filename);

		frameWidth = w;
		frameHeight = h;
		sourceRect.x = sourceX;
		sourceRect.y = sourceY;
		sourceRect.w = w;
		sourceRect.h = h;

		if (texture == NULL)
		{
			printLogError("Failed to load image.");
			return false;
		}
		else
		{
			//query texture to get it's width and height
			SDL_QueryTexture(texture, NULL, NULL, &width, &height);
		}

		return true;
	}

	bool load(char fileName[], int32 w, int32 h)
	{
		if (loadTexture(fileName))
		{
			frameWidth = w;
			frameHeight = h;
			return true;
		}
		return false;
	}
	void setColor(uint8 r, uint8 g, uint8 b)
	{
		//modulate texture rgb
		SDL_SetTextureColorMod(texture, r, g, b);
	}

	void setBlendMode(SDL_BlendMode blending)
	{
		//set blending function
		SDL_SetTextureBlendMode(texture, blending);
	}

	void setAlpha(uint8 alpha)
	{
		//modulate texture alpha
		SDL_SetTextureAlphaMod(texture, alpha);
	}

	//draw texture at position x, y, with width and height
	void draw(int32 x, int32 y, int32 w, int32 h)
	{
		//set draw area
		SDL_Rect dstRect = { x, y, w, h };

		SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	}

	//draw texture at position x, y preserving width and height
	void draw(int32 x, int32 y)
	{
		draw(x, y, width, height);
	}

	//draw a frame from spritesheet at pixel position x, y
	void draw(int32 x, int32 y, int32 frame, real32 angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect dstRect = { x, y, int32(frameWidth*spriteScale), int32(frameHeight*spriteScale) };
		int32 columns = width / frameWidth;

		SDL_Rect srcRect = { (frame%columns)*frameWidth, (frame / columns)*frameHeight,
			frameWidth, frameHeight };

		SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, angle, NULL, flip);
	}

	/*	draw texture at position x, y, with width and height
	angle -> an angle in degrees that indicates the rotation that will be applied to the destination
	flip -> flip value stating which flipping actions should be performed on the texture
	SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL
	*/
	void draw(int32 x, int32 y, int32 w, int32 h, real64 angle, SDL_RendererFlip flip)
	{
		//set draw area
		SDL_Rect dstRect = { x ,y , w, h };

		SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, flip);
	}

	/*	frame ->	source rect in spritesheet or NULL for the entire texture
	center ->	a point indicating the point around which dstRect will be rotated
	if NULL, rotation will be done aroud dstrect.w / 2, dstrect.h / 2
	*/
	void drawEx(int32 x, int32 y, SDL_Rect* frame = NULL, real64 angle = 0.0f, SDL_Point* cnt = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
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
		SDL_RenderCopyEx(renderer, texture, frame, &dstRect, angle, cnt, flip);
	}

	void drawTile(int32 x, int32 y, uint32 tilenumber)
	{
		SDL_Rect destRect;
		destRect.x = x*frameWidth;
		destRect.y = y*frameHeight;;
		destRect.w = frameWidth;
		destRect.h = frameHeight;

		//how many colums the tilemap image has 
		int32 columns = width / frameWidth;

		SDL_Rect srcRect;
		srcRect.x = (tilenumber % columns);
		srcRect.x = sourceRect.x*frameWidth;
		srcRect.y = (tilenumber / columns)*frameHeight;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		//SDL_RenderPresent(renderer);
	}

	void clean()
	{
		//free texture
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
	}
};

/* simple rect to rect collision check */
inline bool rectCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	if (rect1.x >= rect2.x + rect2.w)
		return false;

	if (rect1.y >= rect2.y + rect2.h)
		return false;

	if (rect2.x >= rect1.x + rect1.w)
		return false;

	if (rect2.y >= rect1.y + rect1.h)
		return false;

	return true;
}


#if 1
/* SDL drawing functions */
inline void fill()
{
	state.fillFlag = true;
}

inline void noFill()
{
	state.fillFlag = false;
}

inline void clear_(Color col)
{
	SDL_SetRenderDrawColor(renderer, (uint8)col.r, (uint8)col.g, (uint8)col.b, 255);
	SDL_RenderClear(renderer);
}

inline void setColor_(uint8 r, uint8 g, uint8 b, uint8 a = 255)
{
	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

inline void setColor_(Color col)
{
	strokeColor = col;
	SDL_SetRenderDrawColor(renderer, (uint8)col.r, (uint8)col.g, (uint8)col.b, (uint8)col.a);
}

inline void pixel_(int32 x, int32 y)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

inline void pixel_(int32 x, int32 y, uint32 col)
{
	uint8 r = (uint8)(col >> 16);
	uint8 g = (uint8)(col  >> 8);
	uint8 b = (uint8)col;
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
	SDL_RenderDrawPoint(renderer, x, y);
	//restore color to the one set with setColor
	SDL_SetRenderDrawColor(renderer, (uint8)strokeColor.r, (uint8)strokeColor.g, (uint8)strokeColor.b, 0xff);
}

inline void pixel_(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
	SDL_RenderDrawPoint(renderer, x, y);
	//restore color to the one set with setColor
	SDL_SetRenderDrawColor(renderer, (uint8)strokeColor.r, (uint8)strokeColor.g, (uint8)strokeColor.b, 0xff);
}

inline void pixel_(int32 x, int32 y, Color col)
{
	SDL_SetRenderDrawColor(renderer, (uint8)col.r, (uint8)col.g, (uint8)col.b, 0xff);
	SDL_RenderDrawPoint(renderer, x, y);
	//restore color to the one set with setColor
	SDL_SetRenderDrawColor(renderer, (uint8)strokeColor.r, (uint8)strokeColor.g, (uint8)strokeColor.b, 0xff);
}

inline void line_(int32 x1, int32 y1, int32 x2, int32 y2)
{
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

inline void rect_(int32 x, int32 y, int32 width, int32 height)
{
	SDL_Rect rect = { x, y, width, height };
	if (state.fillFlag)
		SDL_RenderFillRect(renderer, &rect);
	else
		SDL_RenderDrawRect(renderer, &rect);
}

void fillCircleData_(int32 xc, int32 yc, int32 p, int32 pb, int32 pd, int32 radius)
{
	int32 a, b, c, d, e, f, g, h;
	int32 x = 0;
	int32 y = radius;
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

void circle_(int32 xc, int32 yc, int32 radius)
{
	if (xc + radius < 0 || xc - radius >= windowWidth || yc + radius < 0 || yc - radius >= windowHeight) return;

	if (state.fillFlag)
	{

		int32 p = 3 - (radius << 1);
		int32 pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times  (ensure initial value is outside the range)
		if (state.lineWidth >= 1)
		{
			fillCircleData_(xc, yc, p, pb, pd, radius);

			radius -= state.lineWidth;

			p = 3 - (radius << 1);
			pb = yc + radius + 1, pd = yc + radius + 1;
			fillCircleData_(xc, yc, p, pb, pd, radius);
		}
		else
		{
			fillCircleData_(xc, yc, p, pb, pd, radius);
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

//loads window pixel data into the pixels array
//don't work when switching to fullscreen
/*void loadPixels()
{
	SDL_RenderReadPixels(renderer, &windowSurf->clip_rect, windowSurf->format->format,
		pixels, windowSurf->w * windowSurf->format->BytesPerPixel);

	//SDL_FreeSurface(windowSurface);
	//windowSurface = NULL;
}*/
#endif