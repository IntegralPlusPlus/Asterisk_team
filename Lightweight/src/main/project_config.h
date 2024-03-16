#include "global_macros.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_flash.h"
#include "math.h"
#include "string.h"

#define BLUE_GOAL 0
#define YELLOW_GOAL 1
#define GOALKEEPER_ROLE 0
#define FORWARD_ROLE 1
#define D_MODE 0
#define P_MODE 1