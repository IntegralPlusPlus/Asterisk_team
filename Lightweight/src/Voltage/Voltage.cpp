#include "Voltage.h"

VoltageDividor::VoltageDividor() {
	_voltage = 12.6f;
}

/*VoltageDividor::VoltageDividor(Dma& dividor): _volDMA(dividor) {
	_voltage = 12.6f;
}*/

void VoltageDividor::setADC(Adc& dividor) {
	_volADC = dividor;
}

float VoltageDividor::getVoltage() {
	float currVoltage = _volADC.read() * KOEFF_VOLTAGE;
	_voltage = currVoltage;//_voltage;//KSOFT_VOLTAGE * _voltage + (1.f - KSOFT_VOLTAGE) * currVoltage;
	
	return _voltage;
}

bool VoltageDividor::voltageLow(float voltage) {
	return voltage < MINIMUM_VOLTAGE;
}