#include "Goalkeeper.h"

Goalkeeper::Goalkeeper(): ProcessingCoord() {
	_targetIMU = 0;
	
	errOldGkLeft = 0;
	errOldGkRight = 0;
	errOldGkLine = 0;
	timerLeave = 0;
	countBall = 0;
	
	if (_goal == BLUE_GOAL) {
		_downYRight = 32;
		_downYLeft = 28;
	} else {
		_downYRight = 27;
		_downYLeft = 26;
	}
}

int16_t Goalkeeper::getTargetGoalkeeper() {
	_targetIMU = adduct180(RAD2DEG * atan2(float(_y), float(_x)) - 90);
	
	return _targetIMU;
}

int16_t Goalkeeper::getTargetRadiuses() {
	_targetIMU = RAD2DEG * atan2(float(_y), float(_x)) - 180;
	
	return adduct(_targetIMU + _angle);
}

void Goalkeeper::setLeaveTime(int16_t leaveTime) {
	_leaveTime = leaveTime;
}

int16_t Goalkeeper::getCurrentLeaveTime(int16_t angBall) {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	uint8_t gkPos = getGoalkeeperPos();
	int16_t globalAng2Ball = adduct180(angBall - _angle);
	
	if (angGoal < ANGLE_LOW_DOESNT_LEAVE || angGoal > ANGLE_HIGH_DOESNT_LEAVE 
			|| globalAng2Ball > 90 || globalAng2Ball < -90) return 0;
	else if (gkPos == leftPart || gkPos == rightPart) return _leaveTime / 5;
	else return _leaveTime;
}

uint8_t Goalkeeper::getGoalkeeperPos() {
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	if (angGoal >= ANGLE_LOW_TO_CIRCLE && angGoal <= ANGLE_HIGH_TO_CIRCLE) return centralLine;
	else if (angGoal > ANGLE_HIGH_TO_CIRCLE) return leftPart;
	else return rightPart;
}

Vec2b Goalkeeper::getVecToGoalCenter() {
	Vec2b vec;
	float p, d, u;
	static float errOld = 0;
	uint8_t gkPos = getGoalkeeperPos();
	
	if (gkPos == centralLine) {
		int16_t err = -GOAL_OUT_Y_THRESHOLD + _y;
		float speed = err * 0.06f; 
		vec = Vec2b(speed, 270 + _angle); 
		//errOld = 0;
	} else {
		float err, speed;
		distToGoalCenter = sqrt(float(pow(float(_x), 2) + pow(float(_y), 2)));
		if (gkPos == leftPart) {
			err = -RADIUS_GOAL_OUT_LEFT + distToGoalCenter;
			p = err * 0.045f; //0.058
			d = (err - errOld) * 1.f;
			u = p + d;
			errOld = err;
		} else if (gkPos == rightPart) {
			err = -RADIUS_GOAL_OUT_RIGHT + distToGoalCenter;
			p = err * 0.042f;
			d = (err - errOld) * 1.f;
			u = p + d;
			errOld = err;
		}
		
		vec = Vec2b(u, getTargetRadiuses()); 
	}
	
	if (vec.length > _maxLen) vec.length = _maxLen;
	
	return vec;
}

Vec2b Goalkeeper::getVecToIntersection(int16_t angBall) {
	Vec2b res;
	
	int16_t angGoal = RAD2DEG * atan2(float(_y), float(_x));
	globalAngToBall = adduct(angBall - _angle);
	int16_t angleBallGoal = adduct(angGoal + globalAngToBall);
	uint8_t gkPos = getGoalkeeperPos();
	
	if (_y <= CRITICAL_DOWN_Y) return Vec2b(_maxLen, 90 + _angle);
	else if (ballInBack(angBall, byVec)) return Vec2b(0, 0);
	else if (globalAngToBall > 270) globalAngToBall -= 360;
	
	if (gkPos == centralLine) {
		if (globalAngToBall > angGoal) res.angle = 180 + _angle;
		else res.angle = _angle;

		float err, p, d, u;
		err = pow(abs(float(globalAngToBall - angGoal)), 2.2f); //1.3f
		p = 0.00043f * err; //0.00087 0.0015 0.0041f
		d = (err - errOldGkLine) * 0.012f; //0.08f
		u = p + d;
		errOldGkLine = err;
		
		res.length = u; //0.011 1.2
	} else {
		float err, p, d, u;
		
		if (gkPos == rightPart) {
			if (globalAngToBall > angGoal) res.angle = adduct(RAD2DEG * atan2(float(_y), float(_x)) + 90);
			else res.angle = adduct(180 + RAD2DEG * atan2(float(_y), float(_x)) + 90);
				
			err = pow(abs(float(globalAngToBall - angGoal)), 1.4f); //1.1
			p = 0.0054f * err; //0.004
			d = (err - errOldGkRight) * 0.07f; //0.05
			u = p + d;
			errOldGkRight = err;
			
			res.length = u;
			if (//(_x > GK_X_THRESHOLD_RIGHT && (res.angle > 270 || res.angle < 90)) 
					(_y <= _downYRight && (globalAngToBall < 15 || globalAngToBall > 210))) {
				res.angle = 90 + _angle;
				res.length = 0;
			} else if ((_x > GK_X_THRESHOLD_RIGHT && (res.angle > 270 + _angle || res.angle < 90 + _angle))) {
				res.angle = 180 + _angle;
				res.length = _maxLen;
			}
		} else if (gkPos == leftPart) {
			if (globalAngToBall > angGoal) res.angle = adduct(180 + RAD2DEG * atan2(float(_y), float(_x)) - 90);
			else res.angle = adduct(RAD2DEG * atan2(float(_y), float(_x)) - 90);
			
			err = pow(abs(float(globalAngToBall - angGoal)), 1.4f);
			p = 0.004f * err; //0.0045
			d = (err - errOldGkLeft) * 0.07f; //0.05
			u = p + d;
			errOldGkLeft = err;
			res.length = u;			

			//110 300
			if ((_y <= _downYLeft && 
					(globalAngToBall > 130 && globalAngToBall < 300))) {
				res.angle = 90 + _angle;
				res.length = 0;
			} else if ((_x < GK_X_THRESHOLD_LEFT && res.angle > 90 && res.angle < 270)) {
				res.angle = 0 + _angle;
				res.length = _maxLen;
			}
		}
	}
	
	if (res.length > 1.22f * _maxLen) res.length = 1.22f * _maxLen;
	
	return res;
}

Vec2b Goalkeeper::getVecToReturn() {
	float yTo = GOAL_OUT_Y_THRESHOLD, xTo = _x * yTo / _y;
	return getVecToPoint(xTo, yTo);
}

bool Goalkeeper::changeFromReturn() {
	return distance(_x, _y) < 1.15f * GOAL_OUT_Y_THRESHOLD; //1.35f
}

float Goalkeeper::getCoeffToGoalCenter(float intersec) {
	if (intersec < MAX_LEN_TO_INCREASE_VEC) return 1;
	else return map(intersec, MAX_LEN_TO_INCREASE_VEC, _maxLen, 1.f, MAX_COEFF_TO_GOAL_CENTER);
}

bool Goalkeeper::angleStopLeaving(float ang) {
	if (time_service::millis() != timerLeave && 
			abs(ang - _angle) >= ANGLE_TO_LEAVE / 2) {
		countBall++;
		timerLeave = time_service::millis();
	}
			
	if (countBall >= MS_BALL_NOT_FORWARD) {
		countBall = 0;
		return true;
	} else return false;
}

bool Goalkeeper::dist2GoalLong() {
	return distance(_x, _y) >= LONG_DISTANCE_TO_GOAL;
}

uint8_t Goalkeeper::setAngleLeaveStatus() {
	if (_x >= THRESHOLD_ANGLEIMU_LOW && _x <= THRESHOLD_ANGLEIMU_HIGH) return byEnemyGoal;
	else return byMyGoal;
}