#include "usart6.h"

extern "C" {
	void USART6_IRQHandler(void) {
		uint8_t reg = USART6->SR; 	
		if (reg & USART_SR_RXNE) { //GET DATA
			uart6::rx[uart6::_rxCnt] = USART6->DR;
			uart6::_rxCnt++;
			if(uart6::_rxCnt == 8) uart6::_rxCnt = 0;
		} 
		
		if(reg & USART_SR_TC) {
      USART_ClearITPendingBit(USART6, USART_IT_TC);
      if (uart6::_sendCnt != 0) {
				(USART6->DR) = uart6::tx[uart6::_txCnt];
        uart6::_txCnt++;
				if(uart6::_txCnt == 8) uart6::_txCnt = 0;
      } else uart6::flag = true;
    }
		
    if(USART6->SR & USART_SR_ORE) {
       uint8_t a = USART6->DR;
       (void)a;
    }
	} 
}

namespace uart6
{
	volatile uint8_t rx[8];
	volatile uint16_t _rxCnt;
	volatile uint16_t _txCnt;
	volatile uint8_t  tx[8];
	volatile uint16_t _sendCnt;
	volatile uint16_t _readCnt;
	volatile bool flag;
	
	void usart6Init(uint32_t bods, float stopBits, uint8_t len) {
		_txCnt = 0;
		_rxCnt = 0;
		_readCnt = 0;
		_sendCnt = 0;
    rx[0] = 0;  
		flag = true;
		
		//======================USART_CONFIG===========================
		USART_InitTypeDef USART_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
  
		USART_StructInit(&USART_InitStructure);
		USART_InitStructure.USART_BaudRate = bods;
		
		if (len == 9) USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		else USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		
		if (stopBits == 0.5) USART_InitStructure.USART_StopBits = USART_StopBits_0_5; 
		else if (stopBits == 1) USART_InitStructure.USART_StopBits = USART_StopBits_1;
		else if (stopBits == 1.5) USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
		else USART_InitStructure.USART_StopBits = USART_StopBits_2;
		
		USART_InitStructure.USART_Parity = USART_Parity_No; 
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;       // ?ac?aoaai i?eai e ia?aaa?o
        
		USART_Init(USART6, &USART_InitStructure); //eieceaeece?oai
		
		USART_ITConfig(USART6, USART_IT_TC, ENABLE);
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART6, ENABLE);
		
		NVIC_SetPriority(USART6_IRQn, 0);
		NVIC_EnableIRQ(USART6_IRQn);
	}
	
	uint16_t read() {  
		uint16_t dt;
		ENTER_CRITICAL_SECTION();
		dt = rx[_readCnt];
		_readCnt++;
		if(_readCnt == 8) _readCnt = 0;
		EXIT_CRITICAL_SECTION();
		
		return dt;
	}
	
	void write(uint8_t _w)
	{  
		ENTER_CRITICAL_SECTION();
		
		if (!flag) {
			tx[_sendCnt] = _w;
		  _sendCnt++;
			if(_sendCnt == 8) _sendCnt = 0;
		} else {
		 flag = 0;
		 (USART6->DR) = _w;
		}
		
		EXIT_CRITICAL_SECTION();
	}

	uint16_t available() {  
		uint16_t s;
		ENTER_CRITICAL_SECTION();
		s = _rxCnt - _readCnt;
		EXIT_CRITICAL_SECTION();
	
		return s;
	}  
}