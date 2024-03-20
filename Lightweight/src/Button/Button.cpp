#include "Button.h"

Button::Button(Pin myButton): _button(myButton) {
	buttonOld = false;
	press = false;
	startTime = false;
}

bool Button::pressed() {
	bool readPin = _button.readPin();
	if (!startTime && readPin && !buttonOld) {
		time = time_service::millis();
		startTime = true;
		checkPin = readPin;
	} else if (startTime) {
		if (time_service::millis() - time < BUTTON_DELTA && readPin != checkPin) startTime = false;
		else if (time_service::millis() - time >= BUTTON_DELTA) {
			press = !press;
			startTime = false;
		}
	}
		
	buttonOld = readPin;
	
	return press;
}