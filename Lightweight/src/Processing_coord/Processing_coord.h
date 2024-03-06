#pragma once
#include "project_config.h"
#include "Vec2b.h"

#define ENEMY_X 0
#define ENEMY_Y 231
#define DELTA_DIST 13
#define THRESHOLD_X 79
#define UP_Y 211.f
#define DOWN_Y 30.f

#define GOAL_OUT_X_THRESHOLD_RIGHT 27
#define GOAL_OUT_X_THRESHOLD_LEFT -23
#define GOAL_OUT_Y_THRESHOLD 24
#define GOAL_CIRCLE_Y_THRESHOLD_RIGHT 35.f
#define GOAL_CIRCLE_Y_THRESHOLD_LEFT 34.f
#define GOAL_OUT_X_THRESHOLD_ALL 45
#define DIST_BETWEEN_GOALS 221

#define ANGLE_LOW_TO_CIRCLE 60
#define ANGLE_HIGH_TO_CIRCLE 135
#define RADIUS_GOAL_OUT_RIGHT 35
#define RADIUS_GOAL_OUT_LEFT 44

#define MAX_COEFF_TO_GOAL_CENTER 1.075f
#define MAX_LEN_TO_INCREASE_VEC 0.88f
#define GK_X_THRESHOLD_LEFT -38
#define GK_X_THRESHOLD_RIGHT 36

#define BACK_ANGLE 60
#define LIMIT_OUT_CYCLES 5

enum GoalkeeperPos {
	centralLine,
	leftPart,
	rightPart
};

enum processingOUT {
	unknow,
	up,
	down,
	left,
	right,
	enemyCircle,
	myCircle
};

class ProcessingCoord {
	public:
		ProcessingCoord();
		void setGoal(uint8_t currentGoal);
		void setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow);
		void setMaxLen(float len);
		void setLeaveTime(int16_t leaveTime);
		void resetCounts();
		Vec2b getVecForMyCircle(int16_t x, int16_t y);
		Vec2b getVecForEnemyCircle(int16_t x, int16_t y);
		Vec2b getVecToGoalCenter();
		Vec2b getVecToIntersection(int16_t angBall);
		Vec2b getVecToPoint(int16_t pointX = 0, int16_t pointY = GOAL_OUT_Y_THRESHOLD);
		Vec2b getVecToReturn();
		Vec2b setOUTVector(uint8_t status, Vec2b current);
		int16_t adduct(int16_t value);
		int16_t adduct180(int16_t value);
		int16_t getTargetForward();
		int16_t getTargetGoalkeeper();
		int16_t getCurrentLeaveTime();
		uint8_t getGoalkeeperPos();
		uint8_t checkOUTs();
		bool isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool checkXLeft(int16_t x);
		bool checkXRight(int16_t x);
		bool checkYUp(int16_t y);
		bool checkYDown(int16_t y);
		bool myGoalLine(int16_t x, int16_t y);
		bool enemyGoalLine(int16_t x, int16_t y);
		bool robotInOUT();
		bool robotInFreeField();
		bool changeFromReturn();
		float distance(float x, float y, float startX = 0, float startY = 0);
		float getCoeffToGoalCenter(float intersec);
		float map(float a, float from1, float to1, float from2, float to2);
	private:
		float distToGoalCenter;
		float errOldGkLeft, errOldGkLine, errOldGkRight; 
		float _maxLen;
		int16_t _targetIMU;
		int16_t _x, _y, _angle;
		int16_t _dBlue, _dYellow;
		int16_t DOWN_Y_GOALKEEPER_RIGHT, DOWN_Y_GOALKEEPER_LEFT;
		int16_t _leaveTime;
		int16_t _countCyclesOUT, _countCyclesField;
		uint8_t _goal;
		bool inOUT;
};