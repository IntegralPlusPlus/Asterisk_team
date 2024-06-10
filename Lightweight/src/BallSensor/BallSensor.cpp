#include "BallSensor.h"

BallSensor::BallSensor(Dma& sens): _sensor(sens) {
	count2See = 0;
	timeInGrip = time_service::millis();
	valSoft = 0;
}

bool BallSensor::ballInGrip() {
	int16_t val = getValue();
	valSoft = 0.01 * float(val) + 0.99 * valSoft;
	
	if (valSoft > MINIMUM_ADC_SIGNAL && valSoft < SEE_BALL && count2See < COUNT_THRESHOLD) count2See++;
	else count2See = 0;
	
	if (valSoft >= SEE_BALL) timeInGrip = time_service::millis();
	
	return valSoft > MINIMUM_ADC_SIGNAL && valSoft < SEE_BALL;//count2See >= COUNT_THRESHOLD;
}

bool BallSensor::ballLongTimeInGrip() {
	return time_service::millis() - timeInGrip > TIME_IN_GRIP;
}

int16_t BallSensor::getValue() {
	return _sensor.dataReturn(0);
}