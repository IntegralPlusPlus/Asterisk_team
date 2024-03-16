#pragma once
#include "project_config.h"
#include "Pin.h"

class Adc {
	public:
		Adc();
		Adc(ADC_TypeDef* ADCx, uint8_t numCh, uint8_t chNow);
		uint32_t getRCCPeriph(ADC_TypeDef* ADCx);
		void sendMeCh(uint8_t ch);
		uint32_t getCycles(uint8_t cycles);
		void adcInit(uint8_t cycles);
		void adcDmaInit();
		void startADC(); 
		float read();
		ADC_TypeDef* getADC();
		void setChannel(); 
	private:
		ADC_TypeDef* _ADCx;
  	uint8_t _numCh;
		uint8_t _chNow;
		uint32_t _RCC_APB2Periph_ADCx;
		uint8_t channel[50];
		uint8_t current;
};