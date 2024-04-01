#include "Robot.h"

int main() {
	Asterisk::init(YELLOW_GOAL, FORWARD_ROLE, P_MODE);
	
	while (true) {
		Asterisk::update();

		if (Asterisk::calibrated()) {
			if (Asterisk::getRole() == FORWARD_ROLE) {
				Asterisk::forwardStrategy();
			} else if (Asterisk::getRole() == GOALKEEPER_ROLE) {
				Asterisk::goalkeeperStrategy();
			}
		}
	}
	
	return 0;
}