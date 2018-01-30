/************************************************************
// not openframeworks.
#include "PerlinNoise.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
************************************************************/

#include "PerlinNoise.h"

#include <random>
/*
#include <cmath>
#include <algorithm>
#include <numeric>
*/

/************************************************************
************************************************************/

/******************************
******************************/
PERLIN_NOISE::PERLIN_NOISE() {
	// Initialize the permutation vector with the reference values
	p = {
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
		8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
		35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
		134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
		250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
		189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 
		43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
		97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
		107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
		
	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

/******************************
******************************/
PERLIN_NOISE::PERLIN_NOISE(unsigned int seed) {
	p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Suffle  using the above random engine
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}


/******************************
******************************/
double PERLIN_NOISE::OctaveNoise(double x, double y, double z, int octaves, double persistence) {
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
	
	for(int i = 0; i < octaves; i++) {
		total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
 
		maxValue += amplitude;
 
		amplitude *= persistence;
		frequency *= 2;
	}
 
	return total/maxValue;
}

/******************************
******************************/
double PERLIN_NOISE::OctaveNoise(double x, int octaves, double persistence) {
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
	
	for(int i = 0; i < octaves; i++) {
		total += noise(x * frequency) * amplitude;
 
		maxValue += amplitude;
 
		amplitude *= persistence;
		frequency *= 2;
	}
 
	return total/maxValue;
}

/******************************
******************************/
double PERLIN_NOISE::noise(double x, double y, double z) {
	// Find the unit cube that contains the point
	int X = (int) floor(x) & 255; // <= 255
	int Y = (int) floor(y) & 255;
	int Z = (int) floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;		// <= 510(255 + 255), X <= 255
	int AA = p[A] + Z;		// <= 510(255 + 255), A <= 510
	int AB = p[A + 1] + Z;	// <= 510(255 + 255), A + 1 <= 511
	int B = p[X + 1] + Y;	// <= 510(255 + 255), X + 1 <= 256
	int BA = p[B] + Z;		// <= 510(255 + 255), B <= 510
	int BB = p[B + 1] + Z;	// <= 510(255 + 255), B + 1 <= 511
	
	// Add blended results from 8 corners of cube
	double res = lerp(	w,
						lerp(	v,
								lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)),
								lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))
								),	
						lerp(	v,
								lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)),
								lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))
								)
						);
	
	
	return (res + 1.0)/2.0; // For convenience we bind the result to 0 - 1 (theoretical min/max before is [-1, 1])
}

/******************************
******************************/
double PERLIN_NOISE::noise(double x, double y) {
	// Find the unit cube that contains the point
	int X = (int) floor(x) & 255; // <= 255
	int Y = (int) floor(y) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);

	// Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;		// <= 510(255 + 255), X <= 255
	int AA = p[A];			// <= 255, A <= 510
	int AB = p[A + 1];		// <= 255, A + 1 <= 511
	int B = p[X + 1] + Y;	// <= 510(255 + 255), X + 1 <= 256
	int BA = p[B];			// <= 255, B <= 510
	int BB = p[B + 1];		// <= 255, B + 1 <= 511
	
	// Add blended results from 8 corners of cube
	double res = lerp(	v,
						lerp(u, grad(p[AA], x, y), grad(p[BA], x-1, y)),
						lerp(u, grad(p[AB], x, y-1), grad(p[BB], x-1, y-1))
						);
	
	return (res + 1.0)/2.0; // For convenience we bind the result to 0 - 1 (theoretical min/max before is [-1, 1])
}

/******************************
******************************/
double PERLIN_NOISE::noise(double x) {
	// Find the unit cube that contains the point
	int X = (int) floor(x) & 255; // <= 255

	// Find relative x, y,z of point in cube
	x -= floor(x);

	// Compute fade curves for each of x, y, z
	double u = fade(x);

	// Hash coordinates of the 8 cube corners
	int A = p[X];			// <= 255, X <= 255
	int AA = p[A];			// <= 255, A <= 255
	int B = p[X + 1];		// <= 255, X + 1 <= 256
	int BA = p[B];			// <= 255, B <= 255
	
	// Add blended results from 8 corners of cube
	double res = lerp(u, grad(p[AA], x), grad(p[BA], x-1));
	
	return (res + 1.0)/2.0; // For convenience we bind the result to 0 - 1 (theoretical min/max before is [-1, 1])
}

/******************************
******************************/
double PERLIN_NOISE::fade(double t) { 
	return t * t * t * (t * (t * 6 - 15) + 10);
}

/******************************
******************************/
double PERLIN_NOISE::lerp(double t, double a, double b) { 
	// return a + t * (b - a); 
	return (1 - t) * a + t * b;
}

/******************************
description
	hashから勾配ベクトルを決定し、距離ベクトルとの内積を算出。
******************************/
double PERLIN_NOISE::grad(int hash, double x, double y, double z) {
	switch(hash & 0xF){
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x + z;
		case 0x5: return -x + z;
		case 0x6: return  x - z;
		case 0x7: return -x - z;
		case 0x8: return  y + z;
		case 0x9: return -y + z;
		case 0xA: return  y - z;
		case 0xB: return -y - z;
		case 0xC: return  y + x;
		case 0xD: return -y + z;
		case 0xE: return  y - x;
		case 0xF: return -y - z;
		default: return 0; // never happens
	}
}

/******************************
description
	hashから勾配ベクトルを決定し、距離ベクトルとの内積を算出。
******************************/
double PERLIN_NOISE::grad(int hash, double x, double y) {
	switch(hash & 0xF){
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
			return x + y;
		
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
			return x - y;
			
		case 0x8:
		case 0x9:
		case 0xA:
		case 0xB:
			return -x + y;
			
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
			return -x - y;
			
		default: return 0; // never happens
	}
}

/******************************
description
	hashから勾配ベクトルを決定し、距離ベクトルとの内積を算出。
******************************/
double PERLIN_NOISE::grad(int hash, double x) {
	switch(hash & 0xF){
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
			return 2 * x;
			
		case 0x8:
		case 0x9:
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
			return -2 * x;
			
		default: return 0; // never happens
	}
}
