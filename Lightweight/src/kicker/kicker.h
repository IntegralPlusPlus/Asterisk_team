#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"
#include "Dma.h"

#define KICK_DMA 2600
#define KICK_OPEN_TIME 700
#define KICK_CLOSE_TIME 1500
#define KICKER_KSOFT_DMA 0.008f

class Kicker {
	public:
		Kicker(Pin& kickerPin, Dma& capacitorDma);
		void setKickerStatus(bool condition = true);
		void open();
		void close();
		int16_t getDMASignal();
		bool canKick();
	private:
		Pin& _kicker;
		Dma& _capDma;
		int16_t _signalDMA;
		bool _canKick;
		uint64_t _kickerOpenTime;
		float _signalDMASoft;
};
