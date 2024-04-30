#include "Forward.h"

Forward::Forward(): ProcessingCoord() {
	_targetIMU = 0;
	
	inOUT = false;
	_countCyclesOUT = 0;
	_countCyclesField = 0;
}

uint8_t Forward::checkOUTs() {
	uint8_t outStatus = unknow;
	inOUT = true;
	if (!checkYUp(_y)) outStatus = down;
	else if (!checkYDown(_y)) outStatus = up;
	else if (!checkXLeft(_x)) outStatus = right;
	else if (!checkXRight(_x)) outStatus = left;
	else {
		if (myGoalLine(_x, _y)) outStatus = up;
		else if (enemyGoalLine(_x, _y)) outStatus = down;
		else {
			if (isMyGoalCircle(_x, _y, _dBlue, _dYellow)) outStatus = myCircle;
			else if (isEnemyGoalCircle(_x, _y, _dBlue, _dYellow)) outStatus = enemyCircle;
			else inOUT = false;
		}			
	}

	if (outStatus != unknow) {
		_countCyclesOUT++;
		_countCyclesField = 0;
	} else {
		_countCyclesOUT = 0;
		_countCyclesField++;
		if (_countCyclesField > 10000) _countCyclesField = 10000;
	}
		
	return outStatus;
}

Vec2b Forward::setOUTVector(uint8_t status, Vec2b current) {
	switch (status) {
		case up:
			return Vec2b(_maxLen, adduct(90 + _angle));
			break;
		case down:
			return Vec2b(_maxLen, adduct(270 + _angle));
			break;
		case left:
			return Vec2b(_maxLen, adduct(180 + _angle)) + projectionOnY(current);
			break;
		case right:
			return Vec2b(_maxLen, adduct(_angle)) + projectionOnY(current);
			break;
		case enemyCircle:
			return getVecForEnemyCircle(_x, _y);
			break;
		case myCircle:
			return getVecForMyCircle(_x, _y);
			break;
		default:
			return current;
	}
}

Vec2b Forward::projectionOnY(Vec2b vec) {
	float len = vec.length * sin(vec.angle * DEG2RAD), angTo = 90 + _angle;
	if (len < 0) {
		len *= -1;
		angTo = 270 + _angle;
	}
	
	return Vec2b(len, angTo);
}

bool Forward::robotInOUT() {
	return _countCyclesOUT > LIMIT_OUT_CYCLES;
}

bool Forward::robotInFreeField() {
	return _countCyclesField > LIMIT_OUT_CYCLES;
}

void Forward::resetCounts() {
	_countCyclesOUT = 0;
	_countCyclesField = 0;
}

Vec2b Forward::getVecForMyCircle(int16_t x, int16_t y) {
	float ang = atan2(float(y), float(x)) * RAD2DEG;
	return Vec2b(_maxLen, adduct(ang + _angle));
}

Vec2b Forward::getVecForEnemyCircle(int16_t x, int16_t y) {
	float ang = atan2(float(DIST_BETWEEN_GOALS - y), float(x)) * RAD2DEG;
	if (x > 0) return Vec2b(_maxLen, adduct(270 + ang + _angle));
	else return Vec2b(_maxLen, adduct(90 + ang + _angle));
}

Vec2b Forward::vec2bOnGoal(float speed, float angBall) {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	
	angBall -= _angle;
	if (angGoal < ANGLE_LOW_TO_CIRCLE) {
		volatile float angleTanget;	
		if (getBallSide(angBall) == up_left) angleTanget = adduct(adduct(90 + angGoal) - 180);
		else angleTanget = adduct(90 + angGoal);
		
		return Vec2b(speed, angleTanget);
	} else if (angGoal > ANGLE_HIGH_TO_CIRCLE) {
		float angleTanget = adduct(90 + angGoal);
		if (getBallSide(angBall) == up_left) angleTanget = adduct(90 + angGoal);
		else angleTanget = adduct(adduct(90 + angGoal) - 180);
		
		return Vec2b(speed, angleTanget);
	} else {
		if (angBall < 0) return Vec2b(speed, 0);
		else return Vec2b(speed, 180);
	}
}

uint8_t Forward::getBallSide(float angBall) {
	if (angBall < -ANGLE2SIDES || angBall > 180 - ANGLE2SIDES) return down_right;
	else return up_left;
}

uint8_t Forward::setFieldZone() {
	float sidesSize = float(rightThreshold - leftThreshold) / 6;
	
	//return middleZone;
	if (_x <= leftThreshold + sidesSize) return leftZone;
	else if (_x >= rightThreshold - sidesSize) return rightZone;
	else return middleZone;
}

bool Forward::isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	int16_t angGoal = RAD2DEG * atan2(float(DIST_BETWEEN_GOALS - _y), float(_x));
	
	if (x > 0) return distance(x, y, 0, DIST_BETWEEN_GOALS) < 1.15 * RADIUS_GOAL_OUT_LEFT && angGoal < ANGLE_LOW_TO_CIRCLE; 
	else return distance(x, y, 0, DIST_BETWEEN_GOALS) < 1.15 * RADIUS_GOAL_OUT_RIGHT && angGoal > ANGLE_HIGH_TO_CIRCLE; 
}

bool Forward::isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	
	if (x > 0) return distance(x, y) < float(COEFF_CIRCLE * RADIUS_GOAL_OUT_RIGHT) && angGoal < ANGLE_LOW_TO_CIRCLE; 
	else return distance(x, y) < float(COEFF_CIRCLE * RADIUS_GOAL_OUT_LEFT) && angGoal > ANGLE_HIGH_TO_CIRCLE; 
}

bool Forward::myGoalLine(int16_t x, int16_t y) {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	
	return y < GOAL_OUT_Y_THRESHOLD + 0.5 * DELTA_DIST &&
				 angGoal > ANGLE_LOW_TO_CIRCLE && angGoal < ANGLE_HIGH_TO_CIRCLE;
}

bool Forward::enemyGoalLine(int16_t x, int16_t y) {
	int16_t angGoal = RAD2DEG * atan2(float(DIST_BETWEEN_GOALS - _y), float(_x));
	
	return y > DIST_BETWEEN_GOALS - (GOAL_OUT_Y_THRESHOLD + 1.5 * DELTA_DIST) &&
				 angGoal > ANGLE_LOW_TO_CIRCLE && angGoal < ANGLE_HIGH_TO_CIRCLE;
}

bool Forward::inEnemyGoal() {
	return enemyGoalLine(_x, _y) || isEnemyGoalCircle(_x, _y, _dBlue, _dYellow);
}

bool Forward::inMyGoal() {
	return myGoalLine(_x, _y) || isMyGoalCircle(_x, _y, _dBlue, _dYellow);
}

bool Forward::nearMyGoal() {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	
	bool goalLine = _y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST + NEAR_OUT_DIST &&
									angGoal > ANGLE_LOW_TO_CIRCLE && angGoal < ANGLE_HIGH_TO_CIRCLE;
	bool goalCircle;
	
	if (_x > 0) {
		goalCircle = distance(_x, _y) < NEAR_OUT_DIST + float(COEFF_CIRCLE * RADIUS_GOAL_OUT_RIGHT) 
									&& angGoal < ANGLE_LOW_TO_CIRCLE; 
	} else {
		goalCircle = distance(_x, _y) < NEAR_OUT_DIST + float(COEFF_CIRCLE * RADIUS_GOAL_OUT_LEFT) 
									&& angGoal > ANGLE_HIGH_TO_CIRCLE; 
	}
	
	return goalLine || goalCircle;
}

uint8_t Forward::robotNearOUT() {
	if (_x - leftThreshold < NEAR_OUT_DIST) return left;
	else if (rightThreshold - _x < NEAR_OUT_DIST) return right;
	else if (_y - downThreshold < NEAR_OUT_DIST) return down;
	else if (upThreshold - _y < NEAR_OUT_DIST) return up;
	else return unknow;
}

float Forward::setNearSpeed(uint8_t status, float maxSpeed) {
	float minSpeed = MINIMUM_SPEED_TO_BALL;
	if (status == down) {
		return map(_y, downThreshold, downThreshold + NEAR_OUT_DIST, maxSpeed, minSpeed);
	} else if (status == up) {
		return map(_y, upThreshold - NEAR_OUT_DIST, upThreshold, maxSpeed, minSpeed);
	} else if (status == left) {
		return map(_x, leftThreshold, leftThreshold + NEAR_OUT_DIST, maxSpeed, minSpeed);
	} else if (status == right) {
		return map(_x, rightThreshold - NEAR_OUT_DIST, rightThreshold, maxSpeed, minSpeed);
	} else return maxSpeed;
}