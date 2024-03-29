#include "Processing_coord.h"

ProcessingCoord::ProcessingCoord() {
	_maxLen = 1;
	
	DOWN_Y_GOALKEEPER_RIGHT = 15;
	DOWN_Y_GOALKEEPER_LEFT = 20;
}

void ProcessingCoord::setGoal(uint8_t currentGoal) {
	_goal = currentGoal;
	if (_goal == BLUE_GOAL) {
		DOWN_Y_GOALKEEPER_RIGHT = 20;
		DOWN_Y_GOALKEEPER_LEFT = 26;
	} else {
		DOWN_Y_GOALKEEPER_RIGHT = 15;
		DOWN_Y_GOALKEEPER_LEFT = 20;
	}
}

void ProcessingCoord::setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow) {
	_x = x;
	_y = y;
	_dBlue = dBlue;
	_dYellow = dYellow;
	_angle = angle;
}

void ProcessingCoord::setMaxLen(float len) {
	_maxLen = len;
}

float ProcessingCoord::distance(float x, float y, float startX, float startY) {
	return sqrt(pow(x - startX, 2) + pow(y - startY, 2));
}

float ProcessingCoord::map(float a, float from1, float to1, float from2, float to2) {
	return to1 + a * (to2 - to1) / (from2 - from1);
}

int16_t ProcessingCoord::adduct(int16_t value) {
	while (value < 0) value += 360;
	while (value > 360) value -= 360;
	
	return value;
}

int16_t ProcessingCoord::adduct180(int16_t value) {
	while (value < -180) value += 360;
	while (value > 180) value -= 360;
	
	return value;
}

Vec2b ProcessingCoord::getVecToPoint(int16_t pointX, int16_t pointY) {
	float dist = sqrt(pow(float(pointX - _x), 2) + pow(float(pointY - _y), 2));
	float u = dist * 0.027f;
	
	return Vec2b(u, adduct(atan2(float(pointY - _y), float(pointX - _x)) * RAD2DEG));
}