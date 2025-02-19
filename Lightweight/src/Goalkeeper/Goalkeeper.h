#pragma once
#include "project_config.h"
#include "Processing_coord.h"
#include "Vec2b.h"

#define GOAL_OUT_X_THRESHOLD_RIGHT 53
#define GOAL_OUT_X_THRESHOLD_LEFT -53

//Maximum increase coefficint to pressure on the goal
#define MAX_COEFF_TO_GOAL_CENTER 3.2f
//3.3 2.6 2.75f

//From MAX_LEN_TO_INCREASE_VEC value of defend-from-ball vector
//pressure on the goal begins to increase
#define MAX_LEN_TO_INCREASE_VEC 0.5f
//0.6 0.68f

//Left and right thresholds to goalkeeper
#define GK_X_THRESHOLD_LEFT -41
#define GK_X_THRESHOLD_RIGHT 30

//Minimum value of Y for goalkeeper   was:12
#define CRITICAL_DOWN_Y 19 //12

//Transition from out-direct-line to out-circle-line
#define ANGLE_LOW_DOESNT_LEAVE 44
#define ANGLE_HIGH_DOESNT_LEAVE 144

#define ANGLE_TO_LEAVE 200

//Transition to "Return to goal" 
//from usual goalkeeper status (status goal-defend)
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
	
		//Vector to defend goal from ball
		Vec2b getVecToIntersection(int16_t angBall);
	
		//Increase the pressure to the out with a high value of the ball protection 
		float getCoeffToGoalCenter(float intersec);
		
		//Get vector to out goal out
		Vec2b getVecToGoalCenter();
	
		int16_t getTargetGoalkeeper();
		int16_t getTargetRadiuses();
		uint8_t getGoalkeeperPos();
		uint8_t setAngleLeaveStatus();
		Vec2b getVecToReturn();
	
		//Define leaving time to goalkeeper
		int16_t getCurrentLeaveTime(int16_t angBall);
		
		//Change status from "Return to my goal" to "Now I am just in my goal"
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