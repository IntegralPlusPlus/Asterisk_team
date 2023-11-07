#pragma once
#include "project_config.h"
#include "stdint.h"

namespace time_service {
	extern volatile uint64_t tim;
	
	uint64_t millis();
	void startTime();
	void stopTime();
	void init();
	void delay(uint32_t delta);
}