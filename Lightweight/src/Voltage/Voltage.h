#pragma once
#include "project_config.h"
#include "Pin.h"
#include "Dma.h"

#define MINIMUM_VOLTAGE 9.f
#define KOEFF_VOLTAGE 0.0041f
#define KSOFT_VOLTAGE 0.2f

class VoltageDividor {
	public:
		VoltageDividor();
		//VoltageDividor(Dma& dividor);
		void setADC(Adc& dividor);
		float getVoltage();
		bool voltageLow(float voltage);
	private:
		Adc _volADC;
		float _voltage;
};