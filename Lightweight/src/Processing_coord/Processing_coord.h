#pragma once
#include "project_config.h"
#include "Vec2b.h"

#define ENEMY_X 0
#define ENEMY_Y 231
#define DELTA_DIST 5
#define THRESHOLD_X 79
#define UP_Y 225
#define DOWN_Y 7.4f
#define RADIUS_GOAL_OUT 44.7f
#define GOAL_OUT_X_THRESHOLD 24
#define GOAL_OUT_Y_THRESHOLD 32.4f
#define GOAL_OUT_X_THRESHOLD_ALL 45
#define DIST_BETWEEN_GOALS 231

#define GO_RIGHT 0
#define GO_LEFT 1

class ProcessingCoord {
	public:
		ProcessingCoord();
		void setGoal(uint8_t currentGoal);
		void setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow);
		Vec2b ñheckOUTs(Vec2b current);
		Vec2b getVecForMyCircle(int16_t x, int16_t y);
		Vec2b getVecForEnemyCircle(int16_t x, int16_t y);
		int16_t adduct(int16_t value);
		int16_t adduct180(int16_t value);
		int16_t getTargetForward();
		int16_t getTargetGoalkeeper();
		Vec2b getVecToGoalCenter();
		Vec2b getVecToIntersection(int16_t angBall);
		bool getDirectionRobot(int16_t angle);
		bool isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool checkXLeft(int16_t x);
		bool checkXRight(int16_t x);
		bool checkYUp(int16_t y);
		bool checkYDown(int16_t y);
		bool myGoalLine(int16_t x, int16_t y);
		bool enemyGoalLine(int16_t x, int16_t y);
		bool robotInOUT();
	//private:
		Vec2b _leftFast, _rightFast;
		Vec2b _upFast, _downFast;
		int16_t _targetIMU;
		int16_t _x, _y, _angle;
		int16_t _dBlue, _dYellow;
		uint8_t _goal;
		bool inOUT;
		int16_t angleBallGoal;
};