#pragma once
#include "project_config.h"
#include "Vec2b.h"

#define ENEMY_X 0
#define ENEMY_Y 231
#define DELTA_DIST 8
#define THRESHOLD_X_RIGHT 58
#define THRESHOLD_X_LEFT -68
#define UP_Y 212.f
#define DOWN_Y 11.f

#define GOAL_OUT_X_THRESHOLD_RIGHT 27
#define GOAL_OUT_X_THRESHOLD_LEFT -23
#define GOAL_OUT_Y_THRESHOLD 30
#define GOAL_CIRCLE_Y_THRESHOLD_RIGHT 35.f
#define GOAL_CIRCLE_Y_THRESHOLD_LEFT 34.f
#define GOAL_OUT_X_THRESHOLD_ALL 45
#define DIST_BETWEEN_GOALS 231

#define ANGLE_LOW_TO_CIRCLE 60
#define ANGLE_HIGH_TO_CIRCLE 125
#define RADIUS_GOAL_OUT_RIGHT 35
#define RADIUS_GOAL_OUT_LEFT 39

#define MAX_COEFF_TO_GOAL_CENTER 1.075f
#define MAX_LEN_TO_INCREASE_VEC 0.88f
#define GK_X_THRESHOLD_LEFT -38
#define GK_X_THRESHOLD_RIGHT 36

#define BACK_ANGLE 60
#define LIMIT_OUT_CYCLES 20

class ProcessingCoord {
	public:
		ProcessingCoord();
		void setGoal(uint8_t currentGoal);
		void setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow);
		Vec2b getVecToPoint(int16_t pointX, int16_t pointY);
		void setMaxLen(float len);
		int16_t adduct(int16_t value);
		int16_t adduct180(int16_t value);
		float distance(float x, float y, float startX = 0, float startY = 0);
		float map(float a, float from1, float to1, float from2, float to2);
	protected:
		float _maxLen;
		int16_t _x, _y, _angle;
		int16_t _dBlue, _dYellow;
		int16_t DOWN_Y_GOALKEEPER_RIGHT, DOWN_Y_GOALKEEPER_LEFT;
		uint8_t _goal;
};