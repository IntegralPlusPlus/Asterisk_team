#include "Robot.h"

int main() {
	Asterisk::init(BLUE_GOAL, FORWARD_ROLE, P_MODE);
	
	while (true) {
		Asterisk::update();
		//Asterisk::forwardStrategy();
		if (Asterisk::calibrated()) {
			if (Asterisk::getRole() == FORWARD_ROLE) {
				Asterisk::forwardStrategy();
			} else if (Asterisk::getRole() == GOALKEEPER_ROLE) {
				//Asterisk::protectGoal();
			}
		}
	}
	
	return 0;
}