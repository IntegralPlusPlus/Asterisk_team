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
#include "TSOP.h"
#include "usable.h"

int main() 
{
	time_service::init();
	time_service::startTime();
  time_service::delay(100);
	
	Pin w1('C', 0, write_pupd_down);
	Pin w2('C', 1, write_pupd_down);
	Pin w3('C', 2, write_pupd_down);
	Pin w4('C', 3, write_pupd_down);
	Pin mux1Input('A', 4, read_pupd_down);
	Pin mux2Input('A', 5, read_pupd_down);
	TSOP tsops(mux1Input, mux2Input, w1, w2, w3, w4);
	volatile int16_t tsopAngle;
	
	while (true) {
		bool seeBall = tsops.updateTSOPs();
		
		if (seeBall) {
			tsops.calculateAngle();
			tsopAngle = tsops.getAngle();
		}
	}
	
	return 0;
}