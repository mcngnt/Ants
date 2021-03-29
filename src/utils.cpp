#include "utils.h"

float rand_float(bool negativeRange)
{
	float x = (float)rand()/(float)RAND_MAX;
	if (negativeRange)
	{
		x = neg_range(x);
	}
	return x;
}

float rad_to_deg(float rad)
{
	return rad * 180.f/PI;
}

float mod_2pi(float angle)
{
	while(angle > 2.f*PI)
	{
		angle -= 2.f * PI;
	}
	while(angle < 0.f)
	{
		angle += 2.f * PI;
	}
	return angle;
}

float angle_dist(float a, float b)
{
	float r = b - a;
	if(r > PI)
	{
		r -= PI*2 ;
	}
	if(r < -PI)
	{
		r+=PI*2;
	}
	return mod_2pi(r);
}

float clamp(float x, float min, float max)
{
	return fmax(fmin(x, max), min);
}

float sign(float x)
{
	if (x < 0)
	{
		return -1.f;
	}
	return 1.f;
}
float neg_range(float x)
{
	return (x*2.f)-1.f;
}