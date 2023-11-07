#ifndef IMU_LIB
#define IMU_LIB

#include "project_config.h"
#include "mpu9250_spi.h"
#include "SPI_DOT.h"


class IMU {
	public:
    IMU(mpu9250_spi &mpuSensor): _mpuSensor(mpuSensor){}
		void init();
		void setZeroAngle();
		void calibrate(uint32_t t);
		unsigned int update();
		void updateAnglesFromFIFO();
		double getAngle();
		double imuFloatValue, angleChange;
		long long int imuFloatTime;
		double getXa();
		double getYa();
		double getZa();
		double getXg();
		double getYg();
		double getZg();
    int gyro_sel();
    int acc_sel();
    float get_acceleration();
		mpu9250_spi _mpuSensor;
	
	private:
		//unsigned int spi;
		uint16_t en, ss;
		volatile double angle, zeroAngle;
		//bool working;
};

#endif
