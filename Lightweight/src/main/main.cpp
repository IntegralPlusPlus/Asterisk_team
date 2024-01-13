#include "Robot.h"

int main() {
		Robot::init(BLUE_GOAL, GOALKEEPER_ROLE);
	
	while (true) {
		Robot::update();
		
		if (Robot::calibrated()) {
			if (Robot::getRole() == FORWARD_ROLE) {
				Robot::goToBall();
			} else if (Robot::getRole() == GOALKEEPER_ROLE) {
				Robot::protectGoal();
			}
		}
	}
	
	return 0;
}