#pragma once
#include "project_config.h"
#include "Pin.h"

#define RAD2DEG	57.2957795130823208767
#define DEG2RAD	0.01745329251994329576
#define TSOP1GRAD 11.25
#define Ec 2.718281828459

class TSOP {
	public:
		TSOP(Pin& mux1input, Pin& mux2input, Pin& w1, Pin& w2, Pin& w3, Pin& w4);
		TSOP();
		void calculateAngle();
		bool updateTSOPs();
		int16_t getAngle();
		int16_t getDist();
		int16_t detourAngle(int16_t angle);
		int16_t adduct(int16_t a);
		double convertDist(double dist);
		double angleOffset(double angle, double dist);
		bool distBad(int16_t distLocator);
	protected:
		Pin _w1, _w2, _w3, _w4, _mux1input, _mux2input;
		bool tsopValues[32];
		float _angle;
		int16_t _dist;
};