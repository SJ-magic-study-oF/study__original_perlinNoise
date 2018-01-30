/************************************************************
************************************************************/
#pragma once

/************************************************************
// not openframeworks.
#include <vector>
************************************************************/
#include "ofMain.h"
// #include <vector>

#include "sj_common.h"

/************************************************************
************************************************************/
class PERLIN_NOISE{
private:
	/****************************************
	****************************************/
	vector<int> p;
	
	/****************************************
	****************************************/
	double fade(double t);
	double lerp(double t, double a, double b);
	
	double grad(int hash, double x, double y, double z);
	double grad(int hash, double x, double y);
	double grad(int hash, double x);
	
public:
	/****************************************
	****************************************/
	PERLIN_NOISE();
	PERLIN_NOISE(unsigned int seed);
	
	double noise(double x, double y, double z);
	double noise(double x, double y);
	double noise(double x);

	double OctaveNoise(double x, double y, double z, int octaves, double persistence);
	double OctaveNoise(double x, int octaves, double persistence);
};
