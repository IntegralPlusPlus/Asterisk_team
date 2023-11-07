#ifndef PIN_H_
#define PIN_H_
#include "stm32f4xx.h"

enum Pin_type {
	read,
	write,
	write_pupd_down,
	tim1,
	tim2,
	tim3,
	tim4,
	tim5,
	tim6,
	tim7,
	tim8,
	tim9,
	tim10,
	tim11,
	tim12,
	tim13,
	tim14,
	usart2,
	adc,
	spi1,
	spi2
};

class Pin {
	public:
		Pin(char GPIOx, uint8_t pin, uint16_t fun = write);
		void setPin(uint8_t pinNum);
		void setGPIO(char gpio);
		void setTimParams(uint16_t tim);
		void pwmInit(int16_t period, uint32_t prescaler, uint8_t channel);
		void RCC_ClockCmd();
	  void pwm(uint32_t pulse);
		void setBit();
		void resetBit(); 
		int16_t getPeriod();
	private:
		GPIO_TypeDef* _GPIOx;
		uint32_t _pin;
		uint8_t _typeAF;
		uint8_t _pinSource;
	
		GPIOMode_TypeDef _modeGPIO;
		GPIOSpeed_TypeDef _speedGPIO;
		GPIOOType_TypeDef _typeGPIO;
		GPIOPuPd_TypeDef _pupdGPIO;	
	
		TIM_TimeBaseInitTypeDef _timer;
		TIM_OCInitTypeDef _tPWM;
		TIM_TypeDef* _TIMx;
		uint8_t _channel;
		uint32_t _RCC_TIMx; 
		int16_t _period;
};

#endif /* PIN_H_ */