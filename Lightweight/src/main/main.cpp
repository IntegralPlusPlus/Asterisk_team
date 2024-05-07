#include "Robot.h"

int main() {
	Asterisk::init(BLUE_GOAL, FORWARD_ROLE);
	Vec2b curr;
	volatile float x, y, dist, kAng, kLen;
	volatile float angleTSSP, target, angleIMU;
	volatile uint64_t timeCheckLeave;
	volatile bool mayKickBall;
	
	while (true) {
		Asterisk::update();
		curr = Asterisk::currentVector;
		x = Asterisk::x;
		y = Asterisk::y;
		dist = Asterisk::dist;
		target = abs(-Asterisk::angleIMU - Asterisk::gyro.getTarget());
		angleTSSP = Asterisk::ang;//Asterisk::myForward.adduct180(Asterisk::ang - Asterisk::angleIMU);	
		timeCheckLeave = Asterisk::timeCheckLeave;
		mayKickBall = Asterisk::mayKick2Ball();
		angleIMU = Asterisk::angleIMU;
		kAng = Asterisk::kAng;
		kLen = Asterisk::kLen;
		//timeLong = Asterisk::ballSens.timeInGrip;
		
		if (!Asterisk::calibrated()) continue;
		if (Asterisk::getRole() == FORWARD_ROLE) Asterisk::forwardStrategy();
		else if (Asterisk::getRole() == GOALKEEPER_ROLE) Asterisk::goalkeeperStrategy();
	}
	
	return 0;
}