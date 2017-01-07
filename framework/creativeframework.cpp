#include <stdint.h>
#include <stdlib.h> //srand
#include <crtdbg.h> //memory leak check

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#define PI32 3.14159265359f
#define SWAP(x, y)\
		x = x + y; \
		y = x - y; \
		x = x - y;

#if defined(_DEBUG)
#include <iostream>
#define LOG(x) std::cout << x << std::endl;
#define LOGERROR(x)\
				std::cout << x << std::endl; \
				system("PAUSE"); \
				SDL_Quit(); \
				exit(1);
#define Assert(expression) if(!(expression)) { *(int *)0 = 0; }
#else
	#define LOG(x) 
	#define LOGERROR(x)
	#define Assert(expression)
#endif

int32 screenWidth;
int32 screenHeight; 
bool32 fullscreen = false;
bool32 running = true;
bool32 vSync = true;
uint64 performanceFrequency;		//the frequency of the performance counter in counts per seonds
uint32 *pixelBuffer;				//pixelbuffer for pixel manipulation
bool32 fillFlag = true;				//fill flag for shapes

struct ColorRGB
{
	int32 r;
	int32 g;
	int32 b;
} color{ 0xff,0xff,0xff };

namespace Color
{
	ColorRGB black{ 0,0,0 };
	ColorRGB white{ 255,255,255 };
	ColorRGB c64red{ 104, 55, 43 };
	ColorRGB c64cyan{ 112, 164, 178 };
	ColorRGB c64purple{ 111, 61, 134 };
	ColorRGB c64green{ 88, 141, 67 };
	ColorRGB c64blue{ 53, 40, 121 };
	ColorRGB c64yellow{ 184, 199, 111 };
	ColorRGB c64orange{ 111, 79, 37 };
	ColorRGB c64brown{ 67, 57, 0 };
	ColorRGB c64lightred{ 154, 103, 89 };
	ColorRGB c64darkgrey{ 68, 68, 68 };
	ColorRGB c64grey{ 108, 108, 108 };
	ColorRGB c64lightgreen{ 154, 210, 132 };
	ColorRGB c64lightblue{ 108, 94, 181 };
	ColorRGB c64lightgrey{ 149, 149, 149 };
	ColorRGB red{ 255,0,0 };
	ColorRGB green{ 0,255,0 };
	ColorRGB blue{ 0,0,255 };
	ColorRGB yellow{ 255,255,0 };
	ColorRGB cyan{ 0,255,255 };
	ColorRGB magenta{ 255,0,255 };
	ColorRGB purple{ 128, 0, 128 };
	ColorRGB gray{ 128, 128, 128 };
	ColorRGB grey{ 192, 192, 192 };
	ColorRGB maroon{ 128, 0, 0 };
	ColorRGB darkgreen{ 0, 128, 0 };
	ColorRGB navy{ 0, 0, 128 };
	ColorRGB teal{ 0, 128, 128 };
	ColorRGB olive{ 128, 128, 0 };
	ColorRGB orange{ 255,127,50 };
	ColorRGB cornflowerblue{ 101, 156, 239 };
	ColorRGB azure{ 0, 127, 255 };
	ColorRGB turquoise{ 48, 213, 200 };
	ColorRGB gold{ 255, 215, 0 };
	ColorRGB silver{ 192, 192, 192 };
	ColorRGB pink{ 255, 192, 203 };
}

int32 mouseX = 0;
int32 mouseY = 0;
uint32 mouseButton[3] = {};

enum MouseButtons
{
	LEFT,
	MIDDLE,
	RIGHT
};

void setup();
void updateAndDraw(uint32 t);

void setColor(int r, int g, int b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void setColor(ColorRGB col)
{
	color = col;
}

inline void fill()
{
	fillFlag = true;
}

inline void noFill()
{
	fillFlag = false;
}

#if SDL2
	#include "sdl2_framework.cpp"
#endif

#if WIN_32
	#include "win32_framework.cpp"
#endif


#if defined(SDL2) | defined(WINDOWS)
/* clear the pixelbuffer with specific color */
inline void clear(int32 col)
{
	//memset(pixelBuffer, color, screenWidth * screenHeight * 4);
	void *memory = pixelBuffer;
	int32 count = screenWidth * screenHeight;

	/* memory fill */
	_asm
	{
		cld								//clear direction flag
		mov edi, memory					//move pixelBuffer pointer into edi
		mov ecx, count					//move loop counter into ecx
		mov eax, col					//move color into eax
		rep stosd						//fill memory
	}
}

inline void clear(uint8 r, uint8 g, uint8 b)
{
	uint32 col = r << 16 | g << 8 | b | 0xff000000;
	void *memory = pixelBuffer;
	int32 count = screenWidth * screenHeight;

	_asm
	{
		cld
		mov edi, memory
		mov ecx, count
		mov eax, col
		rep stosd
	}
}

inline void clear(ColorRGB c)
{
	int32 col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
	void *memory = pixelBuffer;
	int32 count = screenWidth * screenHeight;

	_asm
	{
		cld
		mov edi, memory
		mov ecx, count
		mov eax, col
		rep stosd
	}
}

inline void pixel(int32 x, int32 y)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	int32 col = color.r << 16 | color.g << 8 | color.b | 0xff000000;
	pixelBuffer[y*screenWidth + x] = col;
}

inline void pixel(int32 x, int32 y, uint32 col)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	pixelBuffer[y*screenWidth + x] = col;
}

inline void pixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	int32 color = r << 16 | g << 8 | b | 0xff000000;
	pixelBuffer[y*screenWidth + x] = color;
}

inline void pixel(int32 x, int32 y, ColorRGB col)
{
	if ((x<0) || (x>screenWidth - 1) || (y<0) || (y>screenHeight - 1)) return;
	int32 c = col.r << 16 | col.g << 8 | col.b | 0xff000000;
	pixelBuffer[y*screenWidth + x] = c;
}

//draws a line with Breshenam's algorithm
inline void line(int32 x1, int32 y1, int32 x2, int32 y2)
{
	bool32 step = abs(x2 - x1) < abs(y2 - y1);

	//rotate the line
	if (step)
	{
		SWAP(x1, y1);
		SWAP(x2, y2);
	}

	if (x2 < x1)
	{
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	//Bresenham’s algorithm starts by plotting a pixel at the first coordinate of the line
	//(x1, y1), and to x+1, it takes the difference of the y component of the line to the
	//two possible y coordinates, and uses the y coordinate where the error is the smaller,
	//and repeats this for every pixel.

	real32 error = 0.0;

	//line slope
	real32 slope = (real32)abs(y2 - y1) / (x2 - x1);

	//starting point
	int32 y = y1;

	int32 ystep = (y2 > y1 ? 1 : -1);

	for (int32 i = x1; i < x2; i++)
	{
		if (step)
			pixel(y, i);
		else
			pixel(i, y);

		error += slope;

		if (error >= 0.5)
		{
			y += ystep;
			error -= 1.0;
		}
	}
}

inline void rect(int x, int y, int width, int height)
{
	if (fillFlag)
	{
		//save values
		int tempX = x;
		int tempW = width;

		while (height--)
		{
			//draw horizontal line
			while (width--)
			{
				pixel(x, y);
				x++;
			}

			//next row
			width = tempW;
			x = tempX;
			y++;
		}
	}
	else
	{
		//draw non filled rect
		line(x, y, x + width, y);
		line(x, y, x, y + height);
		line(x, y + height, x + width, y + height);
		line(x + width, y, x + width, y + height);
	}

}

//Bresenham circle algorithm
inline void circle(int32 x, int32 y, int32 radius)
{
	if (fillFlag)
	{
		//NOTE: fix-> extremly slow with windows GDI, why?
		int32 r2 = radius * radius;
		int32 area = r2 << 2;
		int32 rr = radius << 1;

		for (int32 i = 0; i < area; i++)
		{
			int32 tx = (i % rr) - radius;
			int32 ty = (i / rr) - radius;

			if (tx * tx + ty * ty <= r2)
				pixel(x + tx, y + ty);
		}
	}
	else
	{
		int32 x1 = radius;
		int32 y1 = 0;
		int32 err = 0;

		while (x1 >= y1)
		{
			pixel(x + x1, y + y1);
			pixel(x + y1, y + x1);
			pixel(x - y1, y + x1);
			pixel(x - x1, y + y1);
			pixel(x - x1, y - y1);
			pixel(x - y1, y - x1);
			pixel(x + y1, y - x1);
			pixel(x + x1, y - y1);

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
#endif