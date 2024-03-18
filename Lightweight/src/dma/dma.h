#pragma once
#include "project_config.h"
#include "adc.h"

class Dma {
	public:
		Dma();
		Dma(uint32_t RCC_AHB1Periph_DMAx);
		Dma(uint32_t RCC_AHB1Periph_DMAx, Adc& adcx, DMA_Stream_TypeDef* dmax_streamx, 
				uint32_t dma_channelx, uint32_t bufferSize = 5, uint8_t cycles = 5);
		void dmaInit(DMA_Stream_TypeDef* dmax_streamx, uint32_t dma_channelx, uint32_t bufferSize);
		void adcInitInDma(uint8_t cycles); 
		void setADC(Adc& adcx);
		float dataReturn(uint8_t n);
	private:
		uint32_t _RCC_AHB1Periph_DMAx;
		uint16_t _adcData[5];
		Adc _adcx;
};