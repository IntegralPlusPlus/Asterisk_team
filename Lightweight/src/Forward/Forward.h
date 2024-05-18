#pragma once
#include "project_config.h"
#include "Processing_coord.h"
#include "Vec2b.h"

#define DELTA_DIST 8
#define NEAR_OUT_DIST 10
//18
#define BACK_SECTOR 140
#define BACK_SECTOR_DETOUR 30
#define COEFF_CIRCLE 1.5f
#define ANGLE2SIDES 20
#define MINIMUM_SPEED_TO_BALL 0.35f  
#define LIMIT_OUT_CYCLES 20
#define ANGLE_LOW_TO_CIRCLE_ENEMY 65
#define ANGLE_HIGH_TO_CIRCLE_ENEMY 128

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

enum nearOUT {
	standartNear,
	highNear
};

struct OutPair {
	OutPair() {
		out1 = unknow;
		out2 = unknow;
	}
	
	OutPair(uint8_t out1, uint8_t out2) {
		this->out1 = out1;
		this->out2 = out2;
	}
	
	void setOut(uint8_t out) {
		if (out1 == unknow) out1 = out;
		else if (out2 == unknow) out2 = out;
	}

	bool isDefault() {
		return out1 == unknow && out2 == unknow;
	}
	
	void operator= (const OutPair& oPair) {
		out1 = oPair.out1;
		out2 = oPair.out2;
	}
	
	bool operator!= (const OutPair& oPair) {
		return !(out1 == oPair.out1 && out2 == oPair.out2);
	}
	
	bool operator!= (const uint8_t& state) {
		return !(out1 == state || out2 == state);
	}
	
	uint8_t out1, out2;
};

class Forward : public ProcessingCoord {
	public:
		Forward();
		void resetCounts();
		Vec2b getVecForMyCircle(int16_t x, int16_t y);
		Vec2b getVecForEnemyCircle(int16_t x, int16_t y);
		Vec2b setResOUTVector(OutPair status, Vec2b current);
		Vec2b setVec2Out(uint8_t status, Vec2b current);
		Vec2b vec2bOnGoal(float speed, float angBall);
		Vec2b projectionOnY(Vec2b vec);
		float setNearSpeed(uint8_t status, float maxSpeed);
		OutPair checkOUTs();
		uint8_t getBallSide(float angBall);
		uint8_t robotNearOUT(uint8_t nearStatus = standartNear);
		bool ballInOUT(float globalAng);
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