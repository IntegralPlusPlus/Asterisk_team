#pragma once
#include "project_config.h"
#include "Processing_coord.h"
#include "Vec2b.h"

#define DELTA_DIST 8
#define NEAR_OUT_DIST 15
//18
#define BACK_SECTOR 140
#define BACK_SECTOR_DETOUR 30
#define COEFF_CIRCLE 1.5f
#define ANGLE2SIDES 20
#define MINIMUM_SPEED_TO_BALL 0.5f  
#define LIMIT_OUT_CYCLES 20

enum processingOUTs {
	unknow,
	up,
	down,
	left,
	right,
	enemyCircle,
	myCircle
};

enum ballSides {
	up_left,
	down_right
};

enum fieldZones {
	leftZone,
	middleZone,
	rightZone
};

enum detour {
	defaultDetour,
	leftDetour,
	rightDetour	
};

class Forward : public ProcessingCoord {
	public:
		Forward();
		void resetCounts();
		Vec2b getVecForMyCircle(int16_t x, int16_t y);
		Vec2b getVecForEnemyCircle(int16_t x, int16_t y);
		Vec2b setOUTVector(uint8_t status, Vec2b current);
		Vec2b vec2bOnGoal(float speed, float angBall);
		Vec2b projectionOnY(Vec2b vec);
		float setNearSpeed(uint8_t status, float maxSpeed);
		uint8_t checkOUTs();
		uint8_t getBallSide(float angBall);
		uint8_t setFieldZone();
		uint8_t robotNearOUT();
		bool inEnemyGoal();
		bool inMyGoal();
		bool nearMyGoal();	
		bool isMyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool isEnemyGoalCircle(int16_t x, int16_t y, int16_t dBlue, int16_t dYellow);
		bool myGoalLine(int16_t x, int16_t y);
		bool enemyGoalLine(int16_t x, int16_t y);
		bool robotInOUT();
		bool robotInFreeField();
	private:
		float distToGoalCenter;
		int16_t _countCyclesOUT, _countCyclesField;
		int16_t _targetIMU;
		bool inOUT;
};