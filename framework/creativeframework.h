#pragma once
#include <stdint.h>

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
	ColorRGB red{ 255,0,0 };
	ColorRGB green{ 0,255,0 };
	ColorRGB blue{ 0,0,255 };
	ColorRGB yellow{ 255,255,0 };
	ColorRGB cyan{ 0,255,255 };
	ColorRGB magenta{ 255,0,255 };
	ColorRGB purple{ 128, 0, 128 };
	ColorRGB orange{ 255,127,50 };
	ColorRGB cornflowerblue{ 101, 156, 239 };
	ColorRGB gray{ 128, 128, 128 };
	ColorRGB grey{ 192, 192, 192 };
	ColorRGB maroon{ 128, 0, 0 };
	ColorRGB darkgreen{ 0, 128, 0 };
	ColorRGB navy{ 0, 0, 128 };
	ColorRGB teal{ 0, 128, 128 };
	ColorRGB olive{ 128, 128, 0 };
}
