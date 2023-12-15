#pragma once
#include "project_config.h"
#include "Pin.h"

class Motor {
	public:
		Motor(Pin& in1, uint8_t channelIN1, Pin& in2, uint8_t channelIN2);
		Motor(Pin& in1, Pin& in2);
		void go(int16_t speed);
		void disable();
		int16_t getMaxSpeed(); 
		//void go(uint32_t vel, uint32_t vel);
	private:
		Pin _in1;
		Pin _in2;
};