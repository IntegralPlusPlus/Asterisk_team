#include "BallSensor.h"

BallSensor::BallSensor(Dma& sens): _sensor(sens) {
	count2See = 0;
}

bool BallSensor::ballInGrip() {
	int16_t val = getValue();
	
	if (val < SEE_BALL && count2See < COUNT_THRESHOLD) count2See++;
	else count2See = 0;
	
	return count2See >= COUNT_THRESHOLD;
}

int16_t BallSensor::getValue() {
	return _sensor.dataReturn(0);
}