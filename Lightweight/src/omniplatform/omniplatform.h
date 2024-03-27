#pragma once
#include "project_config.h"
#include "Motor.h"
#include "math.h"

#define RAD2DEG	57.2957795130823208767
#define DEG2RAD	0.01745329251994329576

class omniplatform {
	public:
		omniplatform(Motor &m1, Motor &m2, Motor &m3, Motor &m4);
		void disable();
		void move(float maxSpeed, float nowSpeed, float angle, float inc, float maxInc);
		float adduct0_360(float angle);
	private:
		Motor _m1, _m2, _m3, _m4;
};