#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"
#include "Dma.h"

#define KICK_OPEN_TIME 700
#define KICK_CLOSE_TIME 2000
#define KICKER_KSOFT_DMA 0.008f

class Kicker {
	public:
		Kicker(Pin& kickerPin, Dma& capacitorDma);
		void setKickerStatus(bool buttonPressed = true);
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
		float _kickADC;
		float signalDMASoft;
};
