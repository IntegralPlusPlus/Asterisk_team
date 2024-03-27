#include "omniplatform.h"

omniplatform::omniplatform(Motor &m1, Motor &m2, Motor &m3, Motor &m4): 
	_m1(m1), _m2(m2), _m3(m3), _m4(m4)  {
}
	
void omniplatform::move(float maxSpeed, float nowSpeed, float angle, float inc, float maxInc) {
	float vecAng;
	int32_t motorVal;
	//maxSpeed *= 0.9;
	float toMotorSpeed;
	if (maxSpeed != 0)	toMotorSpeed = _m1.getMaxSpeed() / maxSpeed;
	else toMotorSpeed = 0;
	
	if (inc > maxInc) inc = maxInc;
	else if (inc < -maxInc) inc = -maxInc;
	
	if (nowSpeed > maxSpeed) nowSpeed = maxSpeed;
	else if (nowSpeed < -maxSpeed) nowSpeed = -maxSpeed;
	angle = adduct0_360(angle);
	
	vecAng = (angle - 45) * DEG2RAD;
	motorVal = nowSpeed * -sin(vecAng) * toMotorSpeed - inc;
	_m1.go(-motorVal);
	
	vecAng = (angle - 135) * DEG2RAD;
	motorVal = nowSpeed * -sin(vecAng) * toMotorSpeed - inc;
	_m2.go(-motorVal);
	
	vecAng = (angle - 225) * DEG2RAD;
	motorVal = nowSpeed * -sin(vecAng) * toMotorSpeed - inc;
	_m3.go(-motorVal);
	
	vecAng = (angle - 315) * DEG2RAD;
	motorVal = nowSpeed * -sin(vecAng) * toMotorSpeed - inc;
	_m4.go(-motorVal);
}

void omniplatform::disable() {
	_m1.disable();
	_m2.disable();
	_m3.disable();
	_m4.disable();
}

float omniplatform::adduct0_360(float angle) {
	while (angle < 0) angle += 360;
  while (angle > 360) angle -= 360;
  
	return angle;
}