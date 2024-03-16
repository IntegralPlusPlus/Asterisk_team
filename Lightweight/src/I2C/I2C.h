#pragma once
#include "project_config.h"
#include "Pin.h"

class I2C {
	public:
		I2C();
		I2C(Pin& scl, Pin& sda);  
		void setI2C(Pin &scl, Pin &sda);
    void sclHigh();
    void sdaHigh();
    void sclLow();
    void sdaLow();
    bool generateStart();
    bool generateStop();
    void delay();
    bool sdaRead();
    bool sclRead();
    void writeBit(bool bit);
    bool send(uint8_t package);
    bool readAch();
    uint32_t read();
    void sendAch();
    void sendNach();
	private:
		Pin _scl, _sda;
		
};