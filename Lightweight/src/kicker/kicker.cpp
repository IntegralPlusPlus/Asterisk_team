#include "kicker.h"

Kicker::Kicker(Pin& kickerPin, Dma& capacitorDma): _kicker(kickerPin), _capDma(capacitorDma) {
	_canKick = false;
	_kickerOpenTime = 0;
	_signalDMASoft = 0;
}

void Kicker::setKickerStatus(bool condition) {
	_signalDMA = _capDma.dataReturn(0);
	_signalDMASoft = KICKER_KSOFT_DMA * float(_signalDMA) + (1 - KICKER_KSOFT_DMA) * _signalDMASoft;
	
	if (!_canKick && _signalDMASoft > KICK_DMA && 
			condition && time_service::millis() - _kickerOpenTime > KICK_CLOSE_TIME) {
		_canKick = true;
		_kickerOpenTime = time_service::millis();
	} else if (_canKick && time_service::millis() - _kickerOpenTime > KICK_OPEN_TIME) {
		_canKick = false;
		_kickerOpenTime = time_service::millis();
	}
}

int16_t Kicker::getDMASignal() {
	return _signalDMASoft;
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