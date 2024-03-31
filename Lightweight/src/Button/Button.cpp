#include "Button.h"

Button::Button(Pin myButton, bool inverted): _button(myButton) {
	buttonOld = false;
	press = false;
	startTime = false;
	_inverted = inverted;
}

bool Button::pressed() {
	bool readPin = _button.readPin();
	if (!startTime && (!_inverted && readPin && !buttonOld || _inverted && !readPin && buttonOld)) {
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