#pragma once
#include "project_config.h"
#include "Vec2b.h"

#define ENEMY_X 0
#define ENEMY_Y 231
#define DELTA_DIST 5
#define THRESHOLD_X 79
#define UP_Y 225
#define DOWN_Y 27.f

#define DOWN_Y_GOALKEEPER_RIGHT 16
#define DOWN_Y_GOALKEEPER_LEFT 18
#define GOAL_OUT_X_THRESHOLD_RIGHT 27
#define GOAL_OUT_X_THRESHOLD_LEFT -23
#define GOAL_OUT_Y_THRESHOLD 24
#define GOAL_CIRCLE_Y_THRESHOLD_RIGHT 35.f
#define GOAL_CIRCLE_Y_THRESHOLD_LEFT 34.f
#define GOAL_OUT_X_THRESHOLD_ALL 45
#define DIST_BETWEEN_GOALS 231

#define ANGLE_LOW_TO_CIRCLE 60
#define ANGLE_HIGH_TO_CIRCLE 137
#define RADIUS_GOAL_OUT_RIGHT 35
#define RADIUS_GOAL_OUT_LEFT 44
//44.7f
#define MAX_COEFF_TO_GOAL_CENTER 1.05f
#define MAX_LEN_TO_INCREASE_VEC 0.9f
#define GK_X_THRESHOLD_LEFT -37
#define GK_X_THRESHOLD_RIGHT 36

#define BACK_ANGLE 80

class ProcessingCoord {
	public:
		ProcessingCoord();
		void setGoal(uint8_t currentGoal);
		void setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow);
		Vec2b checkOUTs(Vec2b current);
		Vec2b getVecForMyCircle(int16_t x, int16_t y);
		Vec2b getVecForEnemyCircle(int16_t x, int16_t y);
		Vec2b getVecToGoalCenter();
		Vec2b getVecToIntersection(int16_t angBall);
		Vec2b getVecToPoint(int16_t pointX = 0, int16_t pointY = GOAL_OUT_Y_THRESHOLD);
		int16_t adduct(int16_t value);
		int16_t adduct180(int16_t value);
		int16_t getTargetForward();
		int16_t getTargetGoalkeeper();
		bool isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool checkXLeft(int16_t x);
		bool checkXRight(int16_t x);
		bool checkYUp(int16_t y);
		bool checkYDown(int16_t y);
		bool myGoalLine(int16_t x, int16_t y);
		bool enemyGoalLine(int16_t x, int16_t y);
		bool robotInOUT();
		bool robotInCritical();
		float getCoeffToGoalCenter(float intersec);
		float map(float a, float from1, float to1, float from2, float to2);
	private:
		Vec2b _leftFast, _rightFast;
		Vec2b _upFast, _downFast;
		float distToGoalCenter;
		float errOldGkLeft, errOldGkLine, errOldGkRight; 
		int16_t _targetIMU;
		int16_t _x, _y, _angle;
		int16_t _dBlue, _dYellow;
		uint8_t _goal;
		bool inOUT;
		bool critical;
};