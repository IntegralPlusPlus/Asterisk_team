#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Pin.h"

#define BUTTON_DELTA 60

enum buttonConfig {
	defaultConfig,
	invertedConfig,
	switcherConfig
};

class Button {
	public:
		Button(Pin myButton, bool inverted = defaultConfig);
		bool pressed();
	private:
		Pin _button;
		uint64_t time;
		uint8_t _config;
		bool buttonOld, press, checkPin;
		bool startTime;
};