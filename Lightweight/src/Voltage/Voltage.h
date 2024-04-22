#pragma once
#include "project_config.h"
#include "Pin.h"
#include "Dma.h"

#define MINIMUM_VOLTAGE 10.f
#define KOEFF_VOLTAGE 0.00374f
#define KSOFT_VOLTAGE 0.01f

class VoltageDividor {
	public:
		VoltageDividor(Dma& dividor);
		float getVoltage();
		bool voltageLow(float voltage);
	private:
		Dma& _volDMA;
		float _voltage;
};