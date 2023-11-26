#include "Pin.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"
#include "adc.h"
#include "dma.h"
#include "time_service.h"
#include "project_config.h"
#include "Motor.h"
#include "SPI_1.h"
#include "SPI_DOT.h"
#include "mpu9250_spi.h"
#include "IMU_SPI.h"
#include "omniplatform.h"
#include "IMU_USART.h"
#include "OpenMV.h"
#include "usable.h"

int main() 
{
	time_service::init();
	time_service::startTime();
  time_service::delay(100);
	
	Pin tx_imu('C', 6, usart6);
	Pin rx_imu('C', 7, usart6);
	gyro_imu gyro(tx_imu, rx_imu, 6);
	
	
	Pin motor1in1('E', 5, tim9);
	Pin motor1in2('E', 6, tim9);
	motor1in1.pwmInit(2048, 1, 1);
	motor1in2.pwmInit(2048, 1, 2);
	Motor motor1(motor1in1, motor1in2);
	
	Pin motor2in1('B', 3, tim2);
	Pin motor2in2('B', 5, tim3);
	motor2in1.pwmInit(2048, 1, 2);
	motor2in2.pwmInit(2048, 1, 2);
	Motor motor2(motor2in1, motor2in2);
	
	Pin motor3in1('A', 0, tim5);
	Pin motor3in2('A', 1, tim5);
	motor3in1.pwmInit(2048, 1, 1);
	motor3in2.pwmInit(2048, 1, 2);
	Motor motor3(motor3in1, motor3in2);
	
	Pin motor4in1('E', 11, tim1);
	Pin motor4in2('E', 13, tim1);
	motor4in1.pwmInit(2048, 1, 2);
	motor4in2.pwmInit(2048, 1, 3);
	Motor motor4(motor4in1, motor4in2);
	
	omniplatform omni(motor3, motor4, motor2, motor1);
	int t = time_service::millis();
	bool targetSetted = false;
	volatile int16_t pow;
	volatile bool aharey = true;
	volatile float targetAngle;
	
	while (true) {
		gyro.read();
		
		if (!targetSetted && time_service::millis() - t > 6000) {
			gyro.setTarget(gyro.getCurrentAngle());
			targetAngle = gyro.getTarget();
			targetSetted = true;
			t = time_service::millis();
		} else if (targetSetted) {
			if (time_service::millis() - t >= 5) {
				targetAngle += 0.5;
				t = time_service::millis();
			}
			
			if (targetAngle > 180) targetAngle = -180;
			
			gyro.setTarget(targetAngle);
			aharey = false;
			pow = gyro.setRotationForTarget();
			float ang = 90 + gyro.getCurrentAngle();
			if (ang < 0) ang += 360;
			else if (ang > 180) ang -= 360;
			
			omni.move(100, 15, ang, pow, 0.05 * gyro.getMaxRotation());
			//omni.move(100, 25, 90, pow, 0.8*gyro.getMaxRotation());
			//omni.move(100, 25, );
		}
		
		//omni.move(0, 0, 0, 250, 300);
	}

	return 0;
}