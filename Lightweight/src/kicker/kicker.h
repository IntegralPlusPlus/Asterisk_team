#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"
#include "Dma.h"

#define KICK_VOLTAGE 990
#define KICK_OPEN_TIME 1000
#define KOEFF_CAPACITOR 15

class Kicker {
	public:
		Kicker(Pin& kickerPin, Dma& capacitorDma);
		void setVoltage();
		void openKicker();
		void closeKicker();
		bool canKick();
	private:
		Pin _kicker;
		Dma _capDma;
		int16_t _voltage;
		bool _kickerOpen;
		uint64_t _kickerOpenTime;
};
