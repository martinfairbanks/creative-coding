/* TODO:	mousepress and keypress up/down, scroll wheel
			line blending
			x64 build
			batch build and make file
			translate(x, y) -> change pixel functions


*/

#include <stdint.h>
#include <stdlib.h> //srand
#include <stdio.h>
#include <crtdbg.h> //memory leak check
#include <math.h>
#include <float.h>
#include "dependencies\slang_library_noise.cpp"

//TODO: this is for the gaussian random numbers, fix this with my own gaussian calculations?
#include <random>
std::default_random_engine randomGenerator;

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

//#define FRAMEWORK_INTERNAL

#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL)
#include <iostream>
#define LOG(x) std::cout << x << std::endl;
#define LOGERROR(x)\
				std::cout << x << std::endl; \
				system("PAUSE"); \
				SDL_Quit(); \
				exit(1);
#define Assert(expression) if(!(expression)) { *(int *)0 = 0; }
#define _CRT_SECURE_NO_DEPRECATE
#else
	#define LOG(x) 
	#define LOGERROR(x)
	#define Assert(expression)
#endif

int32 windowWidth;
int32 windowHeight; 
bool32 fullscreen = false;
bool32 running = true;
bool32 vSync = true;
uint64 performanceFrequency;		//the frequency of the performance counter in counts per seonds
uint32 *pixelBuffer;				//pixelbuffer for pixel manipulation
bool32 fillFlag = true;				//fill flag for shapes
int32 lineWidth = 1;				//strokeweight

void setup();
void updateAndDraw(uint32 t);
void shutdown();

/* ----- Input ----- */
int32 mouseX = 0;
int32 mouseY = 0;
int32 prevMouseX = 0;				//mouse coords for previous frame
int32 prevMouseY = 0;

uint32 mouseButton[4] = {};

bool32 mouseIsPressed = false;
bool32 mouseReleased = false;
bool32 mouseDragged = false;

enum MouseButtons
{
	LEFT = 1,
	MIDDLE,
	RIGHT
};

/* ----- Math ----- */
#define PI 3.14159265358979323846
#define TWO_PI 6.28318530717958647693
#define SWAP(x, y)\
		x = x + y; \
		y = x - y; \
		x = x - y;

struct vec2
{
	real32 x;
	real32 y;
	vec2() { };
	vec2(real32 X, real32 Y) : x(X), y(Y) {}

	//overloaded operator for vector addition: v3 = v1 + v2;
	vec2 operator+(const vec2& v2) const
	{
		return vec2(x + v2.x, y + v2.y);
	}

	//v1 += v2;
	friend vec2& operator+=(vec2& v1, const vec2& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	//subtracting two vectors
	vec2 operator-(const vec2& v2) const
	{
		return vec2(x - v2.x, y - v2.y);
	}

	friend vec2& operator-=(vec2& v1, const vec2& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	//multiply by a scalar number
	vec2 operator*(real32 scalar)
	{
		return vec2(x * scalar, y * scalar);
	}

	vec2& operator*=(real32 scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	//divide by a scalar number
	vec2 operator/(real32 scalar)
	{
		return vec2(x / scalar, y / scalar);
	}

	vec2& operator/=(real32 scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	//calculate length (magnitude) of the vector, square root of x^2 + y^2
	//The Pythagorean Theorem
	real32 length() { return sqrt(x * x + y * y); }

	//calculate the angle
	real32 angle() { return atan2(y, x); }

	//set length of vector
	void setMag(real32 length)
	{
		real32 a = angle();
		x = cos(a) * length;
		y = sin(a) * length;
	}

	void setAngle(real32 angle)
	{
		real32 l = length();
		x = cos(angle) * l;
		y = sin(angle) * l;
	}

	//limit the magnitude the vector
	void limit(real32 max)
	{
		if (length() > max)
			setMag(max);
	}

	//normalizing a vector makes its length equal to 1.
	//to normalize a vector - multiply it by the inverse of its length
	void normalize()
	{
		real32 l = length();
		if (l > 0) //avoid divide by 0
		{
			(*this) *= 1 / l;
		}
	}
};

struct vec3
{
	real32 x;
	real32 y;
	real32 z;

	vec3() { };
	vec3(real32 X, real32 Y, real32 Z) : x(X), y(Y), z(Z) {}
	//overloaded operator for vector addition: v3 = v1 + v2;
	vec3 operator+(const vec3& v2) const
	{
		return vec3(x + v2.x, y + v2.y, z + v2.z);
	}

	//v1 += v2;
	friend vec3& operator+=(vec3& v1, const vec3& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		return v1;
	}

	//subtracting two vectors
	vec3 operator-(const vec3& v2) const
	{
		return vec3(x - v2.x, y - v2.y, z - v2.z);
	}

	friend vec3& operator-=(vec3& v1, const vec3& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		return v1;
	}

	//multiply by a scalar number
	vec3 operator*(real32 scalar)
	{
		return vec3(x * scalar, y * scalar, z * scalar);
	}

	vec3& operator*=(real32 scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	//divide by a scalar number
	vec3 operator/(real32 scalar)
	{
		return vec3(x / scalar, y / scalar, z / scalar);
	}

	vec3& operator/=(real32 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	//calculate length (magnitude) of the vector, square root of x^2 + y^2
	//the pythagorean theorem
	real32 length() { return sqrt(x * x + y * y); }

	//normalizing a vector makes its length equal to 1.
	//to normalize a vector - multiply it by the inverse of its length
	void normalize()
	{
		real32 l = length();
		if (l > 0) //avoid divide by 0
		{
			(*this) *= 1 / l;
		}
	}
};

//pseudo-random uniform distribution of numbers
//set the random seed to constant value to return the same pseudo random numbers every time
void randomSeed(uint32 value)
{
	srand(value);
}

//random number between min and max
inline int32 random(int32 min, int32 max)
{
	/* ex:  Random(-10,20) -> will give -10 to, and including, 20. */
	max += 1;
	min -= 1;
	return int32(rand() / (real32)(RAND_MAX + 1) * (max - min) + min);
}

//random number between 0 and max
inline int32 random(int32 max)
{
//	int32 min = -1;
	max += 1;
	return int32(rand() / (real32)(RAND_MAX + 1) * (max));
}

//random float between 0 and 1
inline real32 randomf()
{
	return ((real32)rand() / (RAND_MAX));
	//random float between -1 and 1
	//return (((float)rand() / (RAND_MAX)) * 2 - 1.0f);
}

inline real32 randomf(real32 min, real32 max)
{
	return min + randomf() * (max - min);
}

//random 2d direction
inline vec2 random2d()
{
	vec2 vec{ 1.0, 1.0 };
	//vec.normalize();
	vec.setAngle(randomf() * PI * 2);
	return vec;
}

//returns a Gaussian(normal) distribution of random numbers around the mean with a specific standard deviation.
//the probability of getting values far from the mean is low, the probability of getting numbers near the mean is high.
real32 randomGaussian()
{
	//gaussian random numbers with a mean of 0 and standard deviation of 1.0
	std::normal_distribution<real32> gaussianDistribution(0.0f, 1.0f);
	return gaussianDistribution(randomGenerator);
}

real32 randomGaussian(real32 mean)
{
	std::normal_distribution<real32> gaussianDistribution(mean, 1.0f);
	return gaussianDistribution(randomGenerator);
}

real32 randomGaussian(real32 mean, real32 sd)
{
	std::normal_distribution<real32> gaussianDistribution(mean, sd);
	return gaussianDistribution(randomGenerator);
}

//1D Perlin noise, returns noise value at specified coordinate, the value is always between 0 and 1.
real32 noise(real32 x)
{
	return _slang_library_noise1(x)*0.5f + 0.5f;
}

//2D Perlin noise
real32 noise(real32 x, real32 y)
{
	return _slang_library_noise2(x, y)*0.5f + 0.5f;
}

//3D Perlin noise
real32 noise(real32 x, real32 y, real32 z)
{
	return _slang_library_noise3(x, y, z)*0.5f + 0.5f;
}

//the likelihood that a random value will be picked is equal to the the first random number (r1)
//returns a random value between 0 and 1
real32 montecarlo()
{
	//loop until we find a qualifying random number
	while (true)
	{
		real32 r1 = randomf();
		real32 probability = r1;
		//real32 probability = pow(1.0 - r1, 8);

		real32 r2 = randomf();

		if (r2 < probability)
			return r1;
	}
}

//convert radians to degrees
float degrees(real32 radians)
{

	float degrees = (radians / (2 * PI)) * 360;
	return degrees;
}

//convert degrees to radians
float radians(real32 degrees)
{
	float radian = (degrees / 360) * 2 * PI;
	return radian;
}

//calculates the distance between 2 points
real32 dist(real32 x1, real32 y1, real32 x2, real32 y2)
{
	//return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//returns a interpolated value between two numbers, the amount parameter is the amount to interpolate between the two values. 
real32 lerp(real32 start, real32 stop, real32 amount)
{
	return amount*(stop - start) + start;
};

int32 constrain(int32 value, int32 min, int32 max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

real32 constrainf(real32 value, real32 min, real32 max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

real32 mapVal(real32 value, real32 inMin, real32 inMax, real32 outMin, real32 outMax)
{
	if (fabs(inMin - inMax) < FLT_EPSILON)
	{
		return outMin;
	}
	else
	{
		real32 outVal = ((value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin);

		if (outMax < outMin)
		{
			if (outVal < outMax)outVal = outMax;
			else if (outVal > outMin)outVal = outMin;
		}
		else
		{
			if (outVal > outMax)outVal = outMax;
			else if (outVal < outMin)outVal = outMin;
		}
		
		return outVal;
	}
}

vec2 center;	//center location in window


/* File I/O */

//remember to free the memory after you called this function!
char* loadFile(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	//move pointer to the end of the file to get length
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *contents = (char *)malloc((file_length + 1) * sizeof(char));

	//clear memory
	for (int i = 0; i < file_length + 1; i++) {
		contents[i] = 0;
	}
	//read file
	fread(contents, 1, file_length, fp);
	//insert null terminator
	contents[file_length + 1] = '\0';
	fclose(fp);
	return contents;
}


/* Colors */

struct Color
{
	int32 r;
	int32 g;
	int32 b;
	Color() {};
	Color(int32 red, int32 green, int32 blue) { r = red; g = green; b = blue; }
} color{ 0xff, 0xff, 0xff };

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
//converts HSB(=HSV) color to RGB color
Color colorHSB(Color colorHSB)
{
	if (colorHSB.r < 0 || colorHSB.r > 255)
		colorHSB.r = 0;
	if (colorHSB.g < 0 || colorHSB.g > 255)
		colorHSB.g = 0;
	if (colorHSB.b < 0 || colorHSB.b > 255)
		colorHSB.b = 0;

	real32 r, g, b, h, s, v;
	h = colorHSB.r / 256.0;
	s = colorHSB.g / 256.0;
	v = colorHSB.b / 256.0;

	if (s == 0.0) r = g = b = v;
	else
	{
		real32 f, p, q, t;
		int32 i;
		h *= 6.0;
		i = int32(floor(h));
		f = h - i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s * f));
		t = v * (1.0 - (s * (1.0 - f)));

		switch (i)
		{
		case 0: r = v; g = t; b = p; break;
		case 1: r = q; g = v; b = p; break;
		case 2: r = p; g = v; b = t; break;
		case 3: r = p; g = q; b = v; break;
		case 4: r = t; g = p; b = v; break;
		case 5: r = v; g = p; b = q; break;
		}
	}
	Color col;
	col.r = int32(r * 255.0);
	col.g = int32(g * 255.0);
	col.b = int32(b * 255.0);
	//col.a = 255;
	return col;
}

Color colorHSL(Color colorHSL)
{
	real32 r, g, b, h, s, l;
	real32 temp1, temp2, tempr, tempg, tempb;
	h = (real32)colorHSL.r / 256.0f;
	s = (real32)colorHSL.g / 256.0f;
	l = (real32)colorHSL.b / 256.0f;

	if (s == 0) r = g = b = l;
	else
	{
		if (l < 0.5) temp2 = l * (1 + s);
		else temp2 = (l + s) - (l * s);
		temp1 = 2 * l - temp2;
		tempr = h + 1.0 / 3.0;
		if (tempr > 1) tempr--;
		tempg = h;
		tempb = h - 1.0 / 3.0;
		if (tempb < 0) tempb++;

		//red
		if (tempr < 1.0 / 6.0) r = temp1 + (temp2 - temp1) * 6.0 * tempr;
		else if (tempr < 0.5) r = temp2;
		else if (tempr < 2.0 / 3.0) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
		else r = temp1;

		//green
		if (tempg < 1.0 / 6.0) g = temp1 + (temp2 - temp1) * 6.0 * tempg;
		else if (tempg < 0.5) g = temp2;
		else if (tempg < 2.0 / 3.0) g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
		else g = temp1;

		//blue
		if (tempb < 1.0 / 6.0) b = temp1 + (temp2 - temp1) * 6.0 * tempb;
		else if (tempb < 0.5) b = temp2;
		else if (tempb < 2.0 / 3.0) b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
		else b = temp1;
	}

	Color col;
	col.r = int32(r * 255.0);
	col.g = int32(g * 255.0);
	col.b = int32(b * 255.0);
	//col.a = 255;
	return col;
}

#if defined(SDL2)
	#include "sdl2_framework.cpp"
#endif

#if defined(WIN_32)
	#include "win32_framework.cpp"
#endif

#if defined(OPENGL) || defined(OPENGL4)
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

struct ColorRGBA
{
	real32 r;
	real32 g;
	real32 b;
	real32 a;
};

ColorRGBA strokeColor{ 0.0f, 0.0f, 0.0f, 1.0f };
ColorRGBA fillColor{ 0.0f, 0.0f, 0.0f, 1.0f };
/*
static ColorRGBA colorfromhex(const unsigned int hex)
{
	return ColorRGBA{ real32((hex >> 16) & 0xFF) / 255.0f, real32((hex >> 8) & 0xFF) / 255.0f, real32((hex >> 0) & 0xFF) / 255.0f, 1.0f };
}

const ColorRGBA black{ 0.0f, 0.0f, 0.0f, 1.0f };
const ColorRGBA white{ 1.0f, 1.0f, 1.0f, 1.0f };
const ColorRGBA red{ 1.0f, 0.0f, 0.0f, 1.0f };
const ColorRGBA green{ 0.0f , 1.0f, 0.0f, 1.0f };
const ColorRGBA blue{ 0.0f, 0.0f, 1.0f, 1.0f };
const ColorRGBA yellow{ 1.0f, 1.0f, 0.0f, 1.0f };
const ColorRGBA cyan{ 0.0f, 1.0f, 1.0f, 1.0f };
const ColorRGBA magenta{ 1.0f, 0.0f, 1.0f, 1.0f };
const ColorRGBA cornflowerblue = colorfromhex(0x6495ED);
const ColorRGBA azure = colorfromhex(0xF0FFFF);
const ColorRGBA turquoise = colorfromhex(0x40E0D0);
const ColorRGBA gold = colorfromhex(0xFFD700);
const ColorRGBA silver = colorfromhex(0xC0C0C0);
const ColorRGBA pink = colorfromhex(0xFFC0CB);
const ColorRGBA teal = colorfromhex(0x008080);
*/
#endif

#if defined(OPENGL)
	#include "opengl_framework.cpp"
#endif

#if defined(OPENGL4)
	#include "opengl4_framework.cpp"
#endif


#if defined(SDL2) | defined(WIN_32)

/*
void setColor(int32 r, int32 g, int32 b)
{
	if (colorModeFlag == RGB)
	{
		color.r = r;
		color.g = g;
		color.b = b;
	}
	else if (colorModeFlag == HSB)
	{
		color = colorHSB(Color{ r, g, b });
	}
	else if (colorModeFlag == HSL)
	{
		color = colorHSL(Color{ r, g, b });
	}
}

void setColor(Color col)
{
	if (colorModeFlag == RGB)
	{
		color = col;
	}
	else if(colorModeFlag == HSB)
	{
		color = colorHSB(Color{ col.r, col.g, col.b });
	}
	else if (colorModeFlag == HSL)
	{
		color = colorHSL(Color{ col.r, col.g, col.b });
	}
}

void setColor(uint8 col)
{
	if (colorModeFlag == RGB)
	{
		color.r = col;
		color.g = col;
		color.b = col;
	}
	else if (colorModeFlag == HSB)
	{
		color = colorHSB(Color{ col, col, col });
	}
	else if (colorModeFlag == HSL)
	{
		color = colorHSL(Color{ col, col, col });
	}
}
*/


void fill(int32 r, int32 g, int32 b, int32 alpha = 255)
{
	Color tempCol;
	if (colorModeFlag == RGB)
	{
		tempCol.r = r;
		tempCol.g = g;
		tempCol.b = b;
	}
	else if (colorModeFlag == HSB)
	{
		tempCol = colorHSB(Color{ r, g, b });
	}
	else if (colorModeFlag == HSL)
	{
		tempCol = colorHSL(Color{ r, g, b });
	}
	fillColor.r = tempCol.r;
	fillColor.g = tempCol.g;
	fillColor.b = tempCol.b;
	fillColor.a = alpha;
}

void fill(Color col, int32 alpha = 255)
{
	Color tempCol;
	if (colorModeFlag == RGB)
	{
		tempCol = col;
	}
	else if (colorModeFlag == HSB)
	{
		tempCol = colorHSB(Color{ col.r, col.g, col.b });
	}
	else if (colorModeFlag == HSL)
	{
		tempCol = colorHSL(Color{ col.r, col.g, col.b });
		
	}
	fillColor.r = tempCol.r;
	fillColor.g = tempCol.g;
	fillColor.b = tempCol.b;
	fillColor.a = alpha;
}

void fill(ColorRGBA col)
{

	if (colorModeFlag == HSB || colorModeFlag == HSL)
	{
		Color tempCol;
		if (colorModeFlag == HSB)
		{
			tempCol = colorHSB(Color{ col.r, col.g, col.b });
		}
		else if (colorModeFlag == HSL)
		{
			tempCol = colorHSL(Color{ col.r, col.g, col.b });

		}
		fillColor.r = tempCol.r;
		fillColor.g = tempCol.g;
		fillColor.b = tempCol.b;
		fillColor.a = col.a;
	}

	if (colorModeFlag == RGB)
	{
		fillColor = col;
	}
}

void fill(uint8 col, int32 alpha = 255)
{
	Color tempCol;
	if (colorModeFlag == RGB)
	{
		tempCol.r = col;
		tempCol.g = col;
		tempCol.b = col;
	}
	else if (colorModeFlag == HSB)
	{ 
		tempCol = colorHSB(Color{ col, col, col });
		
	}
	else if (colorModeFlag == HSL)
	{
		tempCol = colorHSL(Color{ col, col, col });
	}
	fillColor.r = tempCol.r;
	fillColor.g = tempCol.g;
	fillColor.b = tempCol.b;
	fillColor.a = alpha;
}


void stroke(int32 r, int32 g, int32 b, int32 alpha = 255)
{
	Color tempCol;
	if (colorModeFlag == RGB)
	{
		tempCol.r = r;
		tempCol.g = g;
		tempCol.b = b;
		
	}
	else if (colorModeFlag == HSB)
	{
		tempCol = colorHSB(Color{ r, g, b });

	}
	else if (colorModeFlag == HSL)
	{
		tempCol = colorHSL(Color{ r, g, b });
	}

	strokeColor.r = tempCol.r;
	strokeColor.g = tempCol.g;
	strokeColor.b = tempCol.b;
	strokeColor.a = alpha;
}

void stroke(Color col, int32 alpha = 255)
{
	Color tempCol;
	if (colorModeFlag == RGB)
	{
		tempCol = col;

	}
	else if (colorModeFlag == HSB)
	{
		tempCol = colorHSB(Color{ col.r, col.g, col.b });
		
	}
	else if (colorModeFlag == HSL)
	{
		tempCol = colorHSL(Color{ col.r, col.g, col.b });
		
	}
	strokeColor.r = tempCol.r;
	strokeColor.g = tempCol.g;
	strokeColor.b = tempCol.b;
	strokeColor.a = alpha;
}


void stroke(uint8 col, int32 alpha = 255)
{
	Color tempCol;
	if (colorModeFlag == RGB)
	{
		tempCol.r = col;
		tempCol.g = col;
		tempCol.b = col;
	}
	else if (colorModeFlag == HSB)
	{
		tempCol = colorHSB(Color{ col, col, col });
	}
	else if (colorModeFlag == HSL)
	{
		tempCol = colorHSL(Color{ col, col, col });
	}
	strokeColor.r = tempCol.r;
	strokeColor.g = tempCol.g;
	strokeColor.b = tempCol.b;
	strokeColor.a = alpha;
}

int32 color32(int32 r, int32 g, int32 b)
{
	/*if (r > 255)
	r = 255;
	if (g > 255)
	g = 255;
	if (b > 255)
	b = 255;*/
	/*if (r < 0)
	r = 0;
	if (g < 0)
	g = 0;
	if (b < 0)
	b = 0;*/
	int32 color = r << 16 | g << 8 | b;
	return color;
}

//set lineWidth on shapes and lines
void strokeWeight(int32 value)
{
	lineWidth = value;
}

void noStroke()
{
	lineWidth = 0;
}

inline void clear(uint8 inCol)
{
	uint32 col;
	Color c;
	switch (colorModeFlag)
	{
	case HSB:
		c = colorHSB(Color{ (int32)inCol, (int32)inCol, (int32)inCol });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	case HSL:
		c = colorHSL(Color{ (int32)inCol, (int32)inCol, (int32)inCol });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	default:	//RGB
		col = inCol << 16 | inCol << 8 | inCol | 0xff000000;
		break;
	}

	memset(pixelBuffer, col, windowWidth * windowHeight * 4);
}

inline void clear(uint8 r, uint8 g, uint8 b)
{
	int32 col;
	Color c;
	switch (colorModeFlag)
	{
	case HSB:
		c = colorHSB(Color{ r, g, b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	case HSL:
		c = colorHSL(Color{ r, g, b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	default:	//RGB
		col = r << 16 | g << 8 | b;// | 0xff000000;
		break;
	}
	//memset(pixelBuffer, col, (windowWidth * windowHeight));
	
	//x86 build only
	void *memory = pixelBuffer;
	int32 count = windowWidth * windowHeight;

	_asm
	{
		cld
		mov edi, memory
		mov ecx, count
		mov eax, col
		rep stosd
	}
}

inline void clear(Color inCol)
{
	uint32 col;
	Color c;
	switch (colorModeFlag)
	{
	case HSB:
		c = colorHSB(Color{ inCol.r, inCol.g, inCol.b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	case HSL:
		c = colorHSL(Color{ inCol.r, inCol.g, inCol.b });
		col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
		break;
	default:	//RGB
		col = inCol.r << 16 | inCol.g << 8 | inCol.b | 0xff000000;
		break;
	}
	//memset(pixelBuffer, col, windowWidth * windowHeight * 4);

	//x86 build only
	void *memory = pixelBuffer;
	int32 count = windowWidth * windowHeight;
	_asm
	{
		cld
		mov edi, memory
		mov ecx, count
		mov eax, col
		rep stosd
	}
}

//blending
void plotBlendedPixel(int32 x, int32 y, ColorRGBA col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	real32 a = col.a / 255.0f; //to get a number between 0.0-1.0
	real32 sR = (real32)col.r;
	real32 sG = (real32)col.g;
	real32 sB = (real32)col.b;

	uint32 tempcol = pixelBuffer[y*windowWidth + x];

	real32 dR = (tempcol >> 16) & 0xff;
	real32 dG = (tempcol >> 8) & 0xff;
	real32 dB = (tempcol >> 8) & 0xff;


	//linear blend
	real32 r = (1.0f - a)*dR + a*sR;
	real32 g = (1.0f - a)*dG + a*sG;
	real32 b = (1.0f - a)*dB + a*sB;

	pixelBuffer[y*windowWidth + x] = (((uint32)(r + 0.5f) << 16) |
		((uint32)(g + 0.5f) << 8) |
		((uint32)(b + 0.5f) << 0));
}

inline void pixel(int32 x, int32 y)
{
	if ((x < 0) || (x > windowWidth - 1) || (y < 0) || (y > windowHeight - 1)) return;
	//alpha blending on
	if (strokeColor.a < 255)
		plotBlendedPixel(x, y, strokeColor);
	else
	{
		int32 col = strokeColor.r << 16 | strokeColor.g << 8 | strokeColor.b | 0xff000000;
		pixelBuffer[y*windowWidth + x] = col;

	}
}
inline void pixel(int32 x, int32 y, uint32 col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	pixelBuffer[y*windowWidth + x] = col;
}

inline void pixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	int32 color = r << 16 | g << 8 | b | 0xff000000;
	pixelBuffer[y*windowWidth + x] = color;
}

inline void pixel(int32 x, int32 y, ColorRGBA col)
{
	if ((x<0) || (x>windowWidth - 1) || (y<0) || (y>windowHeight - 1)) return;
	
	//alpha blending on
	if (col.a < 255)
		plotBlendedPixel(x, y, col);
	else
	{
		int32 c = col.r << 16 | col.g << 8 | col.b | 0xff000000;
		pixelBuffer[y*windowWidth + x] = c;
	}
}

static void hzLine(int y, int x1, int x2, ColorRGBA c)
{
	if (x2 < x1) { x1 += x2; x2 = x1 - x2; x1 -= x2; } //swap x1 and x2
	if (x2 < 0 || x1 >= windowWidth || y < 0 || y >= windowHeight) return;
	if (x1 < 0) x1 = 0; //clip
	if (x2 >= windowWidth) x2 = windowWidth - 1; //clip

	for (int x = x1; x <= x2; x++)
	{
		//pixelBuffer[(y/* + thickness*/)*windowWidth + x] = col;
		pixel(x, y, c);
	}
}

static void vtLine(int x, int y1, int y2, ColorRGBA c)
{
	if (y2 < y1) { y1 += y2; y2 = y1 - y2; y1 -= y2; } //swap y1 and y2
	if (y2 < 0 || y1 >= windowHeight || x < 0 || x >= windowWidth) return;
	if (y1 < 0) y1 = 0; //clip
	if (y2 >= windowWidth) y2 = windowHeight - 1; //clip

	//int col = c.r << 16 | c.g << 8 | c.b | 0xff000000;
	for (int y = y1; y <= y2; y++)
	{
		//pixelBuffer[y*windowWidth + x] = col;
		pixel(x, y, c);
	}
}

static void drawFilledRect(int32 startX, int32 startY, ColorRGBA col, int32 width, int32 height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//pixel(startX + x, startY + y, col);
			plotBlendedPixel(startX + x, startY + y, col);
		}

	}
}

//draws a line with Breshenam's algorithm
inline void line(int32 x1, int32 y1, int32 x2, int32 y2/*, ColorRGBA col*/)
{
	if (lineWidth <= 0)
		return;

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

	//Bresenham's algorithm starts by plotting a pixel at the first coordinate of the line
	//(x1, y1), and to x+1, it takes the difference of the y component of the line to the
	//two possible y coordinates, and uses the y coordinate where the error is the smaller,
	//and repeats this for every pixel.
	bool32 drawRect = false;
	if (lineWidth > 1)
	{
		drawRect = true;
	}
	real32 error = 0.0;

	//line slope
	real32 slope = (real32)abs(y2 - y1) / (x2 - x1);

	//starting point
	int32 y = y1;

	int32 ystep = (y2 > y1 ? 1 : -1);

	if (lineWidth > 1)
	{
		drawRect = true;
		//ystep = (y2 > y1 ? lineWidth : -lineWidth);
	}
	//int32 xStep = lineWidth;
	for (int32 i = x1; i < x2; i += 1)
	{
		if (step)
		{
			if (drawRect)
				drawFilledRect(y, i, strokeColor, lineWidth, lineWidth);
			else
				pixel(y, i, strokeColor);

		}
		else
		{
			if (drawRect)
			{
				drawFilledRect(i, y, strokeColor, lineWidth, lineWidth);
			}
			else
				pixel(i, y, strokeColor);
		}
		error += slope;

		if (error >= 0.5)
		{
			y += ystep;
			error -= 1.0;
		//	i += 1;
		}
		//else
		//	i += 1;
			
	}
}
		
inline void rect(int32 x, int32 y, int32 width, int32 height)
{
	//to prevent that the lineWidth is bigger than the square
	if (lineWidth >= 1 && lineWidth < (width - lineWidth) && lineWidth < (height - lineWidth))
	{
		//draw non filled rect
		line(x, y, x + width, y);						//top
		line(x, y, x, y + height);			//left
		line(x, y + height, x + width, y + height); //bottom
		line(x + width, y, x + width, y + height);		//right
		//change the values so we only fill inside the rect
		x += lineWidth;
		y += lineWidth;
		width -= lineWidth;
		height -= lineWidth;
	}

	if (fillFlag)
	{
		drawFilledRect(x, y, fillColor, width, height);
	}
}

//Bresenham circle algorithm
inline void circle2(int32 x, int32 y, int32 radius)
{
	if (fillFlag)
	{
		//NOTE: fix-> extremly slow with windows GDI, why?
		int32 r2 = radius * radius;
		int32 area = r2 << 2;
		int32 rr = radius << 1;
		if (lineWidth >= 1)
		{
			for (int32 i = 0; i < area; i++)
			{
				int32 tx = (i % rr) - radius;
				int32 ty = (i / rr) - radius;

				if (tx * tx + ty * ty <= r2)
					pixel(x + tx, y + ty);
			}
			radius -= lineWidth;
			r2 = radius * radius;
			area = r2 << 2;
			rr = radius << 1;
			//x++;
			//y++;
			for (int32 i = 0; i < area; i++)
			{
				int32 tx = (i % rr) - radius;
				int32 ty = (i / rr) - radius;

				if (tx * tx + ty * ty <= r2)
					pixel(x + tx, y + ty, fillColor);
			}
		}
		else
		{
			for (int32 i = 0; i < area; i++)
			{
				int32 tx = (i % rr) - radius;
				int32 ty = (i / rr) - radius;

				if (tx * tx + ty * ty <= r2)
					pixel(x + tx, y + ty, fillColor);
			}
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

static void fillCircleData(int xc, int yc, int p, int pb, int pd, int radius, ColorRGBA col)
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
		if (b != pb) hzLine(b, a, c, col);
		if (d != pd) hzLine(d, a, c, col);
		if (f != b)  hzLine(f, e, g, col);
		if (h != d && h != f) hzLine(h, e, g, col);
		pb = b;
		pd = d;
		if (p < 0) p += (x++ << 2) + 6;
		else p += ((x++ - y--) << 2) + 10;
	}
}

inline void circle(int xc, int yc, int radius)
{
	//radius += lineWidth;
	if (xc + radius < 0 || xc - radius >= windowWidth || yc + radius < 0 || yc - radius >= windowHeight) return;

	if (fillFlag)
	{
		
		int p = 3 - (radius << 1);
		int pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times  (ensure initial value is outside the range)
		if (lineWidth >= 1)
		{
			//fillCircleData(xc, yc, p, pb, pd, radius, strokeColor);

			radius -= lineWidth;
			
			int p = 3 - (radius << 1);
			int pb = yc + radius + 1, pd = yc + radius + 1; 
			fillCircleData(xc, yc, p, pb, pd, radius, fillColor);
		}
		else
		{
			fillCircleData(xc, yc, p, pb, pd, radius, fillColor);
		}
	}
	else
	{
		int32 x1 = radius;
		int32 y1 = 0;
		int32 err = 0;

		while (x1 >= y1)
		{
			pixel(xc + x1, yc + y1);
			pixel(xc + y1, yc + x1);
			pixel(xc - y1, yc + x1);
			pixel(xc - x1, yc + y1);
			pixel(xc - x1, yc - y1);
			pixel(xc - y1, yc - x1);
			pixel(xc + y1, yc - x1);
			pixel(xc + x1, yc - y1);

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

void draw2dPolygon(vec2 *inVertices, int32 numVertices)
{

	for (int i = 0; i < numVertices; i++)
	{
	
		line((int32)(inVertices[i].x + center.x),						
			(int32)(-inVertices[i].y + center.y),						
			(int32)(inVertices[(i + 1) % numVertices].x + center.x),		
			(int32)(-inVertices[(i + 1) % numVertices].y + center.y));	
	}
}


void translate2d(vec2 *inVertices, vec2 *outVertices, int32 numVertices, real32 x, real32 y)
{
	for (int32 i = 0; i < numVertices; i++)
	{
		outVertices[i].x = inVertices[i].x + x;
		outVertices[i].y = inVertices[i].y + y;
	}
}

void scale2d(vec2 *inVertices, vec2 *outVertices, int32 Total, real32 scale)
{
	for (int32 i = 0; i < Total; i++)
	{
		outVertices[i].x = inVertices[i].x * scale;
		outVertices[i].y = inVertices[i].y * scale;
	}
}

void rotateZ2d(vec2 *inVertices, vec2 *outVertices, int32 numVertices, real32 angle)
{
	for (int32 i = 0; i < numVertices; i++)
	{
		outVertices[i].x = inVertices[i].x * cos(angle) - inVertices[i].y * sin(angle);
		outVertices[i].y = inVertices[i].y * cos(angle) + inVertices[i].x * sin(angle);
	}
}

void triangle(real32 x1, real32 y1, real32 x2, real32 y2, real32 x3, real32 y3)
{
	vec2 triangleVertices[3] =
	{
		{ x1, y1 },		// top right
		{ x2, y2 },		// top left
		{ x3, y3 },		// bottom left
	};

	vec2 SVertices[3];
	vec2 SRVertices[3];	//Scaled and Rotated 
	vec2 SRTPoints[4];	//Scaled, Rotated, Translated

	//prevents scaling with negative number
	//if (scale < 0) scale = 0;				

	//transformations
	scale2d(triangleVertices, SVertices, 3, 1);
	rotateZ2d(SVertices, SRVertices, 3, 90);
	translate2d(SRVertices, SRTPoints, 3, 100, 100);

	draw2dPolygon(SRTPoints, 3);
}

#endif

// TODO: linked list
/* ----- Data Structures ----- */
struct Entity;
#include <vector>
using namespace std;
struct VecArray
{
	vector<Entity*> entity;
	
	int32 length = 0;
	void push(Entity *ent)
	{
		//data.push_back(ent);
		entity.emplace_back(ent);
		length = entity.size();
	}

	void splice(int32 start, int32 end)
	{
		//if (!data.empty())
		//{
			/*for (int32 i = start; i <= end; i++)
			{
				delete data[i];
			}*/
			delete entity[start];
			entity.erase(entity.begin() + start, entity.begin() + start + end);
			length = entity.size();
		//}
	}
	void reserve(int32 val)
	{
		entity.reserve(val);
	}
	void free()
	{
		for (auto &a : entity)
		{
			delete a;
		}
		//data.shrink_to_fit();
	}
};

//dynamic array
struct Array
{
	int32 length;
	int32 size;
	void **data;

	void reserve(int32 initSize)
	{
		length = 0;
		size = initSize;
		data = (void**)malloc(sizeof(void *) * size);
	}

	void resize(int32 newSize)
	{

#if defined(_DEBUG) || defined(FRAMEWORK_INTERNAL)
		printf("Realloc the Array: %d to %d\n", size, newSize);
#endif
		void **newArray = (void**)realloc(data, sizeof(void *) * newSize);
		if (newArray)
		{
			data = newArray;
			size = newSize;
		}
	}

	void push(void *value)
	{
		if (size == length)
			resize(size * 2);
		data[length++] = value;
	}

	void splice(int32 index)
	{
		if (index < 0 || index >= length)
			return;

		free(data[index]);
		data[index] = 0;

		for (int i = index; i < length - 1; i++)
		{
			data[i] = data[i + 1];
			data[i + 1] = 0;
		}

		length--;

		if (length > 0 && length == size / 4)
			resize(size / 2);
	}

	void set(int index, void *value)
	{
		if (index >= 0 && index < length)
			data[index] = value;
	}

	void *get(int index)
	{
		if (index >= 0 && index < length)
			return data[index];
		return 0;
	}

	void freeMem()
	{
		for (int32 i = 0; i < length; i++) {
			free(data[i]);
		}
		free(data);
	}

};
