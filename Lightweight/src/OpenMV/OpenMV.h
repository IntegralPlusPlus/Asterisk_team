#pragma once
#include "project_config.h"
#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"

#define RAD2DEG	57.2957795130823208767f
#define DEG2RAD	0.017453292519943295769f
#define DIST_BETWEEN_GOALS 231
//231

class OpenMV {
	public:
		OpenMV(Pin &tx, Pin &rx, uint8_t usartNum);
		void read();
		void calculate(int16_t currentAngle, bool goal);
		void initUSART(uint8_t num);
		int16_t adductionMV(int16_t angleIMU);
		uint8_t crc8(volatile uint8_t* data, uint8_t len);
		int16_t getX();
		int16_t getY();
		int16_t getDistYellow();
		int16_t getDistBlue();
	//private:
		int16_t _x, _y;
		volatile int16_t _distBlue, _distYellow;
		volatile uint16_t _angleBlue, _angleYellow;
		uint8_t _usartNumber, crc;
		int16_t uartAvailable;
		uint8_t data[4];
};