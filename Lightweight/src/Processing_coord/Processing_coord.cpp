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
	critical = false;
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
	if (_x > GOAL_OUT_X_THRESHOLD_RIGHT) _targetIMU = adduct(RAD2DEG * atan2(float(_y), float(_x - GOAL_OUT_X_THRESHOLD_RIGHT)) - 180);
	else if (_x < GOAL_OUT_X_THRESHOLD_LEFT) _targetIMU = adduct(RAD2DEG * atan2(float(_y), float(_x - GOAL_OUT_X_THRESHOLD_LEFT)) - 180);
	else _targetIMU = adduct(RAD2DEG * atan2(float(_y), float(_x)) - 180);
	
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
	} else return false;
}

Vec2b ProcessingCoord::getVecToGoalCenter() {
	Vec2b vec;
	if (_x >= GOAL_OUT_X_THRESHOLD_LEFT && _x <= GOAL_OUT_X_THRESHOLD_RIGHT) {
		int16_t err = -GOAL_OUT_Y_THRESHOLD + _y;
		float speed = err * 0.051; 
		vec = Vec2b(speed, 270 + _angle); 
	} else {
		float err, speed;
		bool goToOUT = false;
	
		if (_x > GOAL_OUT_X_THRESHOLD_RIGHT) {
			distToGoalCenter = sqrt(float(pow(float(_x - GOAL_OUT_X_THRESHOLD_RIGHT), 2) + pow(float(_y), 2)));
			err = -GOAL_CIRCLE_Y_THRESHOLD_RIGHT + distToGoalCenter;
			speed = err * 0.043; //0.042
			
			if (speed < 0) {
				speed = -speed;
				goToOUT = true;
			}
		} else {
			distToGoalCenter = sqrt(float(pow(float(_x - GOAL_OUT_X_THRESHOLD_LEFT), 2) + pow(float(_y), 2)));
			err = -GOAL_CIRCLE_Y_THRESHOLD_LEFT + distToGoalCenter;
			speed = err * 0.043; //0.042
		
			if (speed < 0) {
				speed = -speed;
				goToOUT = true;
			}
		}
		
		vec = Vec2b(speed, adduct(180 * goToOUT + getTargetGoalkeeper())); 
	}
	
	return vec;
}

Vec2b ProcessingCoord::getVecToIntersection(int16_t angBall) {
	Vec2b res;
	critical = false;
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	int16_t globalAngToBall = adduct(angBall + _angle);
	int16_t angleBallGoal = adduct(angGoal + globalAngToBall);
	if (globalAngToBall > 270) globalAngToBall -= 360;
	
	if (_x >= GOAL_OUT_X_THRESHOLD_LEFT && _x <= GOAL_OUT_X_THRESHOLD_RIGHT) {
		if (globalAngToBall > angGoal) res.angle = 180 + _angle;
		else res.angle = _angle;

		float err, p, d, u;
		err = pow(abs(float(globalAngToBall - angGoal)), 1.42f); //1.21
		p = 0.0027 * err; //0.007
		d = (err - errOldGkLine) * 0.17; //0.07
		u = p + d;
		errOldGkLine = err;
		
		res.length = u; //0.011 1.2
	} else {
		float err, p, d, u;
		
		if (_x > GOAL_OUT_X_THRESHOLD_RIGHT) {
			if (globalAngToBall > angGoal) {
				res.angle = adduct(RAD2DEG * atan2(float(_y), float(_x - GOAL_OUT_X_THRESHOLD_RIGHT)) + 90);
			} else { 
				res.angle = 180 + adduct(RAD2DEG * atan2(float(_y), float(_x - GOAL_OUT_X_THRESHOLD_RIGHT)) + 90);
			}
				
			err = pow(abs(float(globalAngToBall - angGoal)), 1.14f); //1.11
			p = 0.004 * err; //0.005
			d = (err - errOldGkRight) * 0.08; //0.05
			u = p + d;
			errOldGkRight = err;
			res.length = u;
			
			if (_y < DOWN_Y_GOALKEEPER_RIGHT && (res.angle + _angle < 70 || res.angle + _angle > 180)) {
				res.angle = 90 + _angle;
				res.length = 0.045 * (DOWN_Y_GOALKEEPER_RIGHT - _y);
				critical = true;
			}
		} else if (_x < GOAL_OUT_X_THRESHOLD_LEFT) {
			if (globalAngToBall > angGoal) {
				res.angle = 180 + adduct(RAD2DEG * atan2(float(_y), float(_x - GOAL_OUT_X_THRESHOLD_LEFT)) - 90);
			} else { 
				res.angle = adduct(RAD2DEG * atan2(float(_y), float(_x - GOAL_OUT_X_THRESHOLD_LEFT)) - 90);
			}
			
			err = pow(abs(float(globalAngToBall - angGoal)), 1.14f);
			p = 0.004 * err;
			d = (err - errOldGkLeft) * 0.09; //0.05
			u = p + d;
			errOldGkLeft = err;
			res.length = u;			
			
			if (_y < DOWN_Y_GOALKEEPER_LEFT && res.angle + _angle > 110) {
				res.angle = 90 + _angle;
				res.length = 0.045 * (DOWN_Y_GOALKEEPER_LEFT - _y);
				critical = true;
			}
		}
	}
	
	return res;
}

bool ProcessingCoord::robotInCritical() {
	return critical;
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