#include "I2C.h"

I2C::I2C(Pin &scl, Pin &sda): _scl(scl), _sda(sda) {
	
}

bool I2C::sclRead() {
  bool a = (_scl.getGPIOx()->IDR & _scl.getPin());
  return a;
}

bool I2C::sdaRead() {
  return (_sda.getGPIOx()->IDR & _sda.getPin()); 
}

void I2C::sdaHigh() {
  _sda.setBit();
}    

void I2C::sdaLow() {
  _sda.resetBit();
}

void I2C::sclHigh() {
	_scl.setBit();
}  

void I2C::sclLow() {
  _scl.resetBit();
}

void I2C::delay() {
  volatile uint8_t i = 50;
  while (i != 0) i--;
}

bool I2C::generateStart() {
  sdaHigh();
  sclHigh();
  delay();
  if (!sdaRead()) return false;
  sdaLow();
  delay();
  if (sdaRead()) return false;
  sclLow();
  delay();
	
  return true;
}

bool I2C::readAch() {
	sclLow();
  delay();
  sclHigh();
  delay();
  
	if(sdaRead()) {
    sclLow();
    delay();
    return false;
  }

  sclLow();
  delay();
  
	return true;
}

void I2C::sendAch() {
  sclLow();
  delay();
  sdaLow();
  delay();
  sclHigh();
  delay();
  sclLow();
  delay();
}

void I2C::sendNach() {
	sclLow();
  delay();
  sdaHigh();
  delay();
  sclHigh();
  delay();
  sclLow();
  delay();
}

bool I2C::generateStop() {
	sclLow();
	delay();
  sdaLow();
  delay();
  sclHigh();
  delay();
  sdaHigh();
  delay();
  
	return true;
}

void I2C::writeBit(bool bit) {
  sclLow();
  delay();
	
  if(bit == 0) {
		sdaLow();
    delay();
	} else {
		sdaHigh();
    delay();
  }
		
	sclHigh();
  delay();  
}

uint32_t I2C::read() {
	sdaHigh();
    
  uint16_t number =0;
  for(int8_t i = 7; i >= 0; --i) {
		number <<= 1;
    sclLow();
    delay();
    sclHigh();
    delay();
    
    if(sdaRead()) number |= 0x01;
  }
		
  sclLow();
    
	return number;
}

bool I2C::send(uint8_t package) {
  for(uint8_t i = 0x80; i > 0x00; i >>= 1) { //0x80
		if(package & i)
			writeBit(1);
    else
			writeBit(0);
  }
		
  sclLow();
  delay();
	
	return 1;
}