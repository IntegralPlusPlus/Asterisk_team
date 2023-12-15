#include "IR_Locator.h"

IRLocator::IRLocator(I2C &irlI2C, uint32_t address): _irlI2C(irlI2C)
{
	_address = address;
}

int16_t IRLocator::getValue(uint8_t data) {
	_irlI2C.generateStart();
  _irlI2C.send(_address << 1);
  _irlI2C.readAch();
    
  _irlI2C.send(data);
  _irlI2C.readAch();
  
	_irlI2C.generateStop();
  _irlI2C.generateStart();
  uint32_t tmp1 = _address << 1;
  tmp1 = tmp1 |= 0x01;
  _irlI2C.send(tmp1);
  _irlI2C.readAch();
  uint32_t tmp2 = _irlI2C.read();
  _irlI2C.generateStop();
   
	return tmp2;
}

int16_t IRLocator::getAngle() {
	_dist = getValue(DISTANCE);
		
	if (_dist < 10) _angle = 5 * getValue(ANG_CLOSE);
	else _angle = 5 * getValue(ANG_FAR);
		
	_angle = adduct(-(_angle - 90));
	//_angle = adduct(_angle);
	
	return _angle;
}

int32_t IRLocator::getDist() {
	return _dist;
}

double IRLocator::convertDist(double dist) {
	double maxDist = 25;
  double v = (dist - maxDist) / maxDist + 1;
  if (v > 1) v = 1;
  if (v < 0) v = 0;
	
	return v;
}

float IRLocator::angleOffset(float angle, float dist){
  double angK = 0.011 * pow(double(Ec), double(0.18 * abs(angle))); //0.04 0.15
  if (angK > 90)
    angK = 90;
  dist = convertDist(dist);
  double distK = 0.044 * pow(double(Ec), double(4 * dist));//0.05 4
  if (distK > 1)
    distK = 1;
  if (angle > 0) {
    return angK * distK;
  } else {
    return -angK * distK;
  }
}

int16_t IRLocator::adduct(int16_t a) {
	while (a > 180) a -= 360;
	while (a < -180) a += 360;
	
	return a;
}