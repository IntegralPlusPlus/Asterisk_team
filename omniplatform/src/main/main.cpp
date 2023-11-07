#include "Pin.h"
//#include "usable.h"
#include "usart2.h"
#include "adc.h"
#include "dma.h"
#include "time_service.h"
#include "project_config.h"
#include "Motor.h"
#include "SPI_1.h"
#include "SPI_DOT.h"
#include "mpu9250_spi.h"
#include "IMU_SPI.h"

int main() {
	Pin tx_uart('A', 2, usart2);
	Pin rx_uart('A', 3, usart2);
	uart2::usart2Init(9600, 1, 8);
	
	Pin diode('A', 1, write);
	diode.setBit();
	time_service::init();
	time_service::startTime();
  time_service::delay(500);
	
	Pin spi2_sck('B', 13, spi2);
  Pin spi2_mosi('B', 15, spi2);
  Pin spi2_miso('B', 14, spi2);
  Pin spi2_nss('B', 12, write_pupd_down);
	
	mpu9250_spi sensor_mpu(spi2_nss);
	IMU mpu(sensor_mpu);
	
	mpu.init();
	
	time_service::delay(100);
	mpu.update();
	bool isntCalibrated = true;
	volatile double yaw, yawOld = 1000, time, timeOld = 0;
	uint8_t countMPU = 0;
	
	while (1) {
		mpu.update();
		yaw = mpu.getAngle();
		time = time_service::millis();
		
		uart2::write((uint8_t)abs(yaw));
		//uart2::write('\n');
		
		if (abs((yaw - yawOld) / double(time - timeOld)) < 0.05 && isntCalibrated) {
			diode.resetBit();
			//uart2::write('C');
			//uart2::write('A');
			//uart2::write('L');
			//uart2::write('I');
			//uart2::write('B');
			//uart2::write('R');
			//uart2::write('A');
			//uart2::write('T');
			//uart2::write('E');
			//uart2::write('\n');
			
			mpu.calibrate(2000);
			mpu.setZeroAngle();
			isntCalibrated = false;
		}
		//time_service::delay(1);
		
		//volatile double kk = sensor_mpu.yaw;
		//mpu.setZeroAngle();
		if (countMPU == 255) {
			yawOld = yaw;
			timeOld = time;
		} else countMPU = 0;
	}
	
	return 0;
}