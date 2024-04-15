#include "Robot.h"

int main() {
	Asterisk::init(YELLOW_GOAL, GOALKEEPER_ROLE);
	
	while (true) {
		Asterisk::update();

		if (!Asterisk::calibrated()) continue;
		if (Asterisk::getRole() == FORWARD_ROLE) Asterisk::forwardStrategy();
		else if (Asterisk::getRole() == GOALKEEPER_ROLE) Asterisk::goalkeeperStrategy();
	}
	
	return 0;
}