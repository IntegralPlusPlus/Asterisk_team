#include "TSOP.h"

TSOP::TSOP(Pin& w1, Pin& w2, Pin& w3, Pin& w4, Pin& mux1input, Pin& mux2input): 
_w1(w1), _w2(w2), _w3(w3), _w4(w4), _mux1input(mux1input), _mux2input(mux2input)  
{
	for (uint8_t i = 0; i < 32; ++i) {
		tsopValues[i] = 0;
	}
	
	_angle = 0;
}

TSOP::TSOP() {
	for (uint8_t i = 0; i < 32; ++i) {
		tsopValues[i] = 0;
	}
	
	_angle = 0;
}

bool TSOP::updateTSOPs() {
	bool iSeeBall = false;
	for (uint8_t i = 0; i < 16; ++i) {
		bool addres[] = {i / 8 % 2, i / 4 % 2, i / 2 % 2, i % 2};
		_w1.set(addres[0]);
		_w2.set(addres[1]);
		_w3.set(addres[2]);
		_w4.set(addres[3]);
	
		int16_t value1mux = !(_mux1input.getGPIOx()->IDR & _mux1input.getPin());
		int16_t value2mux = !(_mux2input.getGPIOx()->IDR & _mux2input.getPin());
		tsopValues[i] = value1mux;
		tsopValues[16 + i] = value2mux;
	
		if (value1mux || value2mux) iSeeBall = true;
	}
	
	return iSeeBall;
}

void TSOP::calculateAngle() {
	float vecX = 0, vecY = 0;
	
	for (uint8_t i = 0; i < 32; ++i) {
		float angleTSOP = 90 - i * TSOP1GRAD;
		vecX += cos(angleTSOP * DEG2RAD);
		vecY += sin(angleTSOP * DEG2RAD);
	}
	
	_dist = sqrt(vecY * vecY + vecX * vecX);
	_angle = atan2(vecY, vecX) * RAD2DEG;
	if (_angle < -180) _angle += 360;
}

int16_t TSOP::getAngle() {
	return _angle;
}

int16_t TSOP::getDist() {
	return _dist;
}

double TSOP::convertDist(double dist) {
	double maxDist = 25;
  double v = (dist - maxDist) / maxDist + 1;
  if (v > 1) v = 1;
  if (v < 0) v = 0;
	
	return v;
}

bool TSOP::distBad(int16_t distLocator) {
	return !distLocator || distLocator == 255;
}

double TSOP::angleOffset(double angle, double dist){
  double angK = 0.027 * pow(double(Ec), double(0.3 * abs(angle))); //0.04 0.15
  if (angK > 90)
    angK = 90;
  dist = convertDist(dist);
  double distK = 0.033 * pow(double(Ec), double(4.45 * dist));//0.05 4
  if (distK > 1) distK = 1;
	
  if (angle > 0) return angK * distK;
  else return -angK * distK;
}

int16_t TSOP::adduct(int16_t a) {
	while (a > 180) a -= 360;
	while (a < -180) a += 360;
	
	return a;
}