#include "project_config.h"
#include "Robot.h"

int main() {
	Robot::init(YELLOW_GOAL);
	
	while (true) {
		Robot::updateSensors();
		
		if (Robot::calibrated()) {
			Robot::goToBall();
		}
	}
	
	return 0;
}