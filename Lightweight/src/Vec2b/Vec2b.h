#pragma once
#ifndef VEC2B_H_
#define VEC2B_H_

#include "project_config.h"

#define DELTA_SPEED 0.003
#define RAD2DEG	57.2957795130823208767
#define DEG2RAD	0.01745329251994329576

struct Vec2b {
	public:
		Vec2b();
		Vec2b(double len, double ang);
		void set(double len, double ang);
		void changeTo(Vec2b to);
		double length;
		double angle;
};

#endif /* VEC2B_H_ */