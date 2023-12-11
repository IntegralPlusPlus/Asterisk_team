#pragma once
#include "project_config.h"
#include "Vec2b.h"

#define QUEUE_SIZE 45 
#define MS_DELTA 10

#define STATUS_POP_ELEMENT 0
#define STATUS_PUSH_ELEMENT 1

struct Vec2bTime {
	void set(Vec2b v, uint32_t t) {
		this->vector.length = v.length;
		this->vector.angle = v.angle;
		this->time = t;
	}
	
	bool correct() {
		return this->vector.angle != -1 && this->vector.length != -1 && this->time != 0;
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
	
	Vec2b vector;
	uint32_t time;
};

class BallVec2b {
	public:
		BallVec2b();
		void push(Vec2b vec, uint32_t millis);
		Vec2b getCurrentVec2b();
		void changeValues(bool status); 
		void calculate();
		void setTransitionBy360();
		double adduct(double a);
	private:
		Vec2bTime _queue[QUEUE_SIZE];
		int8_t _last;
		double _sLen, _s2Len, _sAng, _s2Ang, _sLenT, _sAngT, _timeSumm;
		double _length, _angle;
		double _transition360; 
};