#include "Processing_coord.h"

ProcessingCoord::ProcessingCoord() {
	_targetIMU = 0;
	
	_leftFast.set(1, 0);
	_rightFast.set(1, 180);
	_upFast.set(1, 90);
	_downFast.set(1, 270);
	inOUT = false;
	
	errOldGkLeft = 0;
	errOldGkRight = 0;
	errOldGkLine = 0;
}

void ProcessingCoord::setGoal(uint8_t currentGoal) {
	_goal = currentGoal;
}

Vec2b ProcessingCoord::checkOUTs(Vec2b current) {
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
	_leftFast.set(1, adduct(0 + angle));
	_rightFast.set(1, adduct(180 + angle));
	_upFast.set(1, adduct(90 + angle));
	_downFast.set(1, adduct(270 + angle));
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
	_targetIMU = RAD2DEG * atan2(float(_y), float(_x)) - 180;
	
	return adduct(_angle + _targetIMU);
}

Vec2b ProcessingCoord::getVecForEnemyCircle(int16_t x, int16_t y) {
	float ang = atan2(float(DIST_BETWEEN_GOALS - y), float(x)) * RAD2DEG;
	return Vec2b(1, adduct(-ang));
}

bool ProcessingCoord::isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	if (_goal == YELLOW_GOAL) {
		return dBlue && dBlue < RADIUS_GOAL_OUT_RIGHT
					&& y > DIST_BETWEEN_GOALS - (GOAL_OUT_Y_THRESHOLD + DELTA_DIST);
	} else if (_goal == BLUE_GOAL) {
		return dBlue < RADIUS_GOAL_OUT_RIGHT
					&& y > DIST_BETWEEN_GOALS - (GOAL_OUT_Y_THRESHOLD + DELTA_DIST);
	} else return false;
}

Vec2b ProcessingCoord::getVecToGoalCenter() {
	Vec2b vec;
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	float p, d, u;
	static float errOld = 0;
	
	if (angGoal >= ANGLE_LOW_TO_CIRCLE && angGoal <= ANGLE_HIGH_TO_CIRCLE) {
		int16_t err = -GOAL_OUT_Y_THRESHOLD + _y;
		float speed = err * 0.053f; 
		vec = Vec2b(speed, 270 + _angle); 
		//errOld = 0;
	} else {
		float err, speed;
		distToGoalCenter = sqrt(float(pow(float(_x), 2) + pow(float(_y), 2)));
		if (angGoal > ANGLE_HIGH_TO_CIRCLE) {
			err = -RADIUS_GOAL_OUT_LEFT + distToGoalCenter;
			p = err * 0.042f; //0.042
			d = (err - errOld) * 0.7f;
			u = p + d;
			errOld = err;
		} else if (angGoal < ANGLE_LOW_TO_CIRCLE) {
			err = -RADIUS_GOAL_OUT_RIGHT + distToGoalCenter;
			p = err * 0.054f;
			d = (err - errOld) * 0.7f;
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
	if (globalAngToBall > 180 + (180 - BACK_ANGLE) / 2 
			&& globalAngToBall < 360 - (180 - BACK_ANGLE) / 2) return Vec2b(0, 0);
	else if (globalAngToBall > 270) {
		globalAngToBall -= 360;
	}
	
	if (angGoal >= ANGLE_LOW_TO_CIRCLE && angGoal <= ANGLE_HIGH_TO_CIRCLE) {
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
		
		if (angGoal < ANGLE_LOW_TO_CIRCLE) {
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
		} else if (angGoal > ANGLE_HIGH_TO_CIRCLE) {
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
	return distance(_x, _y) < 1.1f * GOAL_OUT_Y_THRESHOLD;
}

float ProcessingCoord::distance(float x, float y, float startX, float startY) {
	return sqrt(pow(x - startX, 2) + pow(y - startY, 2));
}

Vec2b ProcessingCoord::getVecToPoint(int16_t pointX, int16_t pointY) {
	float dist = sqrt(pow(float(pointX - _x), 2) + pow(float(pointY - _y), 2));
	float u = dist * 0.033f;
	
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
	return Vec2b(1, adduct(ang));
}

bool ProcessingCoord::isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow) {
	if (_goal == YELLOW_GOAL) {
		return dYellow && dYellow < RADIUS_GOAL_OUT_RIGHT
					&& y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST;
	} else if (_goal == BLUE_GOAL) {
		return dBlue && dBlue < RADIUS_GOAL_OUT_RIGHT
					&& y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST;
	} else return false;
}

bool ProcessingCoord::myGoalLine(int16_t x, int16_t y) {
	return x >= GOAL_OUT_X_THRESHOLD_LEFT && x <= GOAL_OUT_X_THRESHOLD_RIGHT
				&& y < GOAL_OUT_Y_THRESHOLD + DELTA_DIST;
}

bool ProcessingCoord::enemyGoalLine(int16_t x, int16_t y) {
	return x >= GOAL_OUT_X_THRESHOLD_LEFT && x <= GOAL_OUT_X_THRESHOLD_RIGHT
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