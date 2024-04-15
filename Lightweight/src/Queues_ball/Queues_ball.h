#pragma once
#include "project_config.h"
#include "Vec2b.h"

#define QUEUE_SIZE 90
#define MS_DELTA 20

#define STATUS_POP_ELEMENT 0
#define STATUS_PUSH_ELEMENT 1

struct Vec2bTime {
	void set(double length, double angle, uint32_t t, int16_t transition = 0) {
		this->vector.length = length;
		this->vector.angle = angle;
		this->time = t;
		this->transition = transition;
	}
	
	void setTransition(int16_t transition) {
		this->transition = transition;
	}
	
	bool correct() {
		return !(this->vector.angle == -1 && this->vector.length == -1 && this->time == 0);
	}
	
	double getLen() { 
		return this->vector.length;
	}
	
	double getAngle() { 
		return this->vector.angle;
	}
	
	int32_t getTime() {
		return this->time;
	}
	
	int16_t getTransition() {
		return this->transition;
	}
	
	Vec2b vector;
	int64_t time;
	int16_t transition;
};

class BallVec2b {
	public:
		BallVec2b();
		void push(Vec2b vec, int64_t millis);
		Vec2b getCurrentVec2b();
		void changeValues(bool status); 
		void calculate();
		void setTransitionBy360();
		double adduct(double a);
		double getDerivativeAng();
		double getDerivativeDist();
	private:
		Vec2bTime _queue[QUEUE_SIZE];
		int16_t _last;
		double _sLen, _s2Len, _sAng, _s2Ang, _sLenT, _sAngT, _timeSumm;
		double _length, _angle;
		double _transition360; 
		double aLen, bLen, aAng, bAng;
		double tgDiffDist, tgDiffAng;
};