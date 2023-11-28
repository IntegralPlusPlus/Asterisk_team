//for stm32f407!!!

#include "mpu9250_spi.h"

#define RAD2DEG	57.2957795130823208767
#define DEG2RAD	0.01745329251994329576
#define beta		0.25f		// 2 * proportional gain

mpu9250_spi::mpu9250_spi(Pin &ss):_ss(ss)
{}

inline int mpu9250_spi::convert(uint16_t msb, uint8_t lsb)
{
	uint16_t a = (msb<<8) | lsb;
	if(a & 32768) return (a & 32767) - 32768;
	else return a;
}


///volatile int32_t alucard = 0;
int mpu9250_spi::readReg(int regAddr, int * data, int q)
{
	//setPin(mpuSSPin, 0);
  _ss.resetBit();
	//writeSPI(mpuSPI, 0x80 | regAddr);
  writeSPI(0x80 | regAddr);
	for(int i = 0; i < q; i++) {
		//data[i] = writeSPI(mpuSPI, 0);
    data[i] = writeSPI(0);
  }
	int a = writeSPI(0);
	//setPin(mpuSSPin, 1);
  _ss.setBit();
	return a;
}



void mpu9250_spi::writeReg(int regAddr, uint8_t data) {
//	setPin(mpuSSPin, 0);
//	writeSPI(mpuSPI, regAddr);
//	writeSPI(mpuSPI, data);
//	setPin(mpuSSPin, 1);
  _ss.resetBit();
  writeSPI(regAddr);
  writeSPI(data);
  _ss.setBit();
}



void mpu9250_spi::initIMU()
{
	time_service::delay(10);
	ACC_DIV = 32760.0 / ACC_FS;
	GYRO_DIV = 32760.0 / GYRO_FS / DEG2RAD;
	
	xGyroOffset = 0.00, yGyroOffset = 0.0, zGyroOffset = 0;
	xAccOffset = 0.00, yAccOffset = 0.00, zAccOffset = 0.00;
	pitchOffset = 0, rollOffset = 0, yawOffset = 0;
	
	xa = 0, ya = 0, za = 0;
	xg = 0, yg = 0, zg = 0;
	temp = 0;
	pitch = 0, roll = 0, yaw = 0;
	q0 = 1.0, q1 = 0.0, q2 = 0.0, q3 = 0.0;
	time = 0;
	
	int accFSsel, gyroFSsel;
	if(ACC_FS == 2) accFSsel = 0x0;
	else if(ACC_FS == 4) accFSsel = 0x8;
	else if(ACC_FS == 8) accFSsel = 0x10;
	else if(ACC_FS == 16) accFSsel = 0x18;
	
	if(GYRO_FS == 250) gyroFSsel = 0x0;
	else if(GYRO_FS == 500) gyroFSsel = 0x8;
	else if(GYRO_FS == 1000) gyroFSsel = 0x10;
	else if(GYRO_FS == 2000) gyroFSsel = 0x18;

		
	initSPI2(1, 8, 128);			//init spi
	
	//initPin(mpuSSPin, OUTPUTPP);!!!!!!!!!!!!
	//setPin(mpuSSPin, 1);
  _ss.setBit();
	
	writeReg(PWR_MGMT_1, 0x80);			//reset
	time_service::delay(10);
	writeReg(PWR_MGMT_1, 0x01);			//autoselect the best available clock source
	time_service::delay(1);
	writeReg(PWR_MGMT_2, 0x00);			//enable all axis of accelerometer and gyroscope
	time_service::delay(1);
	writeReg(CONFIG, 0x03);						//DLPF (digital low pass filter) = 3
	time_service::delay(1);
	writeReg(GYRO_CONFIG, gyroFSsel);			//set gyroscope scale, FCHOICE = 2b11 (FCHOICE_b = 2b00)
	time_service::delay(1);
	writeReg(ACCEL_CONFIG, accFSsel);			//set accelerometer scale
	time_service::delay(1);
	writeReg(ACCEL_CONFIG_2, 0x03);			//accelerometer DLPF = 3, FCHOICE = 1 (FCHOICE_b = 0)
	time_service::delay(1);
	writeReg(INT_PIN_CFG, 0x30);				//latch INT pin untill any read operation
	time_service::delay(1);
	writeReg(USER_CTRL, 0x04);			//enable fifo (first input first output buffer) and reset it
	time_service::delay(1);
	writeReg(USER_CTRL, 0x40);
	time_service::delay(1);
	writeReg(FIFO_EN, 0x78);			//write accel and gyro data to FIFO
	time_service::delay(5);
	
	
	setGyroOffset(0, 0, 0);
	setAccOffset(0, 0, 0);
	
	SPI2->CR1 &= ~SPI_CR1_BR;
	SPI2->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;

	//correctionTimer = millis();
	correctionTimer = time_service::millis();
}



void mpu9250_spi::readAcc( )						//reads data directly from accelerometer registers
{
	int data[6];
	readReg(ACCEL_XOUT_H, data, 6);
	xa = double(convert(data[0], data[1])) / ACC_DIV - xAccOffset;
	ya = double(convert(data[2], data[3])) / ACC_DIV - yAccOffset;
	za = double(convert(data[4], data[5])) / ACC_DIV - zAccOffset;
}



void mpu9250_spi::readGyro()					//reads data directly from gyroscope registers. Returns values in RAD/S
{
	int data[6];
	readReg(GYRO_XOUT_H, data, 6);
	xg = double(convert(data[0], data[1])) / GYRO_DIV - xGyroOffset;
	yg = double(convert(data[2], data[3])) / GYRO_DIV - yGyroOffset;
	zg = double(convert(data[4], data[5])) / GYRO_DIV - zGyroOffset;
}

int mpu9250_spi::read_gyro_fs_sel()
{
  int _data[8];
  return readReg(GYRO_CONFIG, _data, 1);
}

int mpu9250_spi::read_acc_fs_sel()
{
  int _data[8];
  return readReg(ACCEL_CONFIG, _data, 1);
}

void mpu9250_spi::readTemp()					//reads data directly from temperature registers
{
	temp = (convert(readReg(TEMP_OUT_H), readReg(TEMP_OUT_L)) - 21.0)/333.87 + 21.0;	//21.0 and 333.87 are some magic numbers
}



int mpu9250_spi::readFIFOsize()					//reads quantity of bites in FIFO
{
	return convert((readReg(FIFO_COUNTH) & 0x1F), readReg(FIFO_COUNTL));
}



void mpu9250_spi::readFIFO()					//reads accelerometer and gyroscope data from FIFO
{
	int d0 = readReg(FIFO_R_W);			//first writes values to variables because
	int d1 = readReg(FIFO_R_W);			//just read from fifo rw register is too fast
	int d2 = readReg(FIFO_R_W);			//(values in that buffer havent enough time to update)
	int d3 = readReg(FIFO_R_W);			//da, eto kostyl`!
	int d4 = readReg(FIFO_R_W);
	int d5 = readReg(FIFO_R_W);
	int d6 = readReg(FIFO_R_W);
	int d7 = readReg(FIFO_R_W);
	int d8 = readReg(FIFO_R_W);
	int d9 = readReg(FIFO_R_W);
	int d10 = readReg(FIFO_R_W);
	int d11 = readReg(FIFO_R_W);
	xa = double(convert(d0, d1) - xAccOffset) / ACC_DIV;
	ya = double(convert(d2, d3) - yAccOffset) / ACC_DIV;
	za = double(convert(d4, d5) - zAccOffset) / ACC_DIV;
	
	xg = double(convert(d6, d7) - xGyroOffset) / GYRO_DIV;
	yg = double(convert(d8, d9) - yGyroOffset) / GYRO_DIV;
	zg = double(convert(d10, d11) - zGyroOffset) / GYRO_DIV;
}



void mpu9250_spi::setGyroOffset(int _xOffset, int _yOffset, int _zOffset)				//sets hardware gyroscope offset for +-1000°/s full-scale
{
	writeReg(XG_OFFSET_H, _xOffset >> 8);
	writeReg(XG_OFFSET_L, _xOffset & 0xFF);
	writeReg(YG_OFFSET_H, _yOffset >> 8);
	writeReg(YG_OFFSET_L, _yOffset & 0xFF);
	writeReg(ZG_OFFSET_H, _zOffset >> 8);
	writeReg(ZG_OFFSET_L, _zOffset & 0xFF);
}



void mpu9250_spi::setAccOffset(int xOffset, int yOffset, int zOffset)				//sets hardware accelerometer offset for +-16g full-scale
{
	writeReg(XG_OFFSET_H, xOffset >> 7);
	writeReg(XG_OFFSET_L, (xOffset & 0x7F) << 1);
	writeReg(YG_OFFSET_H, yOffset >> 7);
	writeReg(YG_OFFSET_L, (yOffset & 0x7F) << 1);
	writeReg(ZG_OFFSET_H, zOffset >> 7);
	writeReg(ZG_OFFSET_L, (zOffset & 0x7F) << 1);
}



//For calibration imu must stand still (for finding gyro offsets)
//and pointing acc Z axis straight downwards!!!
//
//If other axis points upperwards
//it`s (n is x, y or z) nAccOffset calculates as 
//_dan/_T - (32768 / ACC_FS) (or + (32768 / ACC_FS) if it points downwards)

void mpu9250_spi::calibrate(int _T) {
	xGyroOffset = 0, yGyroOffset = 0, zGyroOffset = 0;
	double _dgx = 0, _dgy = 0, _dgz = 0;

	
	time_service::delay(1000);
	
	int num = _T;
	
	for(int i = 0; i < num; i++) {
		readGyro();
		
		_dgx += xg;
		_dgy += yg;
		_dgz += zg;

		time_service::delay(1);
	}

	xGyroOffset = _dgx/num;
	yGyroOffset = _dgy/num;
	zGyroOffset = _dgz/num;
}


void mpu9250_spi::correct() {
	yawOffset -= yaw / (time_service::millis() - correctionTimer);
	correctionTimer = time_service::millis();
}


void mpu9250_spi::MadgwickAHRSupdateIMU(double _time) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * xg - q2 * yg - q3 * zg);
	qDot2 = 0.5f * (q0 * xg + q2 * zg - q3 * yg);
	qDot3 = 0.5f * (q0 * yg - q1 * zg + q3 * xg);
	qDot4 = 0.5f * (q0 * zg + q1 * yg - q2 * xg);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((xa == 0.0f) && (ya == 0.0f) && (za == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(xa * xa + ya * ya + za * za);
		xa *= recipNorm;
		ya *= recipNorm;
		za *= recipNorm;   

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		q0q0 = q0 * q0;
		q1q1 = q1 * q1;
		q2q2 = q2 * q2;
		q3q3 = q3 * q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * xa + _4q0 * q1q1 - _2q1 * ya;
		s1 = _4q1 * q3q3 - _2q3 * xa + 4.0f * q0q0 * q1 - _2q0 * ya - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * za;
		s2 = 4.0f * q0q0 * q2 + _2q0 * xa + _4q2 * q3q3 - _2q3 * ya - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * za;
		s3 = 4.0f * q1q1 * q3 - _2q1 * xa + 4.0f * q2q2 * q3 - _2q2 * ya;
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback stepcorrect
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * _time;
	q1 += qDot2 * _time;
	q2 += qDot3 * _time;
	q3 += qDot4 * _time;

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}



float mpu9250_spi::invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}



void mpu9250_spi::updateAngles(double _time, bool flag1) {
	if(_time == -1) {
		_time = double(time_service::millis() - time) / 1000;
    _additional_time = _time;
		time = time_service::millis();
		if(_time > 1) _time = 0;
	}
	
	if(flag1) {
		readAcc();
		readGyro();
	}
  
	MadgwickAHRSupdateIMU(_time);

	pitch = asin(2*(q0*q2 - q3*q1));
	roll = atan2(2*(q0*q1 + q2*q3), 1 - 2*(q1*q1 + q2*q2));
	yaw = atan2(2*(q0*q3 + q1*q2), 1 - 2*(q2*q2 + q3*q3));
	
	pitch = pitch * RAD2DEG - pitchOffset;
	roll = roll * RAD2DEG - rollOffset;
	yaw = yaw * RAD2DEG - yawOffset;
  
  _acceleration = zg;
  
  _old_yaw = yaw;
	/*
	while (pitch < -180) pitch += 360;
	while (pitch > 180) pitch -= 360;
	while (roll < -180) roll += 360;
	while (roll > 180) roll -= 360;
	while (yaw < -180) yaw += 360;
	while (yaw > 180) yaw -= 360;
	*/
}



void mpu9250_spi::updateAnglesFromFIFO()
{
	int FIFOsize = readFIFOsize() / 12;			//12 bytes is packet size (6 for gyro and 6 for acc)

	for(int i = 0; i < FIFOsize; i++) {
		updateAngles(-1, 1);			//0.001s because gyro and acc sample rate is 1 kHz
	}	
}

double mpu9250_spi::getXa() {
	return xa;
}

double mpu9250_spi::getYa() {
	return ya;
}

double mpu9250_spi::getZa() {
	return za;
}

double mpu9250_spi::getXg() {
	return xg;
}

double mpu9250_spi::getYg() {
	return yg;
}

double mpu9250_spi::getZg() {
	return zg;
}