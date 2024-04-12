#pragma once
#include "project_config.h"
#include "Processing_coord.h"
#include "Vec2b.h"

#define GOAL_OUT_X_THRESHOLD_RIGHT 57
#define GOAL_OUT_X_THRESHOLD_LEFT -57
#define DOWN_Y_GOALKEEPER_RIGHT 30
#define DOWN_Y_GOALKEEPER_LEFT 23

#define MAX_COEFF_TO_GOAL_CENTER 3.f
#define MAX_LEN_TO_INCREASE_VEC 0.4f
#define GK_X_THRESHOLD_LEFT -38
#define GK_X_THRESHOLD_RIGHT 36

#define ANGLE_LOW_DOESNT_LEAVE 35
#define ANGLE_HIGH_DOESNT_LEAVE 140

enum GoalkeeperPos {
	centralLine,
	leftPart,
	rightPart
};

class Goalkeeper : public ProcessingCoord {
	public:
		Goalkeeper();
		void setLeaveTime(int16_t leaveTime);
		Vec2b getVecToGoalCenter();
		Vec2b getVecToIntersection(int16_t angBall);
		Vec2b getVecToReturn();
		int16_t getTargetGoalkeeper();
		int16_t getTargetRadiuses();
		int16_t getCurrentLeaveTime(int16_t angBall);
		uint8_t getGoalkeeperPos();
		bool changeFromReturn();
		float getCoeffToGoalCenter(float intersec);
	private:
		float distToGoalCenter;
		float errOldGkLeft, errOldGkLine, errOldGkRight; 
		int16_t _targetIMU, globalAngToBall;
		int16_t _leaveTime;
};