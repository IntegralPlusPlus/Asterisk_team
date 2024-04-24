#pragma once
#include "project_config.h"
#include "Pin.h"
#include "Adc.h"

#define RAD2DEG	57.2957795130823208767f
#define DEG2RAD	0.01745329251994329576f
#define TSOP1GRAD 11.25f
#define Ec 2.718281828459f

class TSOP {
	public:
		TSOP(Pin& w1, Pin& w2, Pin& w3, Pin& w4, Pin& mux1input, Pin& mux2input);
		TSOP();
		void calculate();
		void updateTSOPs();
		float getAngle();
		float getDist();
		int16_t detourAngle(int16_t angle);
		int16_t adduct(int16_t a);
		double convertDist(double dist);
		double angleOffset(double angle, double dist, double angleIMU = 0);
		bool distBad(int16_t distLocator);
		bool ballFar(float dist);
	protected:
		Pin _w1, _w2, _w3, _w4, _mux1input, _mux2input;
		bool tsopValues[32];
		float _angle;
		float _dist;
};