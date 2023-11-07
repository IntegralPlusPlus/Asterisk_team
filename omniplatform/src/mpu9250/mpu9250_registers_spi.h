#ifndef mpu9250_registers
#define mpu9250_registers


////////////////////// GYROSCOPE AND ACCELEROMETER /////////////////////
#define SELF_TEST_X_GYRO		0
#define	SELF_TEST_Y_GYRO		1
#define	SELF_TEST_Z_GYRO		2
#define	SELF_TEST_X_ACCEL		13
#define	SELF_TEST_Y_ACCEL		14
#define	SELF_TEST_Z_ACCEL		15
#define	XG_OFFSET_H					19
#define	XG_OFFSET_L					20
#define	YG_OFFSET_H					21
#define	YG_OFFSET_L					22
#define	ZG_OFFSET_H					23
#define	ZG_OFFSET_L					24
#define	SMPLRT_DIV					25
#define	CONFIG							26
#define	GYRO_CONFIG 				27
#define	ACCEL_CONFIG				28
#define	ACCEL_CONFIG_2			29
#define	LP_ACCEL_ODR				30
#define	WOM_THR							31
#define	FIFO_EN							35
#define	I2C_MST_CTRL				36
#define	I2C_SLV0_ADDR				37
#define	I2C_SLV0_REG				38
#define	I2C_SLV0_CTRL				39
#define	I2C_SLV1_ADDR				40
#define	I2C_SLV1_REG				41
#define	I2C_SLV1_CTRL				42
#define	I2C_SLV2_ADDR				43
#define	I2C_SLV2_REG				44
#define	I2C_SLV2_CTRL				45
#define	I2C_SLV3_ADDR				46
#define	I2C_SLV3_REG				47
#define	I2C_SLV3_CTRL				48
#define	I2C_SLV4_ADDR				49
#define	I2C_SLV4_REG				50
#define	I2C_SLV4_DO					51
#define	I2C_SLV4_CTRL				52
#define	I2C_SLV4_DI					53
#define	I2C_MST_STATUS			54
#define	INT_PIN_CFG					55
#define	INT_ENABLE					56
#define	INT_STATUS					58
#define	ACCEL_XOUT_H				59
#define	ACCEL_XOUT_L				60
#define	ACCEL_YOUT_H				61
#define	ACCEL_YOUT_L				62
#define	ACCEL_ZOUT_H				63
#define	ACCEL_ZOUT_L				64
#define	TEMP_OUT_H					65
#define	TEMP_OUT_L					66
#define	GYRO_XOUT_H					67
#define	GYRO_XOUT_L					68
#define	GYRO_YOUT_H					69
#define	GYRO_YOUT_L					70
#define	GYRO_ZOUT_H					71
#define	GYRO_ZOUT_L					72
#define EXT_SENS_DATA_00		73
#define EXT_SENS_DATA_01		74
#define EXT_SENS_DATA_02		75
#define EXT_SENS_DATA_03		76
#define EXT_SENS_DATA_04		77
#define EXT_SENS_DATA_05		78
#define EXT_SENS_DATA_06		79
#define EXT_SENS_DATA_07		80
#define EXT_SENS_DATA_08		81
#define EXT_SENS_DATA_09		82
#define EXT_SENS_DATA_10		83
#define EXT_SENS_DATA_11		84
#define EXT_SENS_DATA_12		85
#define EXT_SENS_DATA_13		86
#define EXT_SENS_DATA_14		87
#define EXT_SENS_DATA_15		88
#define EXT_SENS_DATA_16		89
#define EXT_SENS_DATA_17		90
#define EXT_SENS_DATA_18		91
#define EXT_SENS_DATA_19		92
#define EXT_SENS_DATA_20		93
#define EXT_SENS_DATA_21		94
#define EXT_SENS_DATA_22		95
#define EXT_SENS_DATA_23		96
#define	I2C_SLV0_DO					99
#define	I2C_SLV1_DO					100
#define	I2C_SLV2_DO					101
#define	I2C_SLV3_DO					102
#define	I2C_MST_DELAY_CTRL	103
#define	SIGNAL_PATH_RESET		104
#define	MOT_DETECT_CTRL			105
#define	USER_CTRL						106
#define	PWR_MGMT_1					107
#define	PWR_MGMT_2					108
#define	FIFO_COUNTH					114
#define	FIFO_COUNTL					115
#define	FIFO_R_W						116
#define	WHO_AM_I						117
#define	XA_OFFSET_H					119
#define	XA_OFFSET_L					120
#define	YA_OFFSET_H					122
#define	YA_OFFSET_L					123
#define	ZA_OFFSET_H					125
#define	ZA_OFFSET_L					126
////////////////////////////////////////////////////////////////////////

///////////////////////////CONFIGURATION BITS///////////////////////////

//CONFIG
#define	FIFO_MODE					64
#define	EXT_SYNC_SET			56
#define	DPLF_CFG					7

//INT_PIN_CFG
#define	ACTL							128
#define	OPEN							64
#define	LATCH_INT_EN			32
#define	INT_ANYRD_2CLEAR	16
#define	ACTL_FSYNC				8
#define	FSYNC_INT_MODE_EN	4
#define	BYPASS_EN					2

//INT_ENABLE
#define	WOM_EN						64
#define FIFO_OFLOW_EN			8
#define	FSYNC_INT_EN			4
#define	RAW_RDY_EN				1

//INT_STATUS
#define	WOM_INT						64
#define	FIFO_OFLOW_INT		16
#define	FSYNC_INT					8
#define	RAW_DATA_RDY_INT	1

//USER_CTRL
#define FIFO_EN_BIT				64
#define I2C_MST_EN				32
#define I2C_IF_DIS				16
#define FIFO_RST					4
#define I2C_MST_RST				2
#define SIG_CONT_RST			1

//PWR_MGMT_1
#define H_RESET						128
#define SLEEP							64
#define CYCLE							32
#define GYRO_STANDBY			16
#define PD_PTAT						8
#define CLKSEL_INT_OSC		0
#define CLKSEL_BEST_SRC		1
#define CLKSEL_STOP_CLK		7

//PWR_MGMT_2
#define	DIS_XA						32
#define	DIS_YA						16
#define	DIS_ZA						8
#define	DIS_XG						4
#define	DIS_YG						2
#define	DIS_ZG						1

////////////////////////////////////////////////////////////////////////



///////////////////////////// MAGNETOMETER /////////////////////////////
#define AK8963_I2C_ADDR             0x0C
#define AK8963_Device_ID            0x48
#define	AK8963_WIA									0x00
#define	AK8963_INFO									0x01
#define	AK8963_ST1									0x02
#define	AK8963_HXL									0x03
#define	AK8963_HXH									0x04
#define	AK8963_HYL									0x05
#define	AK8963_HYH									0x06
#define	AK8963_HZL									0x07
#define	AK8963_HZH									0x08
#define	AK8963_ST2									0x09
#define	AK8963_CNTL1								0x0A
#define	AK8963_CNTL2								0x0B
#define	AK8963_ASTC									0x0C
#define AK8963_TS1                  0x0D
#define AK8963_TS2                  0x0E
#define	AK8963_I2CDIS								0x0F
#define	AK8963_ASAX									0x10
#define	AK8963_ASAY									0x11
#define	AK8963_ASAZ									0x12
////////////////////////////////////////////////////////////////////////



#endif
