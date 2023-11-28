#ifndef USABLE_H_
#define USABLE_H_

void delay_(int n) {
	while (n > 0) --n;
}

float map(float a, float from1, float from2, float to1, float to2) {
	return to1 + a * (to2 - to1) / (from2 - from1);
}

float sign(float a) {
	if (a > 0) return 1;
	else if (a < 0) return -1;
	else return 0;	
}

#endif /* USABLE_H_ */