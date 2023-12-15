#include "Motor.h"

Motor::Motor(Pin& in1, uint8_t channelIN1, Pin& in2, uint8_t channelIN2): _in1(in1), _in2(in2) 
{
	_in1.pwmInit(2048, 1, channelIN1);
	_in2.pwmInit(2048, 1, channelIN2);
}

Motor::Motor(Pin& in1, Pin& in2): _in1(in1), _in2(in2) 
{
}

void Motor::go(int16_t speed) {
	volatile int16_t p = _in1.getPeriod() - 1;
	volatile int16_t sp = speed;
	
	if(speed > p) speed = p;
	else if (speed < -p) speed = -p;
	
	if (speed > 0) {
		_in1.pwm(p);
		_in2.pwm(p - speed);
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