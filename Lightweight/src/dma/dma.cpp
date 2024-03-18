#include "dma.h"

Dma::Dma() {
	_RCC_AHB1Periph_DMAx = 0;
}

Dma::Dma(uint32_t RCC_AHB1Periph_DMAx): _adcx() {
	_RCC_AHB1Periph_DMAx = RCC_AHB1Periph_DMAx;
}

Dma::Dma(uint32_t RCC_AHB1Periph_DMAx, Adc& adcx, DMA_Stream_TypeDef* dmax_streamx,
					uint32_t dma_channelx, uint32_t bufferSize, uint8_t cycles): _adcx(adcx) {
	_RCC_AHB1Periph_DMAx = RCC_AHB1Periph_DMAx;
	dmaInit(dmax_streamx, dma_channelx, bufferSize);
	adcInitInDma(cycles);
}

void Dma::setADC(Adc& adcx) {
	_adcx = adcx;
}

void Dma::adcInitInDma(uint8_t cycles) {
	_adcx.adcInit(cycles);
	_adcx.startADC();
	_adcx.setChannel();
	_adcx.adcDmaInit();	
}

void Dma::dmaInit(DMA_Stream_TypeDef* dmax_streamx, uint32_t dma_channelx, uint32_t bufferSize) {
	RCC_AHB1PeriphClockCmd(_RCC_AHB1Periph_DMAx, ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(dmax_streamx);
	
	DMA_InitStructure.DMA_Channel = dma_channelx;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(_adcx.getADC()->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&_adcData[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = bufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(dmax_streamx, &DMA_InitStructure);
	DMA_Cmd(dmax_streamx, ENABLE);
}

float Dma::dataReturn(uint8_t n) {
	return _adcData[n];
}