//TODO: this is for the gaussian random numbers, fix this with my own gaussian calculations
#include <random>

#define PI 3.14159265358979323846f
#define TWO_PI 6.28318530717958647693f
#define HALF_PI PI/2
#define SWAP(x, y)\
		x = x + y; \
		y = x - y; \
		x = x - y;

struct Rect
{
	real32 x;
	real32 y;
	real32 w;
	real32 h;
};

struct vec2
{
	real32 x;
	real32 y;
	vec2() { };
	vec2(real32 X, real32 Y) : x(X), y(Y) {}

	//overloaded operator for vector addition: v3 = v1 + v2;
	vec2 operator+(vec2& v2)
	{
		return vec2{x + v2.x, y + v2.y};
	}

	//v1 += v2;
	friend vec2& operator+=(vec2& v1, vec2& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	//subtracting two vectors
	vec2 operator-(vec2& v2)
	{
		return vec2{x - v2.x, y - v2.y};
	}

	friend vec2& operator-=(vec2& v1, vec2& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	//multiply by a scalar number
	vec2 operator*(real32 scalar)
	{
		return vec2{x * scalar, y * scalar};
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
		return vec2{x / scalar, y / scalar};
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

	//calculate the angle, called heading() in Processing and p5.js
	real32 angle() { return atan2(y, x); }
	
	//real32 heading() { return atan2(y, x); }
	
	//that hadamard product - element-wise product of two vectors wich return a new vector
	vec2 hadamard(vec2 a, vec2 b)
	{
		vec2 result = { a.x*b.x, a.y*b.y };
		return result;
	}

	//calculate the dot(inner) product
	real32 dotProduct(vec2 v2) { return x*v2.x + y*v2.y; }

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
	//or divide each component by its magnitude
	void normalize()
	{
		real32 l = length();
		if (l > 0) //avoid divide by 0
		{
			//(*this) *= 1 / l;
			(*this) /= l;
		}
	}
};

inline real32 square(real32 a)
{
	real32 result = a*a;
	return result;
}

//dot(inner) product
//dot product of a * b = lenght of a * length of b * cos(angle between them)
//The dot product consists of multiplying each element of the A vector with its counterpart from vector B and taking the sum of each product.
inline real32 inner(vec2 a, vec2 b)
{
	real32 result = a.x*b.x + a.y*b.y;
	return result;
}

inline real32 lengthSq(vec2 A)
{
	real32 result = inner(A, A);
	return result;
}

inline real32 squareRoot(real32 value)
{
	real32 result = sqrtf(value);
	return result;
}

struct vec3
{
	real32 x;
	real32 y;
	real32 z;

	//vec3() { };
	//vec3(real32 X, real32 Y, real32 Z) : x(X), y(Y), z(Z) {}
	//overloaded operator for vector addition: v3 = v1 + v2;
	vec3 operator+(const vec3& v2) const
	{
		return vec3{ x + v2.x, y + v2.y, z + v2.z };
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
		return vec3{ x - v2.x, y - v2.y, z - v2.z };
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
		return vec3{ x * scalar, y * scalar, z * scalar };
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
		return vec3{ x / scalar, y / scalar, z / scalar };
	}

	vec3& operator/=(real32 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	//calculate length (magnitude) of the vector, square root of x^2 + y^2 + z^2
	real32 length() { return sqrt(x * x + y * y + z * z); }

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

//calculate the cross product of two vectors
inline vec3 vectorCrossProduct(vec3 v1, vec3 v2)
{
	vec3 vResult;
	vResult.x = v1.y * v2.z - v2.y * v1.z;
	vResult.y = -v1.x * v2.z + v2.x * v1.z;
	vResult.z = v1.x * v2.y - v2.x * v1.y;
	return vResult;
}

//get the dot(inner) product between two vectors
inline real32 vectorDotProduct(vec2 v1, vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

inline real32 vectorDotProduct(vec3 v1, vec3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//calculate the unit normal from 3 point on a plane in CCW-order
vec3 getNormalVector(vec3 vP1, vec3 vP2, vec3 vP3)
{
	vec3 vV1, vV2;
	vec3 vNormal;
	vV1 = vP2 - vP1;
	vV2 = vP3 - vP1;

	vNormal = vectorCrossProduct(vV1, vV2);
	vNormal.normalize();
	return vNormal;
}
//gets the three coefficients of a plane equation given three points on the plane
void getPlaneEquation(vec3 vPoint1, vec3 vPoint2, vec3 vPoint3, real32 vPlane[4])
{
	// Get normal vector from three points. The normal vector is the first three coefficients
	// to the plane equation...
	vec3 vP = getNormalVector(vPoint1, vPoint2, vPoint3);
	vPlane[0] = vP.x;
	vPlane[1] = vP.y;
	vPlane[2] = vP.z;
	// Final coefficient found by back substitution
	vPlane[3] = -(vPlane[0] * vPoint3.x + vPlane[1] * vPoint3.y + vPlane[2] * vPoint3.z);
}

/* matrix math */
typedef real32 Matrix[16];      //column major 4x4 matrix

struct mat4x4
{
	real32 m[4][4] = { 0 };
};

struct vec4
{
	real32 x;
	real32 y;
	real32 z;
	real32 w;
};

//rotates a vector using a 4x4 matrix, translation column is ignored
void rotateVector(vec3 vSrc, Matrix mMatrix, vec3 *vOut)
{
	vOut->x = mMatrix[0] * vSrc.x + mMatrix[4] * vSrc.y + mMatrix[8] * vSrc.z;
	vOut->y = mMatrix[1] * vSrc.x + mMatrix[5] * vSrc.y + mMatrix[9] * vSrc.z;
	vOut->z = mMatrix[2] * vSrc.x + mMatrix[6] * vSrc.y + mMatrix[10] * vSrc.z;
}

//load a matrix with the idenity matrix
void loadIdentityMatrix(Matrix m)
{
	static Matrix identity = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	memcpy(m, identity, sizeof(Matrix));
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(identity);
}

//creates a 4x4 rotation matrix, takes radians
void createRotationMatrix(real32 angle, vec3 pos, Matrix mMatrix)
{
	real32 sinSave, cosSave, oneMinusCos;
	real32 xx, yy, zz, xy, yz, zx, xs, ys, zs;

	if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
	{
		loadIdentityMatrix(mMatrix);
		return;
	}

	//normalize rotation matrix
	pos.normalize();

	sinSave = (real32)sin(angle);
	cosSave = (real32)cos(angle);
	oneMinusCos = 1.0f - cosSave;

	xx = pos.x * pos.x;
	yy = pos.y * pos.y;
	zz = pos.z * pos.z;
	xy = pos.x * pos.y;
	yz = pos.y * pos.z;
	zx = pos.z * pos.x;
	xs = pos.x * sinSave;
	ys = pos.y * sinSave;
	zs = pos.z * sinSave;

	mMatrix[0] = (oneMinusCos * xx) + cosSave;
	mMatrix[4] = (oneMinusCos * xy) - zs;
	mMatrix[8] = (oneMinusCos * zx) + ys;
	mMatrix[12] = 0.0f;

	mMatrix[1] = (oneMinusCos * xy) + zs;
	mMatrix[5] = (oneMinusCos * yy) + cosSave;
	mMatrix[9] = (oneMinusCos * yz) - xs;
	mMatrix[13] = 0.0f;

	mMatrix[2] = (oneMinusCos * zx) - ys;
	mMatrix[6] = (oneMinusCos * yz) + xs;
	mMatrix[10] = (oneMinusCos * zz) + cosSave;
	mMatrix[14] = 0.0f;

	mMatrix[3] = 0.0f;
	mMatrix[7] = 0.0f;
	mMatrix[11] = 0.0f;
	mMatrix[15] = 1.0f;
}

//creates a shadow matrix out of the plane equation coefficients and the position of the light
void createShadowMatrix(vec3 vPoints[3], real32 vLightPos[4], Matrix destMat)
{
	real32 vPlaneEquation[4];
	real32 dot;
	//Matrix destMat;

	getPlaneEquation(vPoints[0], vPoints[1], vPoints[2], vPlaneEquation);

	//dot product of plane and light position
	dot = vPlaneEquation[0] * vLightPos[0] +
		vPlaneEquation[1] * vLightPos[1] +
		vPlaneEquation[2] * vLightPos[2] +
		vPlaneEquation[3] * vLightPos[3];

	//calculate shadow projection
	//1st column
	destMat[0] = dot - vLightPos[0] * vPlaneEquation[0];
	destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
	destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
	destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];

	//2nd column
	destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
	destMat[5] = dot - vLightPos[1] * vPlaneEquation[1];
	destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
	destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];

	//3rd Column
	destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
	destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
	destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
	destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];

	//4th Column
	destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
	destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
	destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
	destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
	//return &destMat;
}


//pseudo-random uniform distribution of numbers
//set the random seed to constant value to return the same pseudo random numbers every time
inline void randomSeed(uint32 value)
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
	//return (((real32)rand() / (RAND_MAX)) * 2 - 1.0f);
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
	vec.setAngle(randomf() * TWO_PI);
	return vec;
}

//returns a Gaussian(normal) distribution of random numbers around the mean with a specific standard deviation.
//the probability of getting values far from the mean is low, the probability of getting numbers near the mean is high.
inline real32 randomGaussian()
{
	std::default_random_engine randomGenerator;
	//gaussian random numbers with a mean of 0 and standard deviation of 1.0
	std::normal_distribution<real32> gaussianDistribution(0.0f, 1.0f);
	return gaussianDistribution(randomGenerator);
}

inline real32 randomGaussian(real32 mean)
{
	std::default_random_engine randomGenerator;
	std::normal_distribution<real32> gaussianDistribution(mean, 1.0f);
	return gaussianDistribution(randomGenerator);
}

inline real32 randomGaussian(real32 mean, real32 sd)
{	
	std::default_random_engine randomGenerator;
	std::normal_distribution<real32> gaussianDistribution(mean, sd);
	return gaussianDistribution(randomGenerator);
}

//1D Perlin noise, returns noise value at specified coordinate, the value is always between 0 and 1.
inline real32 noise(real32 x)
{
	return _slang_library_noise1(x)*0.5f + 0.5f;
}

//2D Perlin noise
inline real32 noise(real32 x, real32 y)
{
	return _slang_library_noise2(x, y)*0.5f + 0.5f;
}

//3D Perlin noise
inline real32 noise(real32 x, real32 y, real32 z)
{
	return _slang_library_noise3(x, y, z)*0.5f + 0.5f;
}

//the likelihood that a random value will be picked is equal to the the first random number (r1)
//returns a random value between 0 and 1
inline real32 montecarlo()
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

#define getMin(a, b) ((a) < (b) ? (a) : (b))
#define getMax(a, b) ((a) > (b) ? (a) : (b))

/*
int32 getMin(int32 a, int32 b)
{
	int32 result;
	result = a < b ? a : b;
	return result;
}

int32 getMax(int32 a, int32 b)
{
	int32 result;
	result = a > b ? a : b;
	return result;
}
*/

//convert radians to degrees
inline real32 degrees(real32 radians)
{
	real32 degrees = (radians / TWO_PI) * 360;
	return degrees;
}

//convert degrees to radians
inline real32 radians(real32 degrees)
{
	real32 radians = (degrees / 360) * TWO_PI;
	return radians;
}

//calculates the distance between 2 points
inline real32 dist(real32 x1, real32 y1, real32 x2, real32 y2)
{
	//return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//returns a interpolated value between two numbers, the amount parameter is the amount to interpolate between the two values. 
inline real32 lerp(real32 start, real32 stop, real32 amount)
{
	return amount*(stop - start) + start;
};

inline int32 constrain(int32 value, int32 min, int32 max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

inline real32 constrainf(real32 value, real32 min, real32 max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

inline real32 mapVal(real32 value, real32 inMin, real32 inMax, real32 outMin, real32 outMax)
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
			if (outVal < outMax) 
				outVal = outMax;
			else if (outVal > outMin) 
				outVal = outMin;
		}
		else
		{
			if (outVal > outMax) 
				outVal = outMax;
			else if (outVal < outMin) 
				outVal = outMin;
		}

		return outVal;
	}
}