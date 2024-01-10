#include "project_config.h"
#include "Robot.h"

int main() {
	Robot::init(BLUE_GOAL, FORWARD_ROLE);
	
	while (true) {
		Robot::updateSensors();
		
		if (Robot::calibrated()) {
			if (Robot::getRole() == FORWARD_ROLE) {
				Robot::goToBall();
			} else if (Robot::getRole() == GOALKEEPER_ROLE) {
				Robot::keepGoal();
			}
		}
	}
	
	return 0;
}