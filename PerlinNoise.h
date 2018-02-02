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
	
public:
	enum FADE_TYPE{
		FADE_TYPE__DEG_9,
		FADE_TYPE__DEG_7,
		FADE_TYPE__DEG_5,
		FADE_TYPE__DEG_3,
		FADE_TYPE__DEG_1,
		FADE_TYPE__DEG_COS,
	};
	
private:
	FADE_TYPE FadeType;
	FADE_TYPE Sharpen_Output;
	
	/****************************************
	****************************************/
private:
	double lerp(double t, double a, double b);
	double fade(double t, FADE_TYPE FadeType);
	
	double grad(int hash, double x, double y, double z);
	double grad(int hash, double x, double y);
	double grad(int hash, double x);
	
public:
	/****************************************
	****************************************/
	PERLIN_NOISE();
	PERLIN_NOISE(unsigned int seed);
	
	double test_fadeFunction(double t);
	
	void set_FadeType(FADE_TYPE _type);
	PERLIN_NOISE::FADE_TYPE get_FadeType();
	void set_SharpenType(FADE_TYPE _type);
	PERLIN_NOISE::FADE_TYPE get_SharpenType();
	
	double noise(double x, double y, double z);
	double noise(double x, double y);
	double noise(double x);

	double OctaveNoise(double x, double y, double z, int octaves, double persistence);
	double OctaveNoise(double x, double y, int octaves, double persistence);
	double OctaveNoise(double x, int octaves, double persistence);
};
