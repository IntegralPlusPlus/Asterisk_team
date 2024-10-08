#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Vec2b.h"

#define ENEMY_X 0
#define ENEMY_Y 231
#define THRESHOLD_X_RIGHT 50
//54
#define THRESHOLD_X_LEFT -75
#define UP_Y 220
//217
#define DOWN_Y 32.f
//202

#define GOAL_OUT_Y_THRESHOLD 35//30
//33
//#define GOAL_OUT_Y_THRESHOLD_FORWARD 30
#define DIST_BETWEEN_GOALS 231

#define ANGLE_LOW_TO_CIRCLE 55
#define ANGLE_HIGH_TO_CIRCLE 138
#define RADIUS_GOAL_OUT_RIGHT 52
//52
#define RADIUS_GOAL_OUT_LEFT 46
//44

#define BACK_ANGLE 20
#define DELTA_DIST 8
#define SAVE_DELTA_GK 13

#define LEFT_GOAL_THRESHOLD -23 
#define RIGHT_GOAL_THRESHOLD 20
#define DELTA_GOAL_THRESHOLD 0

enum ballAng {
	tsopRaw,
	byVec
};

class ProcessingCoord {
	public:
		ProcessingCoord();
		void setGoal(uint8_t currentGoal);
		//void setRole(uint8_t currentRole);
		void setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow);
		void setMaxLen(float len);
		Vec2b getVecToPoint(int16_t pointX = 0, int16_t pointY = GOAL_OUT_Y_THRESHOLD);
		float distance(float x, float y, float startX = 0, float startY = 0);
		float map(float a, float from1, float to1, float from2, float to2);
		float getAngleBetween(float ang1, float ang2);
		int16_t adduct(int16_t value);
		int16_t adduct180(int16_t value);
		int16_t getTarget2Enemy();
		bool checkXLeft(int16_t x, uint8_t role = FORWARD_ROLE);
		bool checkXRight(int16_t x, uint8_t role = FORWARD_ROLE);
		bool checkYUp(int16_t y);
		bool checkYDown(int16_t y);
		bool ballInBack(float angBall, uint8_t varible);
		bool suitableParams2Kick();
	protected:
		float _maxLen;
		int16_t upThreshold, downThreshold, leftThreshold, rightThreshold;
		int16_t _x, _y, _angle;
		int16_t _dBlue, _dYellow;
		uint8_t _goal;
		//int16_t GOAL_OUT_Y_THRESHOLD;
};