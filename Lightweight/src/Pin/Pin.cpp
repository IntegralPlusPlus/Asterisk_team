#include "Pin.h"

Pin::Pin(char GPIOx, uint8_t pin, uint16_t fun) {
	setPin(pin);
	setGPIO(GPIOx);
	
	if (fun == write) {
		_modeGPIO = GPIO_Mode_OUT;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_NOPULL;
		_typeGPIO = GPIO_OType_PP;
	} else if (fun == write_pupd_down) {
		_modeGPIO = GPIO_Mode_OUT;
		_speedGPIO = GPIO_Speed_50MHz;
		_pupdGPIO = GPIO_PuPd_DOWN;
		_typeGPIO = GPIO_OType_PP;
	} else if (fun == read) {
		_modeGPIO = GPIO_Mode_IN;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_NOPULL;
		_typeGPIO = GPIO_OType_PP;
	} else if (fun == read_pupd_down) {
		_modeGPIO = GPIO_Mode_IN;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_DOWN;
		_typeGPIO = GPIO_OType_PP;
	} else if (fun == tim1 || fun == tim2 || fun == tim3 || fun == tim4 || fun == tim5 || fun == tim6 || fun == tim7 || fun == tim8 
						|| fun == tim9 || fun == tim10 || fun == tim11 || fun == tim12 || fun == tim13 || fun == tim14) {
		_modeGPIO = GPIO_Mode_AF;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_NOPULL;
		_typeGPIO = GPIO_OType_PP;
		setTimParams(fun); 
	} else if (fun == usart1 || fun == usart2 || fun == usart3 || fun == usart6) {
		_modeGPIO = GPIO_Mode_AF;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_DOWN;
		_typeGPIO = GPIO_OType_PP;
		setAFforUSART(fun);
	} else if (fun == i2c) {
		_modeGPIO = GPIO_Mode_OUT;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_UP;
		_typeGPIO = GPIO_OType_OD;
		_typeAF = GPIO_AF_I2C3;
	} else if (fun == adc) {
		_modeGPIO = GPIO_Mode_AN;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_UP;
		_typeGPIO = GPIO_OType_PP;
	} else if (fun == spi1 || fun == spi2) {
		_modeGPIO = GPIO_Mode_AF;
		_speedGPIO = GPIO_Speed_100MHz;
		_pupdGPIO = GPIO_PuPd_NOPULL;
		_typeGPIO = GPIO_OType_PP;
		
		if (fun == spi1) _typeAF = GPIO_AF_SPI1;
		else _typeAF = GPIO_AF_SPI2;
	}	
	
	if (_GPIOx == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if (_GPIOx == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if (_GPIOx == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if (_GPIOx == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if (_GPIOx == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	else if (_GPIOx == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	else if (_GPIOx == GPIOG) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	else if (_GPIOx == GPIOH) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	else if (_GPIOx == GPIOI) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
						
	GPIO_InitTypeDef init_GPIOx;
	init_GPIOx.GPIO_Pin = _pin;
	init_GPIOx.GPIO_Speed = _speedGPIO;
	init_GPIOx.GPIO_Mode = _modeGPIO;
	init_GPIOx.GPIO_PuPd = _pupdGPIO;
	init_GPIOx.GPIO_OType = _typeGPIO;
	
	GPIO_Init(_GPIOx, &init_GPIOx);
	
	if (_modeGPIO == GPIO_Mode_AF) GPIO_PinAFConfig(_GPIOx, _pinSource, _typeAF);
}

Pin::Pin() {
}

void Pin::pwmInit(int16_t period, uint32_t prescaler, uint8_t channel) {
	RCC_ClockCmd();
	
	_period = period;
	
	_timer.TIM_Prescaler = prescaler; 
	_timer.TIM_CounterMode = TIM_CounterMode_Up;
	_timer.TIM_Period = period;
	_timer.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(_TIMx, &_timer);
	TIM_Cmd(_TIMx, ENABLE);
	
	if(_TIMx == TIM1 || _TIMx == TIM8) {
		TIM_BDTRInitTypeDef _tPWMTIM1;
		_tPWMTIM1.TIM_OSSRState = TIM_OSSRState_Disable;
		_tPWMTIM1.TIM_OSSIState = TIM_OSSIState_Disable;
		_tPWMTIM1.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
		_tPWMTIM1.TIM_DeadTime = 0;
		_tPWMTIM1.TIM_Break = TIM_Break_Disable;
		_tPWMTIM1.TIM_BreakPolarity = TIM_BreakPolarity_Low;
		_tPWMTIM1.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
		TIM_BDTRConfig(_TIMx, &_tPWMTIM1);
	}
	
	_tPWM.TIM_Pulse = 0;
  _tPWM.TIM_OCMode = TIM_OCMode_PWM1;
  _tPWM.TIM_OutputState = TIM_OutputState_Enable;
  _tPWM.TIM_OCPolarity = TIM_OCPolarity_High; 
	_tPWM.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	_tPWM.TIM_OutputNState = TIM_OutputNState_Disable;
	
	if (_TIMx != TIM1 && _TIMx != TIM8) _tPWM.TIM_OCIdleState = TIM_OCIdleState_Reset;
	else _tPWM.TIM_OCIdleState = TIM_OCIdleState_Set;
	
	if (_TIMx != TIM1 && _TIMx != TIM8) _tPWM.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	else _tPWM.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	_channel = channel;
	
	if (_channel == 1) TIM_OC1Init(_TIMx, &_tPWM);
	else if (_channel == 2) TIM_OC2Init(_TIMx, &_tPWM);
	else if (_channel == 3) TIM_OC3Init(_TIMx, &_tPWM);
	else TIM_OC4Init(_TIMx, &_tPWM);
	
	TIM_Cmd(_TIMx, ENABLE);
	if(_TIMx == TIM1 || _TIMx == TIM8) TIM_CtrlPWMOutputs(_TIMx, ENABLE);
}

void Pin::pwm(uint32_t pulse) {
	switch(_channel) {
		case 1: TIM_SetCompare1(_TIMx, pulse); break;
		case 2: TIM_SetCompare2(_TIMx, pulse); break;
		case 3: TIM_SetCompare3(_TIMx, pulse); break;
		default: TIM_SetCompare4(_TIMx, pulse); break;
	}
}

bool Pin::readPin() {
	return GPIO_ReadInputData(_GPIOx) & _pin;
}

void Pin::setBit() {
	GPIO_SetBits(_GPIOx, _pin);
}

void Pin::resetBit() {
	GPIO_ResetBits(_GPIOx, _pin);
}

int16_t Pin::getPeriod() {
	return _period;
}

GPIO_TypeDef* Pin::getGPIOx() {
	return _GPIOx;
}

uint32_t Pin::getPin() {
	return _pin;
}

void Pin::set(bool bit) {
	if (bit) GPIO_SetBits(_GPIOx, _pin);
	else GPIO_ResetBits(_GPIOx, _pin);
}

void Pin::setAFforUSART(uint16_t usart) {
	if (usart == usart1) _typeAF = GPIO_AF_USART1;
	else if (usart == usart2) _typeAF = GPIO_AF_USART2;
	else if (usart == usart3) _typeAF = GPIO_AF_USART3;
	else if (usart == usart6) _typeAF = GPIO_AF_USART6;
}

void Pin::RCC_ClockCmd() {
	if(_TIMx == TIM1) RCC_APB2PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM2) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM3) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM4) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM5) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM6) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM7) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM8) RCC_APB2PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM9) RCC_APB2PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM10) RCC_APB2PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM11) RCC_APB2PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM12) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM13) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
	else if(_TIMx == TIM14) RCC_APB1PeriphClockCmd(_RCC_TIMx, ENABLE);
}

void Pin::setPin(uint8_t pinNum) {
	switch(pinNum) {
		case 0: _pin = GPIO_Pin_0; _pinSource = GPIO_PinSource0; break;
		case 1: _pin = GPIO_Pin_1; _pinSource = GPIO_PinSource1; break;
		case 2: _pin = GPIO_Pin_2; _pinSource = GPIO_PinSource2; break;
		case 3: _pin = GPIO_Pin_3; _pinSource = GPIO_PinSource3; break;
		case 4: _pin = GPIO_Pin_4; _pinSource = GPIO_PinSource4; break;
		case 5: _pin = GPIO_Pin_5; _pinSource = GPIO_PinSource5; break;
		case 6: _pin = GPIO_Pin_6; _pinSource = GPIO_PinSource6; break;
		case 7: _pin = GPIO_Pin_7; _pinSource = GPIO_PinSource7; break;
		case 8: _pin = GPIO_Pin_8; _pinSource = GPIO_PinSource8; break;
		case 9: _pin = GPIO_Pin_9; _pinSource = GPIO_PinSource9; break;
		case 10: _pin = GPIO_Pin_10; _pinSource = GPIO_PinSource10; break;
		case 11: _pin = GPIO_Pin_11; _pinSource = GPIO_PinSource11; break;
		case 12: _pin = GPIO_Pin_12; _pinSource = GPIO_PinSource12; break;
		case 13: _pin = GPIO_Pin_13; _pinSource = GPIO_PinSource13; break;
		case 14: _pin = GPIO_Pin_14; _pinSource = GPIO_PinSource14; break;
		default: _pin = GPIO_Pin_15; _pinSource = GPIO_PinSource15; break;
	}
}

void Pin::setTimParams(uint16_t tim) {
	switch(tim) {
		case tim1: _typeAF = GPIO_AF_TIM1; _RCC_TIMx = RCC_APB2ENR_TIM1EN; _TIMx = TIM1; break;
		case tim2: _typeAF = GPIO_AF_TIM2; _RCC_TIMx = RCC_APB1ENR_TIM2EN; _TIMx = TIM2; break;
		case tim3: _typeAF = GPIO_AF_TIM3; _RCC_TIMx = RCC_APB1ENR_TIM3EN; _TIMx = TIM3; break;
		case tim4: _typeAF = GPIO_AF_TIM4; _RCC_TIMx = RCC_APB1ENR_TIM4EN; _TIMx = TIM4; break;
		case tim5: _typeAF = GPIO_AF_TIM5; _RCC_TIMx = RCC_APB1ENR_TIM5EN; _TIMx = TIM5; break;
		case tim8: _typeAF = GPIO_AF_TIM8; _RCC_TIMx = RCC_APB2ENR_TIM8EN; _TIMx = TIM8; break;
		case tim9: _typeAF = GPIO_AF_TIM9; _RCC_TIMx = RCC_APB2ENR_TIM9EN; _TIMx = TIM9; break;
		case tim10: _typeAF = GPIO_AF_TIM10; _RCC_TIMx = RCC_APB2ENR_TIM10EN; _TIMx = TIM10; break;
		case tim11: _typeAF = GPIO_AF_TIM11; _RCC_TIMx = RCC_APB2ENR_TIM11EN; _TIMx = TIM11; break;
		case tim12: _typeAF = GPIO_AF_TIM12; _RCC_TIMx = RCC_APB1ENR_TIM12EN; _TIMx = TIM12; break;
		case tim13: _typeAF = GPIO_AF_TIM13; _RCC_TIMx = RCC_APB1ENR_TIM13EN; _TIMx = TIM13; break;
		default: _typeAF = GPIO_AF_TIM14; _RCC_TIMx = RCC_APB1ENR_TIM14EN; _TIMx = TIM14; break;
	}
}

void Pin::setGPIO(char gpio) {
	switch(gpio) {
		case 'A': _GPIOx = GPIOA; break;
		case 'B': _GPIOx = GPIOB; break;
		case 'C': _GPIOx = GPIOC; break;
		case 'D': _GPIOx = GPIOD; break;
		case 'E': _GPIOx = GPIOE; break;
		case 'F': _GPIOx = GPIOF; break;
		case 'G': _GPIOx = GPIOG; break;
		case 'H':  _GPIOx = GPIOH; break;
		default: _GPIOx = GPIOI; break;
	}
}
