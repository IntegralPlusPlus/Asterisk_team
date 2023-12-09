#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Vec2b {
	public:
		Vec2b();
		Vec2b(double len, double ang);
		void set(double len, double ang);
		double length;
		double angle;
};

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

int main() {
    vector<Vec2b> vec;

    /*vec.push_back(Vec2b(13, 12));
    vec.push_back(Vec2b(13, 13));
    vec.push_back(Vec2b(13, 14));
    vec.push_back(Vec2b(14, 13));
    vec.push_back(Vec2b(14, 14));
    vec.push_back(Vec2b(14, 14));
    vec.push_back(Vec2b(15, 14));
    vec.push_back(Vec2b(16, 15));
    vec.push_back(Vec2b(17, 16));
    vec.push_back(Vec2b(18, 16));
    vec.push_back(Vec2b(20, 16));
    vec.push_back(Vec2b(18, 16));
    vec.push_back(Vec2b(18, 16));*/

    vec.push_back(Vec2b(13, 358));
    vec.push_back(Vec2b(13, 359));
    vec.push_back(Vec2b(13, 360));
    vec.push_back(Vec2b(13, 1));
    vec.push_back(Vec2b(14, 2));
    vec.push_back(Vec2b(14, 3));
    vec.push_back(Vec2b(15, 5));

    double n = vec.size();
    double summLen = 0, summ2Len = 0, summAng = 0, summ2Ang = 0, summT = 0, summLenT = 0, summAngT = 0;
    int minAngle;
    for (int i = 0; i < n; ++i) {
        if (i == 0) minAngle = vec[i].angle;
        else if (abs(vec[i].angle - minAngle) < 180 && minAngle > vec[i].angle) minAngle = vec[i].angle;
        else if (abs(vec[i].angle - minAngle) > 180) {
            if (minAngle < vec[i].angle) vec[i].angle -= 360;
            else vec[i].angle += 360;
        }

        summLen += vec[i].length;
        summ2Len += vec[i].length * vec[i].length;
        summAng += vec[i].angle;
        summ2Ang += vec[i].angle * vec[i].angle;
        summT += i;
        summLenT += (vec[i].length * i);
        summAngT += (vec[i].angle * i);
        cout << "(" << vec[i].angle << ", " << i << ")" << endl;
    }

    double bLen = (summLenT / n - summLen * summT / pow(n, 2)) / (summ2Len / n - pow((summLen / n), 2));
    double aLen = summT / n - bLen * summLen / n;

    double bAng = (summAngT / n - summAng * summT / pow(n, 2)) / (summ2Ang / n - pow((summAng / n), 2));
    double aAng = summT / n - bAng * summAng / n;

    cout << "LENGTH: " << aLen << " + " << bLen << " * x" << endl;
    cout << "ANGLE: " << aAng << " + " << bAng << " * x" << endl;

    return 0;
}
