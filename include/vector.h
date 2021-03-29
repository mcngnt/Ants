#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "utils.h"

#pragma once

typedef struct Vector
{
	float x, y;
} Vector;

Vector vec_v(float, float);
Vector print_v(Vector);
Vector operator+(const Vector&, const Vector&);
Vector invert_v(Vector);
Vector operator-(const Vector&, const Vector&);
Vector operator+(const Vector&, float);
Vector operator-(const Vector&, float);
Vector operator*(const Vector&, const Vector&);
float get_norm_v(Vector);
float dist_v(Vector, Vector);
Vector operator/(const Vector&, float);
Vector operator/(const Vector&, const Vector&);
bool operator==(const Vector&, const Vector&);
Vector abs_v(Vector);
float dot_v(Vector, Vector);
float det_v(Vector, Vector);
Vector normalize_v(Vector);
bool vec_in_rect(Vector, Vector, Vector);
Vector operator*(const Vector&, float);
Vector max_v(Vector, float);
Vector min_v(Vector, float);
float get_angle_v(Vector);
Vector vec_from_angle_v(float);
Vector random_vec_v(int, int, int, int);
sf::Vector2f vec_to_sfvec2_v(Vector);
Vector sfvec2_to_vec_v(sf::Vector2f);
float get_angle_two_vec_v(Vector, Vector);
float angle_btw_pos_v(Vector, Vector);
Vector floor_v(Vector);
Vector clamp_mag_v(Vector, float);

Vector mult_comp_c(Vector, Vector);
Vector conjugate_c(Vector);
Vector divide_comp_c(Vector, Vector);
Vector power_comp_c(Vector, int);

#endif