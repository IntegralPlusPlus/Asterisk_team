#pragma once
#include "project_config.h"
#include "Pin.h"

class Button {
	public:
		Button(Pin myButton);
		bool pressed();
	private:
		Pin _butt;
		bool buttOld, press;
};