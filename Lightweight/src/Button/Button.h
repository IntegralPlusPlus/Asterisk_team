#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"

#define BUTTON_DELTA 70

class Button {
	public:
		Button(Pin myButton);
		bool pressed();
	private:
		Pin _button;
		bool buttonOld, press, checkPin;
		bool startTime;
		uint64_t time;
};