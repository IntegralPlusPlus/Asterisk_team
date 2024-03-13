#include "kicker.h"

Kicker::Kicker(Pin& kickerPin, Dma& capacitorDma): _kicker(kickerPin), _capDma(capacitorDma) {
	_kickerOpen = false;
	_kickerOpenTime = 0;
}

void Kicker::setVoltage() {
	_voltage = _capDma.dataReturn(0);
	if (!_kickerOpen && _voltage > KICK_VOLTAGE) {
		_kickerOpen = true;
		_kickerOpenTime = time_service::millis();
	} else if (_kickerOpen && time_service::millis() - _kickerOpenTime > KICK_OPEN_TIME) {
		_kickerOpen = false;
	}
}

bool Kicker::canKick() {
	return _kickerOpen;
}

void Kicker::openKicker() {
	_kicker.setBit();
}

void Kicker::closeKicker() {
	_kicker.resetBit();
}