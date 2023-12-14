#include <iostream>
#include <cmath>
#define QUEUE_SIZE 45 
#define MS_DELTA 10

#define STATUS_POP_ELEMENT 0
#define STATUS_PUSH_ELEMENT 1

#define DELTA_SPEED 0.004
#define RAD2DEG	57.2957795130823208767
#define DEG2RAD	0.01745329251994329576
#define uint32_t int
#define int32_t int
#define int8_t int
#define uint8_t int
#define int16_t int

int buff_cycles = 0;

struct Vec2b {
	public:
		Vec2b();
		Vec2b(double len, double ang);
		void set(double len, double ang);
		void changeTo(Vec2b to);
		double length;
		double angle;
};

Vec2b::Vec2b() {
	this->length = 0;
	this->angle = 0;
}

Vec2b::Vec2b(double len, double ang) {
	this->length = len;
	this->angle = ang;
}

void Vec2b::set(double len, double ang) {
	this->length = len;
	this->angle = ang;
}

void Vec2b::changeTo(Vec2b to) {
	double x1 = this->length * cos(this->angle * DEG2RAD);
	double y1 = this->length * sin(this->angle * DEG2RAD);
	double x2 = to.length * cos(to.angle * DEG2RAD);
	double y2 = to.length * sin(to.angle * DEG2RAD);
	double xDiff = x2 - x1, yDiff = y2 - y1;
	double len = sqrt(xDiff * xDiff + yDiff * yDiff);
	double lyambda;
	
	if (len - DELTA_SPEED > 0) lyambda = double(DELTA_SPEED / (len - DELTA_SPEED));
	else lyambda = 1;
	
	double dx = (lyambda * xDiff) / (1 + lyambda), dy = (lyambda * yDiff) / (1 + lyambda);
	double resX = x1 + dx, resY = y1 + dy;
	this->length = sqrt(resX * resX + resY * resY);
	this->angle = atan2(resY, resX) * RAD2DEG;
	if (this->angle < 0) this->angle += 360;
}

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
	
	int16_t getTransition() {
		return this->transition;
	}
	
	Vec2b vector;
	uint32_t time;
	int16_t transition;
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
		int16_t _transition360; 
};

BallVec2b::BallVec2b() {
	_last = 0;
	for (int8_t i = 0; i < QUEUE_SIZE; ++i) _queue[i].set(-1, -1, 0);
	
	_sLen = 0;
	_s2Len = 0;
	_sAng = 0;
	_s2Ang = 0;
	_sLenT = 0;
	_sAngT = 0;
	_timeSumm = 0;
	_length = 0;
	_angle = 0;
	_transition360 = 0;
}

void BallVec2b::push(Vec2b vec, uint32_t millis) {
	if (_queue[_last].correct()) {
		changeValues(STATUS_POP_ELEMENT);
		_timeSumm -= _queue[_last].getTime();
	}	
	
	_queue[_last].set(vec.length, vec.angle, millis);
	setTransitionBy360();
	_queue[_last].setTransition(_transition360);
	changeValues(STATUS_PUSH_ELEMENT);
	calculate();
	_timeSumm += _queue[_last].getTime();
	_last++;
	
	if (_last > QUEUE_SIZE - 1) {
		buff_cycles++;
		_last = 0;
	}
}

void BallVec2b::calculate() {
	if (_s2Len / double(QUEUE_SIZE) - pow((_sLen / double(QUEUE_SIZE)), 2) == 0) {
		_length = _queue[_last].getLen();
	} else {
		double bLen = (_sLenT / double(QUEUE_SIZE) - _sLen * _timeSumm / pow(double(QUEUE_SIZE), 2)) / (_s2Len / double(QUEUE_SIZE) - pow(_sLen / double(QUEUE_SIZE), 2));
		double aLen = _timeSumm / double(QUEUE_SIZE) - bLen * _sLen / double(QUEUE_SIZE);

		uint32_t futureTime = _queue[_last].getTime() + MS_DELTA;
		_length = (futureTime - aLen) / bLen;
	}
	
	if (_s2Ang / double(QUEUE_SIZE) - pow((_sAng / double(QUEUE_SIZE)), 2) == 0) {
		_angle = _queue[_last].getAngle();
	} else {
		double bAng = (_sAngT / double(QUEUE_SIZE) - _sAng * _timeSumm / pow(double(QUEUE_SIZE), 2)) / (_s2Ang / double(QUEUE_SIZE) - pow((_sAng / double(QUEUE_SIZE)), 2));
		double aAng = _timeSumm / double(QUEUE_SIZE) - bAng * _sAng / double(QUEUE_SIZE);
		
		uint32_t futureTime = _queue[_last].getTime() + MS_DELTA;
		_angle = (futureTime - aAng) / bAng;
	}
	
	_angle = adduct(_angle);
}

void BallVec2b::changeValues(bool status) {
	int8_t koeff = 1;
	
	if (status == STATUS_POP_ELEMENT) koeff = -1;
	else if (status == STATUS_PUSH_ELEMENT) koeff = 1;
	
	_sLen += double(koeff) * _queue[_last].getLen();
	_s2Len += double(koeff) * pow(_queue[_last].getLen(), 2);
	_sLenT += double(koeff) * _queue[_last].getLen() * _queue[_last].getTime();
	
	_sAng += double(koeff) * (_queue[_last].getAngle() + 360 * _queue[_last].getTransition());
	_s2Ang += double(koeff) * pow((_queue[_last].getAngle() + 360 * _queue[_last].getTransition()), 2);
	_sAngT += double(koeff) * (_queue[_last].getAngle() + 360 * _queue[_last].getTransition()) * _queue[_last].getTime();
}

void BallVec2b::setTransitionBy360() {
	int8_t current = _last, elder;
	if (current != 0) elder = current - 1;
	else elder = QUEUE_SIZE - 1;	
	
	if (_queue[elder].correct() && _queue[current].correct()) {
		if (_queue[current].getAngle() - _queue[elder].getAngle() > 180) {
			_transition360--;
		} else if (_queue[current].getAngle() - _queue[elder].getAngle() < -180) {
			_transition360++;
		} 
	}
}

double BallVec2b::adduct(double a) {
	while (a < 0) a += 360;
	while (a > 360) a -= 360;
	
	return a;
}

Vec2b BallVec2b::getCurrentVec2b() {
	return Vec2b(_length, _angle);
}

int main() {
	BallVec2b ball;
	for (double i = 0; i < 10000; ++i) {
		double ang;
		if (int(i) % 3 == 0) ang = 0;
		else if (int(i) % 3 == 1) ang = 359;
		else if (int(i) % 3 == 2) ang = 358;
		
		ball.push(Vec2b(5, ang), 3000 + i);
	}
	
	double angle = ball.getCurrentVec2b().angle;
	double len = ball.getCurrentVec2b().length;
	std::cout << "======================= RESULT ====================" << std::endl;
	std::cout << "LEN: " << len << " ANGLE: " << angle;
	
	return 0;
}
