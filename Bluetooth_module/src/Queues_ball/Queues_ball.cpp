#include "Queues_ball.h"

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
	
	if (_last > QUEUE_SIZE - 1) _last = 0;
}

void BallVec2b::calculate() {
	if (_s2Len / double(QUEUE_SIZE) - pow((_sLen / double(QUEUE_SIZE)), 2) == 0) {
		_length = _queue[_last].getLen();
	} else {
		double bLen = (_sLenT / double(QUEUE_SIZE) - _sLen * _timeSumm / pow(double(QUEUE_SIZE), 2)) / (_s2Len / double(QUEUE_SIZE) - pow((_sLen / double(QUEUE_SIZE)), 2));
		double aLen = _timeSumm / double(QUEUE_SIZE) - bLen * _sLen / double(QUEUE_SIZE);

		if (bLen != 0) {
			uint32_t futureTime = _queue[_last].getTime() + MS_DELTA;
			_length = (futureTime - aLen) / bLen;
		}
	}
	
	if (_s2Ang / double(QUEUE_SIZE) - pow((_sAng / double(QUEUE_SIZE)), 2) == 0) {
		_angle = _queue[_last].getAngle();
	} else {
		double bAng = (_sAngT / double(QUEUE_SIZE) - _sAng * _timeSumm / pow(double(QUEUE_SIZE), 2)) / (_s2Ang / double(QUEUE_SIZE) - pow((_sAng / double(QUEUE_SIZE)), 2));
		double aAng = _timeSumm / double(QUEUE_SIZE) - bAng * _sAng / double(QUEUE_SIZE);
		
		if (bAng != 0) {
			uint32_t futureTime = _queue[_last].getTime() + MS_DELTA;
			_angle = (futureTime - aAng) / bAng;
		}
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
	
	if (_queue[elder].correct()) {
		if (_queue[current].getAngle() - _queue[elder].getAngle() > 180) _transition360--;
		else if (_queue[current].getAngle() - _queue[elder].getAngle() < -180) _transition360++;
	}
}

double BallVec2b::adduct(double a) {
	while (a < -180) a += 360;
	while (a > 180) a -= 360;
	
	return a;
}

Vec2b BallVec2b::getCurrentVec2b() {
	return Vec2b(_length, _angle);
}