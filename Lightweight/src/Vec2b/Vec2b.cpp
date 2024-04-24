#include "Vec2b.h"

Vec2b::Vec2b() {
	this->length = 0;
	this->angle = 0;
}

Vec2b::Vec2b(double len, double ang) {
	this->length = len;
	this->angle = ang;
}

void Vec2b::set(double len, double ang) {
	this->length = len;
	this->angle = ang;
}

void Vec2b::changeTo(Vec2b to) {
	double x1 = double(this->length) * cos(double(this->angle) * DEG2RAD);
	double y1 = double(this->length) * sin(double(this->angle) * DEG2RAD);
	double x2 = to.length * cos(to.angle * DEG2RAD);
	double y2 = to.length * sin(to.angle * DEG2RAD);
	double xDiff = x2 - x1, yDiff = y2 - y1;
	double len = sqrt(xDiff * xDiff + yDiff * yDiff);
	double lyambda;
	
	if (len - DELTA_SPEED > 0) lyambda = double(DELTA_SPEED / (len - DELTA_SPEED));
	else lyambda = 1;
	
	double dx = (lyambda * xDiff) / (1 + lyambda), dy = (lyambda * yDiff) / (1 + lyambda);
	double resX = x1 + dx, resY = y1 + dy;
	this->length = sqrt(resX * resX + resY * resY);
	this->angle = atan2(resY, resX) * RAD2DEG;
	if (this->angle < 0) this->angle += 360;
}

Vec2b Vec2b::operator+ (const Vec2b& a) {
	double x = a.length * cos(DEG2RAD * a.angle) + this->length * cos(DEG2RAD * this->angle);
	double y = a.length * sin(DEG2RAD * a.angle) + this->length * sin(DEG2RAD * this->angle);
	int16_t ang = atan2(y, x) * RAD2DEG;
	if (ang < 0) ang += 360;
	
	return Vec2b(sqrt(x * x + y * y), ang);
}

Vec2b Vec2b::operator*= (const float& coeff) {
	this->length *= coeff;
}

Vec2b Vec2b::operator* (const float& coeff) {
	this->length *= coeff;
}