#include "project_config.h"

#define ENEMY_X 0
#define ENEMY_Y 231

#define RAD2DEG	57.2957795130823208767f

class ProcessingCoord {
	ProcessingCoord(uint8_t currentGoal);
	void set(int16_t x, int16_t y);
	int16_t getIMUTarget();
	
	int16_t targetIMU;
	int16_t _x, _y;
	uint8_t _goal;
};