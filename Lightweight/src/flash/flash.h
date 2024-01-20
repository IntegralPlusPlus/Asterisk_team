#pragma once
#include "project_config.h"

#define STANDART_PAGE_WRITING_ADDRESS 0x080E0000

void writeFLASH(uint8_t _data) {
  FLASH_Unlock();
  FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
  FLASH_ProgramWord(STANDART_PAGE_WRITING_ADDRESS, _data);
  FLASH_Lock();
}

uint32_t readFLASH() {
  FLASH_Unlock();
  return *(__IO uint32_t*)STANDART_PAGE_WRITING_ADDRESS;
  FLASH_Lock();
}