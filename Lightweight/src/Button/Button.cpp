#include "Button.h"

Button::Button(Pin myButton): _butt(myButton) {
	buttOld = 0;
	press = 0;
}

bool Button::pressed() {
	if (_butt.readPin() && !buttOld) press = !press;
	buttOld = _butt.readPin();
	
	return press;
}