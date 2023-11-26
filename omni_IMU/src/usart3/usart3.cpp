#include "usart3.h"

extern "C" {
	void USART3_IRQHandler(void) {
		uint8_t reg = USART3->SR; 	
		if (reg & USART_SR_RXNE) { //GET DATA
			uart3::rx[uart3::_rxCnt] = USART3->DR;
			uart3::_rxCnt++;
			if(uart3::_rxCnt == 8) uart3::_rxCnt = 0;
		} 
		
		if(reg & USART_SR_TC) {
      USART_ClearITPendingBit(USART3, USART_IT_TC);
      if (uart3::_sendCnt != 0) {
				(USART3->DR) = uart3::tx[uart3::_txCnt];
        uart3::_txCnt++;
				if(uart3::_txCnt == 8) uart3::_txCnt = 0;
      } else uart3::flag = true;
    }
		
    if(USART3->SR & USART_SR_ORE) {
       uint8_t a = USART3->DR;
       (void)a;
    }
	} 
}

namespace uart3
{
	volatile uint8_t rx[8];
	volatile uint16_t _rxCnt;
	volatile uint16_t _txCnt;
	volatile uint8_t  tx[8];
	volatile uint16_t _sendCnt;
	volatile uint16_t _readCnt;
	volatile bool flag;
	
	void usart3Init(uint32_t bods, float stopBits, uint8_t len) {
		_txCnt = 0;
		_rxCnt = 0;
		_readCnt = 0;
		_sendCnt = 0;
    rx[0] = 0;  
		flag = true;
		
		//======================USART_CONFIG===========================
		USART_InitTypeDef USART_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
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
        
		USART_Init(USART3, &USART_InitStructure); //eieceaeece?oai
		
		USART_ITConfig(USART3, USART_IT_TC, ENABLE);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART3, ENABLE);
		
		NVIC_SetPriority(USART3_IRQn, 0);
		NVIC_EnableIRQ(USART3_IRQn);
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
		 (USART3->DR) = _w;
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