#pragma once
#include "project_config.h"
#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"

#define RAD2DEG	57.2957795130823208767f
#define DEG2RAD	0.01745329251994329576f
#define RECEIVED2REAL 1.4117647058823529411764705882353f
#define DATA_SIZE 3
#define PACKAGE_SIZE 5

enum convertData {
	ANGLE_360_TO_255,
	ANGLE_255_TO_360,
	UINT8T_TO_INT16T,
	INT16T_TO_UINT8T
};

class Bluetooth {
	public:
		Bluetooth(Pin& tx, Pin& rx, uint8_t usartNum);
		void initUSART(uint8_t num);
		void read();
		void send(int16_t myX, int16_t myY, int16_t myAngle);
		int16_t convert(int16_t data, uint8_t type);
		void ballCalculate(int16_t myX, int16_t myY, int16_t myAngle);
		uint8_t crc8(volatile uint8_t* data, uint8_t len);
		int16_t getXBall();
		int16_t getYBall();
	private:
		Pin _tx, _rx;
		uint8_t _usartNumber, crc;
		uint8_t _data[3];
		int16_t _otherX, _otherY, _otherAngle;
		int16_t _xBall, _yBall;
};