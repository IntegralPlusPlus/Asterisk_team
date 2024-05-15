#include "BallSensor.h"

BallSensor::BallSensor(Dma& sens): _sensor(sens) {
	count2See = 0;
	timeInGrip = time_service::millis();
}

bool BallSensor::ballInGrip() {
	int16_t val = getValue();
	
	if (val > MINIMUM_ADC_SIGNAL && val < SEE_BALL && count2See < COUNT_THRESHOLD) count2See++;
	else count2See = 0;
	
	if (val >= SEE_BALL) timeInGrip = time_service::millis();
	
	return count2See >= COUNT_THRESHOLD;
}

bool BallSensor::ballLongTimeInGrip() {
	return time_service::millis() - timeInGrip > TIME_IN_GRIP;
}

int16_t BallSensor::getValue() {
	return _sensor.dataReturn(0);
}