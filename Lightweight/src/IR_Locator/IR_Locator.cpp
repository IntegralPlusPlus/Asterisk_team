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
		
	_angle = adduct(-(_angle - 180)); //-90
	
	return _angle;
}

int32_t IRLocator::getDist() {
	return _dist;
}
