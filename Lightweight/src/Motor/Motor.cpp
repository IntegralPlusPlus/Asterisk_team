#include "Motor.h"

Motor::Motor(Pin& in1, Pin& in2): _in1(in1), _in2(in2) 
{
}

void Motor::go(int16_t speed) {
	volatile int16_t p = _in1.getPeriod() - 1;
	volatile int16_t sp = speed;
	
	if(speed > p) speed = p;
	else if (speed < -p) speed = -p;
	
	if (speed > 0) {
		_in1.pwm(p);//speed);
		_in2.pwm(p - speed);//p - speed);
	} else {
		_in1.pwm(p + speed);
		_in2.pwm(p);
	}	
}

void Motor::disable() {
	_in1.pwm(0);
	_in2.pwm(0);
}

int16_t Motor::getMaxSpeed() {
	return _in1.getPeriod();
}