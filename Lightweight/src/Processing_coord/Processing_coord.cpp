#include "Processing_coord.h"

ProcessingCoord::ProcessingCoord(uint8_t currentGoal) {
	_goal = currentGoal;
	targetIMU = 0;
	_x = 0;
	_y = 0;
}

void ProcessingCoord::set(int16_t x, int16_t y) {
	_x = x;
	_y = y;
}

int16_t ProcessingCoord::getIMUTarget() {
	targetIMU = RAD2DEG * atan2(float(_x - ENEMY_X), float(_y - ENEMY_Y));
	return targetIMU;
}