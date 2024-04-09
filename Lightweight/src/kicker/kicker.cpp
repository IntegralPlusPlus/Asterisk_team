#include "kicker.h"

Kicker::Kicker(Pin& kickerPin, Dma& capacitorDma): _kicker(kickerPin), _capDma(capacitorDma) {
	_kickADC = 33000;//45.f * 4096.f / (3.3 * 15.f);
	_canKick = false;
	_kickerOpenTime = 0;
	signalDMASoft = 0;
}

void Kicker::setKickerStatus(bool buttonPressed) {
	_signalDMA = _capDma.dataReturn(0);
	signalDMASoft = KICKER_KSOFT_DMA * float(_signalDMA) 
									+ (1 - KICKER_KSOFT_DMA) * signalDMASoft;
	
	if (!_canKick && signalDMASoft > _kickADC && 
			buttonPressed && time_service::millis() - _kickerOpenTime > KICK_CLOSE_TIME) {
		_canKick = true;
		_kickerOpenTime = time_service::millis();
	} else if (_canKick && time_service::millis() - _kickerOpenTime > KICK_OPEN_TIME) {
		_canKick = false;
		_kickerOpenTime = time_service::millis();
	}
}

int16_t Kicker::getDMASignal() {
	return signalDMASoft;
}

bool Kicker::canKick() {
	return _canKick;
}

void Kicker::open() {
	_kicker.setBit();
}

void Kicker::close() {
	_kicker.resetBit();
}