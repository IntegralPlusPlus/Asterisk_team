#pragma once
#include "project_config.h"
#include "Processing_coord.h"
#include "Vec2b.h"

#define GOAL_OUT_X_THRESHOLD_RIGHT 53
#define GOAL_OUT_X_THRESHOLD_LEFT -53

#define MAX_COEFF_TO_GOAL_CENTER 3.2f
//3.3 2.6 2.75f
#define MAX_LEN_TO_INCREASE_VEC 0.5f
//0.6 0.68f
#define GK_X_THRESHOLD_LEFT -41
#define GK_X_THRESHOLD_RIGHT 30
#define CRITICAL_DOWN_Y 15
//12

#define ANGLE_LOW_DOESNT_LEAVE 44
#define ANGLE_HIGH_DOESNT_LEAVE 144

#define ANGLE_TO_LEAVE 200
#define LONG_DISTANCE_TO_GOAL 60
#define MS_BALL_NOT_FORWARD 20

#define THRESHOLD_ANGLEIMU_HIGH 35
#define THRESHOLD_ANGLEIMU_LOW -42

enum GoalkeeperPos {
	centralLine,
	leftPart,
	rightPart
};

enum angleIMUStatus {
	byEnemyGoal,
	byMyGoal
};

class Goalkeeper : public ProcessingCoord {
	public:
		Goalkeeper();
		void setLeaveTime(int16_t leaveTime);
		Vec2b getVecToGoalCenter();
		Vec2b getVecToIntersection(int16_t angBall);
		Vec2b getVecToReturn();
		float getCoeffToGoalCenter(float intersec);
		int16_t getTargetGoalkeeper();
		int16_t getTargetRadiuses();
		int16_t getCurrentLeaveTime(int16_t angBall);
		uint8_t getGoalkeeperPos();
		uint8_t setAngleLeaveStatus();
		bool changeFromReturn();
		bool angleStopLeaving(float ang);
		bool dist2GoalLong();
	private:
		float distToGoalCenter;
		float errOldGkLeft, errOldGkLine, errOldGkRight; 
		int16_t _targetIMU, globalAngToBall;
		int16_t _leaveTime, countBall;
		int16_t _downYRight, _downYLeft;
		uint32_t timerLeave;
};