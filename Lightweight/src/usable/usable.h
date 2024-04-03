#pragma once
#include "project_config.h"

#ifndef USABLE_H_
#define USABLE_H_

int16_t adduct180_360(int16_t ang) {
	while (ang < 0) ang += 360;
	while (ang > 360) ang -= 360;
	
	return ang;
}

int16_t adduct360_180(int16_t ang) {
	while (ang < -180) ang += 360;
	while (ang > 180) ang -= 360;
	
	return ang;
}

void delay_(int n) {
	while (n > 0) --n;
}

/*
float map(float a, float from1, float from2, float to1, float to2) {
	return to1 + a * (to2 - to1) / (from2 - from1);
}*/

float sign(float a) {
	if (a > 0) return 1;
	else if (a < 0) return -1;
	else return 0;	
}

#endif /* USABLE_H_ */