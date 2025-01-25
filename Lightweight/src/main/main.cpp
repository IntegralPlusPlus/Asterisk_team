#include "Robot.h"

int main() {
	Asterisk::init(BLUE_GOAL, FORWARD_ROLE);
	Vec2b curr;
	volatile float x, y, dBl, dYe, dist, kAng, kLen, ballADC;
	volatile float angleTSSP, target, angleIMU, ballVal;
	volatile uint64_t timeCheckLeave, buttonForwTime, timeBallFront;
	volatile bool mayKickBall, mustLeave, ballInGrip;
	volatile bool inLeave, inReturn;
	volatile bool enemyGoal;
	volatile bool doesntSeeGoals;
	

	//Asterisk::update();
	if (Asterisk::reset_imu_pin.readPin()) Asterisk::NEED_TO_CALIBRATE = 1;
	else Asterisk::NEED_TO_CALIBRATE = 0;
	
	while (true) {
		Asterisk::update();
		curr = Asterisk::currentVector;
		doesntSeeGoals = Asterisk::doesntSeeGoals;
		dBl = Asterisk::dBl;
		dYe = Asterisk::dYe;
		x = Asterisk::x;
		y = Asterisk::y;
		dist = Asterisk::dist;
		target = Asterisk::myForward.adduct180(Asterisk::ang - Asterisk::angleIMU);//abs(-Asterisk::angleIMU - Asterisk::gyro.getTarget());
		angleTSSP = Asterisk::ang;//Asterisk::myForward.adduct180(Asterisk::ang - Asterisk::angleIMU);	
		timeCheckLeave = Asterisk::timeCheckLeave;
		ballVal = Asterisk::ballVal;
		//mayKickBall = Asterisk::mayKick2Ball();
		angleIMU = Asterisk::angleIMU;
		kAng = Asterisk::kAng;
		kLen = Asterisk::kLen;
		buttonForwTime = Asterisk::timeMotorsWork;
		mustLeave = Asterisk::robotMustLeave;
		inLeave = Asterisk::inLeave;
		inReturn = Asterisk::inReturn;
		ballADC = Asterisk::ballSens.getValue();
		enemyGoal = Asterisk::myForward.isEnemyGoalCircle(Asterisk::x, Asterisk::y, Asterisk::dBl, Asterisk::dYe);
		//timeLong = Asterisk::ballSens.timeInGrip;
		timeBallFront = Asterisk::timeBallFront;
		ballInGrip = Asterisk::ballGrip;
		
		if (!Asterisk::calibrated()) continue;
		if (Asterisk::getRole() == FORWARD_ROLE) Asterisk::forwardStrategy();
		else if (Asterisk::getRole() == GOALKEEPER_ROLE) Asterisk::goalkeeperStrategy();
	}
	
	return 0;
}