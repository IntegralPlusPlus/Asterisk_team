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

void TSOP::updateTSOPs() {
	//bool iSeeBall = false;
	bool value1mux, value2mux;
	for (uint16_t i = 0; i < 16; ++i) {
		bool address[] = {(i / 8) % 2, (i / 4) % 2, (i / 2) % 2, i % 2};
		_w1.set(address[3]);
		_w2.set(address[2]);
		_w3.set(address[1]);
		_w4.set(address[0]);
	
		value1mux = (_mux1input.getGPIOx()->IDR & _mux1input.getPin());
		value2mux = (_mux2input.getGPIOx()->IDR & _mux2input.getPin());
		tsopValues[i] = value1mux;
		tsopValues[16 + i] = value2mux;
	
		//if (!value1mux || !value2mux) iSeeBall = true;
	}
	//return iSeeBall;
	//return iSeeBall;
}

void TSOP::calculate() {
	float vecX = 0.f, vecY = 0.f;
	
	for (float i = 0; i < 32; ++i) {
		float angleTSOP = float(i * TSOP1GRAD);
		vecX += float(!tsopValues[int16_t(i)]) * cos(angleTSOP * DEG2RAD);
		vecY += float(!tsopValues[int16_t(i)]) * sin(angleTSOP * DEG2RAD);
	}
	
	_angle = adduct(-float(atan2(vecY, vecX) * RAD2DEG) + 5);
	_dist = float(sqrt(vecY * vecY + vecX * vecX));
}

float TSOP::getAngle() {
	return _angle;
}

float TSOP::getDist() {
	return _dist;
}

double TSOP::convertDist(double dist) {
	double maxDist = 7;//8.5;//9.1 10.1; //7.85 //9.42
	double v = (dist - maxDist) / maxDist + 1;
  if (v > 1) v = 1;
  if (v < 0) v = 0;
	
	return v;
}

bool TSOP::distBad(int16_t distLocator) {
	return !distLocator || distLocator == 255;
}

double TSOP::angleOffset(double angle, double dist, double angleIMU){
  /*
	double angK = 0.24 * pow(double(Ec), double(0.3 * abs(angle))); //0.24 0.26				0.06 0.19
  if (angK > 90)
    angK = 90;
	
	
	dist *= 1.1; //1.1
  dist = convertDist(dist);
	//0.076 4.05
  double distK = 0.076 * pow(double(Ec), double(4.05 * abs(dist)));//0.0382 4					0.037 4.02
  */
	double angK = 0.195 * pow(double(Ec), double(0.2 * abs(angle))); //0.125 0.2 		0.065 0.19				0.06 0.19
  if (angK > 90)
    angK = 90;
	
	//dist *= 1.1;
  dist = convertDist(dist);
  double distK = 0.11 * pow(double(Ec), double(4.2 * abs(dist)));//0.074 4.02 0.0382 4					0.037 4.02
	if (distK > 1) distK = 1;
	
	double offset = angK * distK;
	if (offset > 180) offset = 180;
	
  if (adduct(angle/* - angleIMU*/) >= 0) return offset;
  else return -offset;
}

int16_t TSOP::adduct(int16_t a) {
	while (a > 180) a -= 360;
	while (a < -180) a += 360;
	 
	return a;
}

bool TSOP::ballFar(float dist) {
	return dist < BALL_FAR;
}