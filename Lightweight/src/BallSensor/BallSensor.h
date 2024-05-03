#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Dma.h"

#define SEE_BALL 1500
#define COUNT_THRESHOLD 2
#define TIME_IN_GRIP 500

class BallSensor {
	public:
		BallSensor(Dma& sens);
		int16_t getValue();
		bool ballInGrip();
		bool ballLongTimeInGrip();
	private:
		Dma& _sensor;
		uint8_t count2See;
		uint64_t timeInGrip;
};