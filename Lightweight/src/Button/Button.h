#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"

#define BUTTON_DELTA 60

enum buttonConfig {
	defaultConfig,
	invertedConfig
};

class Button {
	public:
		Button(Pin myButton, bool inverted = defaultConfig);
		bool pressed();
	private:
		Pin _button;
		bool _inverted;
		bool buttonOld, press, checkPin;
		bool startTime;
		uint64_t time;
};