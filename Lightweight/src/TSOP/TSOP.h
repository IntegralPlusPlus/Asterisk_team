#pragma once
#include "project_config.h"
#include "Pin.h"

#define RAD2DEG	57.2957795130823208767
#define DEG2RAD	0.01745329251994329576
#define TSOP1GRAD 11.25

class TSOP {
	public:
		TSOP(Pin& mux1input, Pin& mux2input, Pin& w1, Pin& w2, Pin& w3, Pin& w4);
		void calculateAngle();
		bool updateTSOPs();
		int16_t getAngle();
	private:
		Pin _w1, _w2, _w3, _w4, _mux1input, _mux2input;
		bool tsopValues[32];
		float _angle;
};