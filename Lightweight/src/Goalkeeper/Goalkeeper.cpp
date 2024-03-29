#include "Goalkeeper.h"

Goalkeeper::Goalkeeper(): ProcessingCoord() {
	_targetIMU = 0;
	
	errOldGkLeft = 0;
	errOldGkRight = 0;
	errOldGkLine = 0;
}

int16_t Goalkeeper::getTargetGoalkeeper() {
	_targetIMU = RAD2DEG * atan2(float(_y), float(_x)) - 180;
	
	return adduct(_angle + _targetIMU);
}

void Goalkeeper::setLeaveTime(int16_t leaveTime) {
	_leaveTime = leaveTime;
}

int16_t Goalkeeper::getCurrentLeaveTime() {
	uint8_t gkPos = getGoalkeeperPos();
	
	if (gkPos == leftPart || gkPos == rightPart) return _leaveTime / 8;
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

Vec2b Goalkeeper::getVecToIntersection(int16_t angBall) {
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

Vec2b Goalkeeper::getVecToReturn() {
	float yTo = GOAL_OUT_Y_THRESHOLD, xTo = _x * yTo / _y;
	return getVecToPoint(xTo, yTo);
}

bool Goalkeeper::changeFromReturn() {
	return distance(_x, _y) < 1.5f * GOAL_OUT_Y_THRESHOLD;
}

float Goalkeeper::getCoeffToGoalCenter(float intersec) {
	if (intersec < MAX_LEN_TO_INCREASE_VEC) return 1;
	else return map(intersec, MAX_LEN_TO_INCREASE_VEC, _maxLen, 1, MAX_COEFF_TO_GOAL_CENTER);
}