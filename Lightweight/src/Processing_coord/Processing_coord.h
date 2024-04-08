#pragma once
#include "project_config.h"
#include "Vec2b.h"

#define ENEMY_X 0
#define ENEMY_Y 231
#define THRESHOLD_X_RIGHT 54
//54
#define THRESHOLD_X_LEFT -72
#define UP_Y 212.f
#define DOWN_Y 14.f

#define GOAL_OUT_Y_THRESHOLD 34
#define DIST_BETWEEN_GOALS 231

#define ANGLE_LOW_TO_CIRCLE 50
#define ANGLE_HIGH_TO_CIRCLE 125
#define RADIUS_GOAL_OUT_RIGHT 43
//43
#define RADIUS_GOAL_OUT_LEFT 44
//44

#define BACK_ANGLE 60

class ProcessingCoord {
	public:
		ProcessingCoord();
		void setGoal(uint8_t currentGoal);
		void setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow);
		void setMaxLen(float len);
		Vec2b getVecToPoint(int16_t pointX = 0, int16_t pointY = GOAL_OUT_Y_THRESHOLD);
		float distance(float x, float y, float startX = 0, float startY = 0);
		float map(float a, float from1, float to1, float from2, float to2);
		float getAngleBetween(float ang1, float ang2);
		int16_t adduct(int16_t value);
		int16_t adduct180(int16_t value);
	protected:
		float _maxLen;
		int16_t _x, _y, _angle;
		int16_t _dBlue, _dYellow;
		uint8_t _goal;
};