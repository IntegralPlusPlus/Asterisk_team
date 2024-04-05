#include "Voltage.h"

VoltageDividor::VoltageDividor(Dma& dividor): _volDMA(dividor) {
	_voltage = 12.6f;
}

float VoltageDividor::getVoltage() {
	float currVoltage = _volDMA.dataReturn(0) * KOEFF_VOLTAGE;
	_voltage = KSOFT_VOLTAGE * currVoltage + (1 - KSOFT_VOLTAGE) * _voltage;
	
	return _voltage;
}

bool VoltageDividor::voltageLow(float voltage) {
	return voltage < MINIMUM_VOLTAGE;
}