#pragma once
#include "project_config.h"
#include "time_service.h"
#include "Vec2b.h"

#define ENEMY_X 0
#define ENEMY_Y 231

#define THRESHOLD_X_RIGHT 50 //Right threshold to goalkeeper
#define THRESHOLD_X_LEFT -75 //Left threshold to goalkeeper

#define UP_Y 220
//217
#define DOWN_Y 32.f
//202

//Threshold to own out in front of the goal
#define GOAL_OUT_Y_THRESHOLD 35

#define DIST_BETWEEN_GOALS 231

//Threshold to transition from a straight line to a circle
//To own out in front of the goal
#define ANGLE_LOW_TO_CIRCLE 55 //Right part of the circle
#define ANGLE_HIGH_TO_CIRCLE 138 //Left part of the circle

//Radius of the right part of the out in front of the goal 
#define RADIUS_GOAL_OUT_RIGHT 52
//52

//Radius of the left part of the out in front of the goal
#define RADIUS_GOAL_OUT_LEFT 46
//44

#define BACK_ANGLE 20

//Delta in +-all thresholds
#define DELTA_DIST 8
#define SAVE_DELTA_GK 13

//Ball is back from robot if he is in [LEFT_GOAL_THRESHOLD; RIGHT_GOAL_THRESHOLD] area
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
		void setParams(int16_t x, int16_t y, int16_t angle, int16_t dBlue, int16_t dYellow);
		void setMaxLen(float len);
		Vec2b getVecToPoint(int16_t pointX = 0, int16_t pointY = GOAL_OUT_Y_THRESHOLD);
		float distance(float x, float y, float startX = 0, float startY = 0);
		float map(float a, float from1, float to1, float from2, float to2);
		float getAngleBetween(float ang1, float ang2);
		int16_t adduct(int16_t value);
		int16_t adduct180(int16_t value);
		int16_t getTarget2Enemy(); //Target to enemy goal
		bool checkYUp(int16_t y);
		bool checkYDown(int16_t y);
		
	bool checkXLeft(int16_t x, uint8_t role = FORWARD_ROLE); //Check left threshold
		bool checkXRight(int16_t x, uint8_t role = FORWARD_ROLE); //Check right threshold
		bool ballInBack(float angBall, uint8_t varible); //Ball in back of robot
		bool suitableParams2Kick(); //If robot may kick now 
	protected:
		float _maxLen;
		int16_t upThreshold, downThreshold, leftThreshold, rightThreshold;
		int16_t _x, _y, _angle;
		int16_t _dBlue, _dYellow;
		uint8_t _goal;
};