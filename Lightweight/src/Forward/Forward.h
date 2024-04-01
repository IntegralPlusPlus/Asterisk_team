#pragma once
#include "project_config.h"
#include "Processing_coord.h"
#include "Vec2b.h"

#define NEAR_OUT_DIST 11
#define BACK_SECTOR 120

enum processingOUT {
	unknow,
	up,
	down,
	left,
	right,
	enemyCircle,
	myCircle
};

class Forward : public ProcessingCoord {
	public:
		Forward();
		void resetCounts();
		Vec2b getVecForMyCircle(int16_t x, int16_t y);
		Vec2b getVecForEnemyCircle(int16_t x, int16_t y);
		Vec2b setOUTVector(uint8_t status, Vec2b current);
		int16_t getTargetForward();
		uint8_t checkOUTs();
		bool ballInBack(float angBall);
		bool nearEnemyGoal();
		bool nearMyGoal();	
		bool robotNearOUTUpDown();
		bool robotNearOUTSides();
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
	private:
		int16_t upThreshold, downThreshold, leftThreshold, rightThreshold;
		float distToGoalCenter;
		int16_t _targetIMU;
		int16_t _countCyclesOUT, _countCyclesField;
		bool inOUT;
};