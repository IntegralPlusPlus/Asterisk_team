#pragma once
#include "project_config.h"
#include "Processing_coord.h"
#include "Vec2b.h"

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
		int16_t getCurrentLeaveTime();
		uint8_t getGoalkeeperPos();
		bool changeFromReturn();
		float getCoeffToGoalCenter(float intersec);
	private:
		float distToGoalCenter;
		float errOldGkLeft, errOldGkLine, errOldGkRight; 
		int16_t _targetIMU;
		int16_t _leaveTime;
};