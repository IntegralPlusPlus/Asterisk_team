#pragma once
#include "time_service.h"

extern "C" {
	void SysTick_Handler (void) {
		time_service::tim++;
	}
}

namespace time_service {
	volatile uint64_t tim = 0;
	
	uint64_t millis(void) {
		return tim;
	}
	
	void startTime(void) {
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}
	
	void stopTime(void) {
		SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
	}
	
	void init(void) {
		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);
		SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	}
	
	void delay(uint32_t delta) {
		uint32_t now = tim;
		while (tim - now < delta);
	}
}