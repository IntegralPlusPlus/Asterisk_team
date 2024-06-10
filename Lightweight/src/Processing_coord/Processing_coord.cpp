#include "Processing_coord.h"

ProcessingCoord::ProcessingCoord() {
	_maxLen = 1;
	
	upThreshold = UP_Y - DELTA_DIST;
	downThreshold = DOWN_Y + 2.5 * DELTA_DIST;
	if (_goal == YELLOW_GOAL) {
		leftThreshold = -78 + DELTA_DIST;//THRESHOLD_X_LEFT + 3.5 * DELTA_DIST;
		rightThreshold = 58 - DELTA_DIST;//THRESHOLD_X_RIGHT - 1 * DELTA_DIST;
	} else {
		leftThreshold = THRESHOLD_X_LEFT + 4.8 * DELTA_DIST; //4.8
		rightThreshold = THRESHOLD_X_RIGHT - 3 * DELTA_DIST; //2
	}
}

void ProcessingCoord::setGoal(uint8_t currentGoal) {
	_goal = currentGoal;
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
	if (a > to1) a = to1;
	if (a < from1) a = from1;
	if (to1 > from1) return from2 + (to2 - from2) * (a - from1) / (to1 - from1);
	else return from2;
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

int16_t ProcessingCoord::getTarget2Enemy() {
	//                                     DIST_BETWEEN_GOALS
	return adduct180(RAD2DEG * atan2(float(219 - _y), float(-_x)) - 90);
}

Vec2b ProcessingCoord::getVecToPoint(int16_t pointX, int16_t pointY) {
	float dist = sqrt(pow(float(pointX - _x), 2) + pow(float(pointY - _y), 2));
	float u = dist * 0.057f; //0.027
	if (u < 0.25f && !(pointX == 0 && pointY == GOAL_OUT_Y_THRESHOLD)) u = 0.25f;
	else if (u > 0.6f) u = 0.6f;
	
	return Vec2b(u, adduct(atan2(float(pointY - _y), float(pointX - _x)) * RAD2DEG));
}

float ProcessingCoord::getAngleBetween(float ang1, float ang2) {
	if (abs(ang1 - ang2) < 180) return abs(ang1 - ang2);
	else if (ang2 < ang1) return abs(ang2 + 360 - ang1);
	else return abs(ang2 - ang1 - 360);
}

bool ProcessingCoord::ballInBack(float angBall, uint8_t varible) {
	float angLeft = 180 + adduct180(RAD2DEG * atan2(float(_y), float(_x - LEFT_GOAL_THRESHOLD)));
	float angRight = 180 + adduct180(RAD2DEG * atan2(float(_y), float(_x - RIGHT_GOAL_THRESHOLD)));
	angBall -= _angle;
	
	if (varible == tsopRaw) return adduct(90 + angBall) >= angLeft && adduct(90 + angBall) <= angRight;
	else return adduct(angBall) >= angLeft && adduct(angBall) <= angRight;
}

bool ProcessingCoord::suitableParams2Kick() {
	float angLeft = atan2(float(LEFT_GOAL_THRESHOLD - _x), float(DIST_BETWEEN_GOALS - _y)) * RAD2DEG;
	float angRight = atan2(float(RIGHT_GOAL_THRESHOLD - _x), float(DIST_BETWEEN_GOALS - _y)) * RAD2DEG;

	return _angle >= angLeft && _angle <= angRight;
}

bool ProcessingCoord::checkXLeft(int16_t x, uint8_t role) {
	if (role == GOALKEEPER_ROLE) return x > THRESHOLD_X_LEFT + DELTA_DIST + SAVE_DELTA_GK;//leftThreshold - 3.5 * DELTA_DIST + SAVE_DELTA_GK;
	else {
		if (_y > 210 - 70) {
			return x > leftThreshold - 9;
		} else {
			return x > leftThreshold;
		}
	}
}

bool ProcessingCoord::checkXRight(int16_t x, uint8_t role) {
	if (role == GOALKEEPER_ROLE) return x < THRESHOLD_X_RIGHT - DELTA_DIST - SAVE_DELTA_GK / 2;//rightThreshold + 2 * DELTA_DIST - SAVE_DELTA_GK;
	else {
		if (_y > 210 - 40) {
			return x < rightThreshold + 10;
		} else {
			return x < rightThreshold;
		}
	}
}

bool ProcessingCoord::checkYUp(int16_t y) {
	return y < upThreshold;
}

bool ProcessingCoord::checkYDown(int16_t y) {
	return y > downThreshold;
}