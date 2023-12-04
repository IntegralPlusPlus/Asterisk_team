#pragma once
#include "project_config.h"

#define DELTA_SPEED 20.f
#define RAD2DEG	57.2957795130823208767f
#define DEG2RAD	0.01745329251994329576f

struct Vec2b {
	public:
		void update(Vec2b to);
		float length;
		float	angle;
};

void Vec2b::update(Vec2b to) {
	Vec2b result;
	float x1 = this->length * cos(this->angle * DEG2RAD);
	float y1 = this->length * sin(this->angle * DEG2RAD);
	float x2 = to.length * cos(to.angle * DEG2RAD);
	float y2 = to.length * sin(to.angle * DEG2RAD);
	float xDiff = x2 - x1, yDiff = y2 - y1, len = sqrt(xDiff * xDiff + yDiff * yDiff);
	float lyambda;
	
	if (len - DELTA_SPEED > 0) lyambda = DELTA_SPEED / (len - DELTA_SPEED);
	else lyambda = 1;
	
	float dx = (x1 + lyambda * x2) / (1 + lyambda), dy = (y1 + lyambda * y2) / (1 + lyambda);
	float resX = x1 + dx, resY = y1 + dy;
	this->length = sqrt(resX * resX + resY * resY);
	this->angle = atan2(resY, resX) * RAD2DEG;
	if (this->angle < 0) this->angle += 360;
}