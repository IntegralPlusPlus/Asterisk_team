#include "Robot.h"

int main() {
	Asterisk::init(YELLOW_GOAL, FORWARD_ROLE);
	volatile uint64_t kek = 0;
	
	while (true) {
		kek++;
		Asterisk::update();

		if (!Asterisk::calibrated()) continue;
		if (Asterisk::getRole() == FORWARD_ROLE) Asterisk::forwardStrategy();
		else if (Asterisk::getRole() == GOALKEEPER_ROLE) Asterisk::goalkeeperStrategy();
	}
	
	return 0;
}