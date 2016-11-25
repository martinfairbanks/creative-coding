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

int32 screenWidth;
int32 screenHeight; 
bool32 fullscreen = false;
bool32 running = true;
bool32 vSync = false;
uint32 time;
uint64 performanceFrequency;		//the frequency of the performance counter in counts per seonds, unsigned long long

#include "sdl2_framework.cpp"
