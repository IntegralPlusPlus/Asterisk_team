#include "Robot.h"

int main() {
	volatile float volt1, volt2;
	Asterisk::init(BLUE_GOAL, FORWARD_ROLE, P_MODE);
	
	while (true) {
		Asterisk::update();

		/*if (Asterisk::calibrated()) {
			if (Asterisk::getRole() == FORWARD_ROLE) {
				Asterisk::goToBall();
			} else if (Asterisk::getRole() == GOALKEEPER_ROLE) {
				Asterisk::protectGoal();
			}
		}*/
	}
	
	return 0;
}