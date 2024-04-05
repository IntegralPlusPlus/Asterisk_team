#include "adc.h"

Adc::Adc(ADC_TypeDef* ADCx, uint8_t numCh, uint8_t chNow) {
	_ADCx = ADCx;
	_numCh = numCh;
	_chNow = chNow;
	_RCC_APB2Periph_ADCx = getRCCPeriph(ADCx);
	current = 1;
	
	channel[current] = chNow;
  current++;
}

Adc::Adc() {
}

void Adc::adcInit(uint8_t cycles) {
	RCC_APB2PeriphClockCmd(_RCC_APB2Periph_ADCx, ENABLE);
	ADC_CommonInitTypeDef ADC_common;
  
	ADC_common.ADC_Mode = ADC_Mode_Independent;
  ADC_common.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_common.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_common.ADC_TwoSamplingDelay = getCycles(cycles);
	ADC_CommonInit(&ADC_common);
}

void Adc::startADC() {
		ADC_InitTypeDef ADC_InitStructure;
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //12b
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfConversion = _numCh;
		
		ADC_Init(_ADCx, &ADC_InitStructure);
		ADC_Cmd(_ADCx, ENABLE);
}

float Adc::read() {
	ADC_RegularChannelConfig(_ADCx, _chNow, 1, ADC_SampleTime_3Cycles);
  
	//start working
  ADC_SoftwareStartConv(_ADCx);
  while(ADC_GetFlagStatus(_ADCx, ADC_FLAG_EOC) == RESET);
	
  return ADC_GetConversionValue(_ADCx);
}

void Adc::sendMeCh(uint8_t ch) {
    channel[current] = ch;
    current++;
}

void Adc::setChannel()
{
    for(uint8_t i = 1; i <= _numCh; i++) ADC_RegularChannelConfig(_ADCx, channel[i], i, ADC_SampleTime_56Cycles);
}

uint32_t Adc::getRCCPeriph(ADC_TypeDef* ADCx) {
	if (_ADCx == ADC1) return RCC_APB2Periph_ADC1;
	else if (_ADCx == ADC2) return RCC_APB2Periph_ADC2;
	else return RCC_APB2Periph_ADC3;
}

uint32_t Adc::getCycles(uint8_t cycles) {
	switch (cycles) {
    case 10: return ADC_TwoSamplingDelay_10Cycles;
    case 15: return ADC_TwoSamplingDelay_15Cycles;
    case 20: return ADC_TwoSamplingDelay_20Cycles;
    default: return ADC_TwoSamplingDelay_5Cycles;
  }
}

ADC_TypeDef* Adc::getADC() {
	return _ADCx;
}

void Adc::adcDmaInit() {
	ADC_DMARequestAfterLastTransferCmd(_ADCx, ENABLE);
	ADC_EOCOnEachRegularChannelCmd(_ADCx, ENABLE);
	ADC_DMACmd(_ADCx, ENABLE);
	ADC_SoftwareStartConv(_ADCx);
}