#include "Processing_coord.h"

ProcessingCoord::ProcessingCoord() {
	_targetIMU = 0;
	
	_leftFast.set(1, 0);
	_rightFast.set(1, 180);
	_upFast.set(1, 90);
	_downFast.set(1, 270);
	inOUT = false;
}

void ProcessingCoord::setGoal(uint8_t currentGoal) {
	_goal = currentGoal;
}

Vec2b ProcessingCoord::ñheckOUTs(Vec2b current) {
	inOUT = true;
	if (!checkXLeft(_x)) return _rightFast;
	else if (!checkXRight(_x)) return _leftFast;
	else if (!checkYUp(_y)) return _downFast;
	else if (!checkYDown(_y)) return _upFast;
	else {
		if (myGoalLine(_x, _y)) return _upFast;
		else if (enemyGoalLine(_x, _y)) return _downFast;
		else {
			if (isMyGoalCircle(_x, _y, _dBlue, _dYellow)) return getVecForMyCircle(_x, _y);
			else if (isEnemyGoalCircle(_x, _y, _dBlue, _dYellow)) return getVecForEnemyCircle(_x, _y);
			else {
				inOUT = false;
				return current;
			}
		}			
	} 
}

void ProcessingCoord::setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow) {
	_x = x;
	_y = y;
	_dBlue = dBlue;
	_dYellow = dYellow;
	_angle = angle;
	_leftFast.set(1, 0 - angle);
	_rightFast.set(1, 180 - angle);
	_upFast.set(1, 90 - angle);
	_downFast.set(1, 270 - angle);
}

bool ProcessingCoord::robotInOUT() {
	return inOUT;
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

int16_t ProcessingCoord::getTargetForward() {
	_targetIMU = RAD2DEG * atan2(float(_x - ENEMY_X), float(_y - ENEMY_Y)) - 180;
	return _targetIMU;
}

int16_t ProcessingCoord::getTargetGoalkeeper() {
	if (_x > GOAL_OUT_X_THRESHOLD) _targetIMU = RAD2DEG * atan2(float(_y), float(_x - GOAL_OUT_X_THRESHOLD));
	else if (_x < -GOAL_OUT_X_THRESHOLD) _targetIMU = RAD2DEG * atan2(float(_y), float(_x + GOAL_OUT_X_THRESHOLD));
	else _targetIMU = RAD2DEG * atan2(float(_y), float(_x));
		
	return _targetIMU;
}

Vec2b ProcessingCoord::getVecForEnemyCircle(int16_t x, int16_t y) {
	float ang = atan2(float(DIST_BETWEEN_GOALS - y), float(x)) * RAD2DEG;
	return Vec2b(1, adduct(-ang));
}

bool ProcessingCoord::isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	if (_goal == YELLOW_GOAL) {
		return dBlue && dBlue < RADIUS_GOAL_OUT 
					&& y > DIST_BETWEEN_GOALS - (GOAL_OUT_Y_THRESHOLD + DELTA_DIST);
	} else if (_goal == BLUE_GOAL) {
		return dBlue < RADIUS_GOAL_OUT
					&& y > DIST_BETWEEN_GOALS - (GOAL_OUT_Y_THRESHOLD + DELTA_DIST);
	} else return 0;
}

Vec2b ProcessingCoord::getVecToGoalCenter() {
	if (_x >= -GOAL_OUT_X_THRESHOLD && _x <= GOAL_OUT_X_THRESHOLD) {
		int16_t err = -GOAL_OUT_Y_THRESHOLD + _y;
		double speed = err * 0.04; 
		Vec2b vec = Vec2b(speed, 270 + _angle); 
	} else {
		float distToGoalCenter;
		if (_x > GOAL_OUT_X_THRESHOLD) distToGoalCenter = sqrt(float(pow(float(_x - GOAL_OUT_X_THRESHOLD), 2) + pow(float(_y), 2)));
		else if (_x < -GOAL_OUT_X_THRESHOLD) distToGoalCenter = sqrt(float(pow(float(_x + GOAL_OUT_X_THRESHOLD), 2) + pow(float(_y), 2)));
		
		//float distToGoalCenter = sqrt(float(_x * _x + _y * _y));
		
		float err = 0.8f * GOAL_OUT_Y_THRESHOLD - distToGoalCenter;
		double speed = err * 0.03; 
		Vec2b vec = Vec2b(speed, getTargetGoalkeeper()); 
	}
}

Vec2b ProcessingCoord::getVecToIntersection(int16_t angBall) {
	Vec2b res;
	int16_t angGoal = RAD2DEG * atan2(abs(double(_y)), abs(double(_x)));
	//if (angGoal > 90) angGoal = 180 - angGoal; 
	int16_t globalAngToBall = adduct(angBall + _angle);
	angleBallGoal = adduct(angGoal + globalAngToBall);
	uint8_t dir = getDirectionRobot(angleBallGoal);
	
	//if (_x >= -GOAL_OUT_X_THRESHOLD - 100 && _x <= GOAL_OUT_X_THRESHOLD + 100) {
		if (dir == GO_LEFT) res.angle = 180 + _angle;
		else if (dir == GO_RIGHT) res.angle = _angle;
	
		res.length = 0.01 * pow(abs(180 - float(angleBallGoal)), 1); //0.0015 2
	//} else {
	//	res.angle = RAD2DEG * atan2(double(_y), double(_x));
	//	res.length = 0 * pow(abs(180 - float(angleBallGoal)), 2);
	//}
	
	return res;
}

bool ProcessingCoord::getDirectionRobot(int16_t angle) {
	if (angle > 180) return GO_LEFT;
	else return GO_RIGHT;
}

Vec2b ProcessingCoord::getVecForMyCircle(int16_t x, int16_t y) {
	float ang = atan2(float(y), float(x)) * RAD2DEG;
	return Vec2b(1, adduct(ang));
}

bool ProcessingCoord::isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	if (_goal == YELLOW_GOAL) {
		return dYellow && dYellow < RADIUS_GOAL_OUT 
					&& y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST;
	} else if (_goal == BLUE_GOAL) {
		return dBlue && dBlue < RADIUS_GOAL_OUT 
					&& y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST;
	} else return 0;
}

bool ProcessingCoord::myGoalLine(int16_t x, int16_t y) {
	return x >= -GOAL_OUT_X_THRESHOLD && x <= GOAL_OUT_X_THRESHOLD 
				&& y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST;
}

bool ProcessingCoord::enemyGoalLine(int16_t x, int16_t y) {
	return x >= -GOAL_OUT_X_THRESHOLD && x <= GOAL_OUT_X_THRESHOLD 
				&& y > DIST_BETWEEN_GOALS - (GOAL_OUT_Y_THRESHOLD + DELTA_DIST);
}

bool ProcessingCoord::checkXLeft(int16_t x) {
	return x > -THRESHOLD_X + DELTA_DIST;
}

bool ProcessingCoord::checkXRight(int16_t x) {
	return x < THRESHOLD_X - DELTA_DIST;
}

bool ProcessingCoord::checkYUp(int16_t y) {
	return y < UP_Y - DELTA_DIST;
}

bool ProcessingCoord::checkYDown(int16_t y) {
	return y > DOWN_Y + DELTA_DIST;
}