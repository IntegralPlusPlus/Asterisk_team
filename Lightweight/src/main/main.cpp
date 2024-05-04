#include "Robot.h"

int main() {
	Asterisk::init(YELLOW_GOAL, FORWARD_ROLE);
	Vec2b curr;
	volatile float x, y, dist;
	volatile float angleTSSP, target;
	volatile uint64_t timeBallFront;
	volatile bool mayKickBall;
	
	while (true) {
		Asterisk::update();
		curr = Asterisk::currentVector;
		x = Asterisk::x;
		y = Asterisk::y;
		dist = Asterisk::dist;
		target = abs(-Asterisk::angleIMU - Asterisk::gyro.getTarget());
		angleTSSP = Asterisk::ang;//Asterisk::myForward.adduct180(Asterisk::ang - Asterisk::angleIMU);	
		timeBallFront = Asterisk::timeBallFront;
		mayKickBall = Asterisk::mayKick2Ball();
		//timeLong = Asterisk::ballSens.timeInGrip;
		
		if (!Asterisk::calibrated()) continue;
		if (Asterisk::getRole() == FORWARD_ROLE) Asterisk::forwardStrategy();
		else if (Asterisk::getRole() == GOALKEEPER_ROLE) Asterisk::goalkeeperStrategy();
	}
	
	return 0;
}