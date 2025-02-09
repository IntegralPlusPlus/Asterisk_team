#pragma once
#include "project_config.h"
#include "Pin.h"
#include "Adc.h"

#define RAD2DEG	57.2957795130823208767f
#define DEG2RAD	0.01745329251994329576f
#define TSOP1GRAD 11.25f
#define Ec 2.718281828459f

//Value of TSOP-dist when ball is far from robot
//If _dist < BALL_FAR robot follow to ball
//If _dist > BALL_FAR robot start detown of ball
#define BALL_FAR 5.5

//Maximum value of _dist from robot to ball in this ball-charging characteristics
#define MAX_DIST 7.5

class TSOP {
	public:
		TSOP(Pin& w1, Pin& w2, Pin& w3, Pin& w4, Pin& mux1input, Pin& mux2input);
		TSOP();
		void calculate();
		void updateTSOPs();
		float getAngle();
		float getDist();
		int16_t detownAngle(int16_t angle);
		int16_t adduct(int16_t a);
	
		//Convert dist from [0, MAX_DIST] to [0, 1]
		double convertDist(double dist);
	
		//Main function of robots following to ball
		double angleOffset(double angle, double dist, double angleIMU = 0);
		
		//If dist is uncorrect
		bool distBad(int16_t distLocator);
		
		//Check distance to ball (if its far or close)
		bool ballFar(float dist);
	protected:
		Pin _w1, _w2, _w3, _w4, _mux1input, _mux2input;
		bool tsopValues[32];
		float _angle; //angle to ball
		float _dist; //distance to ball
};