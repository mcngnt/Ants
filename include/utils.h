#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>

#define SCREEN_W 1080
#define SCREEN_H 720

#define PI 3.14

float rand_float(bool);
float rad_to_deg(float);
float mod_2pi(float);
float angle_dist(float, float);
float clamp(float, float, float);
float sign(float);
float neg_range(float);

#endif