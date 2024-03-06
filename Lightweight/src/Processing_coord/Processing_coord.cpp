#include "Processing_coord.h"

ProcessingCoord::ProcessingCoord() {
	_targetIMU = 0;
	
	inOUT = false;
	_countCyclesOUT = 0;
	_countCyclesField = 0;
	_maxLen = 1;
	
	errOldGkLeft = 0;
	errOldGkRight = 0;
	errOldGkLine = 0;
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

uint8_t ProcessingCoord::checkOUTs() {
	uint8_t outStatus = unknow;
	inOUT = true;
	if (!checkXLeft(_x)) outStatus = right;
	else if (!checkXRight(_x)) outStatus = left;
	else if (!checkYUp(_y)) outStatus = down;
	else if (!checkYDown(_y)) outStatus = up;
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

Vec2b ProcessingCoord::setOUTVector(uint8_t status, Vec2b current) {
	switch (status) {
		case up:
			return Vec2b(_maxLen, adduct(90 + _angle));
			break;
		case down:
			return Vec2b(_maxLen, adduct(270 + _angle));
			break;
		case left:
			return Vec2b(_maxLen, adduct(_angle));
			break;
		case right:
			return Vec2b(_maxLen, adduct(180 + _angle));
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

void ProcessingCoord::setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow) {
	_x = x;
	_y = y;
	_dBlue = dBlue;
	_dYellow = dYellow;
	_angle = angle;
}

bool ProcessingCoord::robotInOUT() {
	return _countCyclesOUT > LIMIT_OUT_CYCLES;
}

bool ProcessingCoord::robotInFreeField() {
	return _countCyclesField > LIMIT_OUT_CYCLES;
}

void ProcessingCoord::resetCounts() {
	_countCyclesOUT = 0;
	_countCyclesField = 0;
}

int16_t ProcessingCoord::getTargetForward() {
	_targetIMU = -(90 - RAD2DEG * atan2(float(DIST_BETWEEN_GOALS - _y), float(-_x)));
	return _targetIMU;
}

int16_t ProcessingCoord::getTargetGoalkeeper() {
	_targetIMU = RAD2DEG * atan2(float(_y), float(_x)) - 180;
	
	return adduct(_angle + _targetIMU);
}

void ProcessingCoord::setLeaveTime(int16_t leaveTime) {
	_leaveTime = leaveTime;
}

int16_t ProcessingCoord::getCurrentLeaveTime() {
	uint8_t gkPos = getGoalkeeperPos();
	
	if (gkPos == leftPart || gkPos == rightPart) return _leaveTime / 8;
	else return _leaveTime;
}

uint8_t ProcessingCoord::getGoalkeeperPos() {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	if (angGoal >= ANGLE_LOW_TO_CIRCLE && angGoal <= ANGLE_HIGH_TO_CIRCLE) return centralLine;
	else if (angGoal > ANGLE_HIGH_TO_CIRCLE) return leftPart;
	else return rightPart;
}

Vec2b ProcessingCoord::getVecToGoalCenter() {
	Vec2b vec;
	float p, d, u;
	static float errOld = 0;
	uint8_t gkPos = getGoalkeeperPos();
	
	if (gkPos == centralLine) {
		int16_t err = -GOAL_OUT_Y_THRESHOLD + _y;
		float speed = err * 0.053f; 
		vec = Vec2b(speed, 270 + _angle); 
		//errOld = 0;
	} else {
		float err, speed;
		distToGoalCenter = sqrt(float(pow(float(_x), 2) + pow(float(_y), 2)));
		if (gkPos == leftPart) {
			err = -RADIUS_GOAL_OUT_LEFT + distToGoalCenter;
			p = err * 0.041f; //0.042
			d = (err - errOld) * 0.6f;
			u = p + d;
			errOld = err;
		} else if (gkPos == rightPart) {
			err = -RADIUS_GOAL_OUT_RIGHT + distToGoalCenter;
			p = err * 0.051f;
			d = (err - errOld) * 0.5f;
			u = p + d;
			errOld = err;
			//speed = err * 0.054f; //0.042
		}
		
		//speed = u;
		vec = Vec2b(u, getTargetGoalkeeper()); 
	}
	
	return vec;
}

Vec2b ProcessingCoord::getVecToIntersection(int16_t angBall) {
	Vec2b res;
	
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	int16_t globalAngToBall = adduct(angBall + _angle);
	int16_t angleBallGoal = adduct(angGoal + globalAngToBall);
	uint8_t gkPos = getGoalkeeperPos();
	
	if (globalAngToBall > 180 + (180 - BACK_ANGLE) / 2 
			&& globalAngToBall < 360 - (180 - BACK_ANGLE) / 2) return Vec2b(0, 0);
	else if (globalAngToBall > 270) {
		globalAngToBall -= 360;
	}
	
	if (gkPos == centralLine) {
		if (globalAngToBall > angGoal) res.angle = 180 + _angle;
		else res.angle = _angle;

		float err, p, d, u;
		err = pow(abs(float(globalAngToBall - angGoal)), 1.3f); //1.3f
		p = 0.0031f * err; //0.0035f
		d = (err - errOldGkLine) * 0.065f;
		u = p + d;
		errOldGkLine = err;
		
		res.length = u; //0.011 1.2
	} else {
		float err, p, d, u;
		
		if (gkPos == rightPart) {
			if (globalAngToBall > angGoal) res.angle = adduct(RAD2DEG * atan2(float(_y), float(_x)) + 90);
			else res.angle = adduct(180 + RAD2DEG * atan2(float(_y), float(_x)) + 90);
				
			err = pow(abs(float(globalAngToBall - angGoal)), 1.1f);
			p = 0.0032f * err;
			d = (err - errOldGkRight) * 0.065f; //0.05
			u = p + d;
			errOldGkRight = err;
			
			res.length = u;
			if (//(_x > GK_X_THRESHOLD_RIGHT && (res.angle > 270 || res.angle < 90)) 
					(_y <= DOWN_Y_GOALKEEPER_RIGHT && 
					(adduct(angBall + _angle) < 20 || adduct(angBall + _angle) > 255))) {
				res.angle = 0;
				res.length = 0;
			} else if ((_x > GK_X_THRESHOLD_RIGHT && (res.angle > 270 || res.angle < 90))) {
				res.angle = 180;
				res.length = 0.4;
			}
		} else if (gkPos == leftPart) {
			if (globalAngToBall > angGoal) res.angle = adduct(180 + RAD2DEG * atan2(float(_y), float(_x)) - 90);
			else res.angle = adduct(RAD2DEG * atan2(float(_y), float(_x)) - 90);
			
			err = pow(abs(float(globalAngToBall - angGoal)), 1.1f);
			p = 0.004f * err;
			d = (err - errOldGkLeft) * 0.065f; //0.05
			u = p + d;
			errOldGkLeft = err;
			res.length = u;			
			
			if ((_y <= DOWN_Y_GOALKEEPER_LEFT && 
					(adduct(angBall + _angle) > 100 && adduct(angBall + _angle) < 300))) {
				res.angle = 0;
				res.length = 0;
			} else if ((_x < GK_X_THRESHOLD_LEFT && res.angle > 90 && res.angle < 270)) {
				res.angle = 0;
				res.length = 0.4;
			}
		}
	}
	
	return res;
}

Vec2b ProcessingCoord::getVecToReturn() {
	float yTo = GOAL_OUT_Y_THRESHOLD, xTo = _x * yTo / _y;
	return getVecToPoint(xTo, yTo);
}

bool ProcessingCoord::changeFromReturn() {
	return distance(_x, _y) < 1.5f * GOAL_OUT_Y_THRESHOLD;
}

float ProcessingCoord::distance(float x, float y, float startX, float startY) {
	return sqrt(pow(x - startX, 2) + pow(y - startY, 2));
}

Vec2b ProcessingCoord::getVecToPoint(int16_t pointX, int16_t pointY) {
	float dist = sqrt(pow(float(pointX - _x), 2) + pow(float(pointY - _y), 2));
	float u = dist * 0.027f;
	
	return Vec2b(u, adduct(atan2(float(pointY - _y), float(pointX - _x)) * RAD2DEG));
}

void ProcessingCoord::setMaxLen(float len) {
	_maxLen = len;
}

float ProcessingCoord::getCoeffToGoalCenter(float intersec) {
	if (intersec < MAX_LEN_TO_INCREASE_VEC) return 1;
	else return map(intersec, MAX_LEN_TO_INCREASE_VEC, _maxLen, 1, MAX_COEFF_TO_GOAL_CENTER);
}

float ProcessingCoord::map(float a, float from1, float to1, float from2, float to2) {
	return to1 + a * (to2 - to1) / (from2 - from1);
}

Vec2b ProcessingCoord::getVecForMyCircle(int16_t x, int16_t y) {
	float ang = atan2(float(y), float(x)) * RAD2DEG;
	return Vec2b(_maxLen, adduct(ang + _angle));
}

Vec2b ProcessingCoord::getVecForEnemyCircle(int16_t x, int16_t y) {
	float ang = atan2(float(DIST_BETWEEN_GOALS - y), float(x)) * RAD2DEG;
	return Vec2b(_maxLen, adduct(180 + ang + _angle));
}

bool ProcessingCoord::isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	int16_t angGoal = RAD2DEG * atan2(float(DIST_BETWEEN_GOALS - _y), float(_x));
	
	if (x > 0) return distance(x, y, 0, DIST_BETWEEN_GOALS) < 1.1 * RADIUS_GOAL_OUT_LEFT && angGoal < ANGLE_LOW_TO_CIRCLE; 
	else return distance(x, y, 0, DIST_BETWEEN_GOALS) < 1.1 * RADIUS_GOAL_OUT_RIGHT && angGoal > ANGLE_HIGH_TO_CIRCLE; 
}

bool ProcessingCoord::isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	
	if (x > 0) return distance(x, y) < 1.1 * RADIUS_GOAL_OUT_RIGHT && angGoal < ANGLE_LOW_TO_CIRCLE; 
	else return distance(x, y) < 1.1 * RADIUS_GOAL_OUT_LEFT && angGoal > ANGLE_HIGH_TO_CIRCLE; 
}

bool ProcessingCoord::myGoalLine(int16_t x, int16_t y) {
	int16_t angGoal = RAD2DEG * atan2(float(DIST_BETWEEN_GOALS
		- _y), float(_x));
	
	return y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST &&
				 angGoal > ANGLE_LOW_TO_CIRCLE && angGoal < ANGLE_HIGH_TO_CIRCLE;
}

bool ProcessingCoord::enemyGoalLine(int16_t x, int16_t y) {
	int16_t angGoal = RAD2DEG * atan2(float(DIST_BETWEEN_GOALS - _y), float(_x));
	
	return y > DIST_BETWEEN_GOALS - (GOAL_OUT_Y_THRESHOLD + DELTA_DIST) &&
				 angGoal > ANGLE_LOW_TO_CIRCLE && angGoal < ANGLE_HIGH_TO_CIRCLE;
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