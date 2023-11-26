#include "project_config.h"

void initSPI2(bool mode, uint8_t dataFrameFormat, uint16_t clkPrescaler);
uint16_t writeSPI(uint16_t TxData);								//for master
void writeTxBufSPI(uint16_t TxData);						//for slave
uint16_t readRxBufSPI();								//for slave
bool SPIAvailable();