#pragma once
#include "project_config.h"
#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"

#define RAD2DEG	57.2957795130823208767f
#define DEG2RAD	0.01745329251994329576f

struct CoordsXY {
	CoordsXY(int16_t x, int16_t y) {
		this->x = x;
		this->y = y;
	}
	
	void set(int16_t x, int16_t y) {
		this->x = x;
		this->y = y;
	}
	
	int16_t x, y;
};

class Bluetooth {
	public:
		Bluetooth(Pin& tx, Pin& rx, uint8_t usartNum);
		void initUSART(uint8_t num);
		void read();
		void ballCalculate(int16_t myX, int16_t myY, int16_t myAngle);
		uint8_t crc8(volatile uint8_t* data, uint8_t len);
	private:
		Pin _tx, _rx;
		uint8_t _usartNumber, crc;
		uint8_t _data[3];
		int16_t _otherX, _otherY, _otherAngle;
		int16_t _xBall, _yBall;
};