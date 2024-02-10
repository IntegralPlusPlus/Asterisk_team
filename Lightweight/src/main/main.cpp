#include "Robot.h"

int main() {
	Asterisk::init(YELLOW_GOAL, GOALKEEPER_ROLE, PLAY_MODE);
	
	while (true) {
		Asterisk::update();
		
		if (Asterisk::calibrated()) {
			if (Asterisk::getRole() == FORWARD_ROLE) {
				Asterisk::goToBall();
			} else if (Asterisk::getRole() == GOALKEEPER_ROLE) {
				Asterisk::protectGoal();
			}
		}
	}
	
	return 0;
}