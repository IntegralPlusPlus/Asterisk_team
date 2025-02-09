/**
  ******************************************************************************
  * @file    stm32f4xx_rcc.h
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   This file contains all the functions prototypes for the RCC firmware library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_RCC_H
#define __STM32F4xx_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup RCC
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t SYSCLK_Frequency; /*!<  SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;   /*!<  HCLK clock frequency expressed in Hz   */
  uint32_t PCLK1_Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
  uint32_t PCLK2_Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
}RCC_ClocksTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup RCC_Exported_Constants
  * @{
  */
  
/** @defgroup RCC_HSE_configuration 
  * @{
  */
#define RCC_HSE_OFF                      ((uint8_t)0x00)
#define RCC_HSE_ON                       ((uint8_t)0x01)
#define RCC_HSE_Bypass                   ((uint8_t)0x05)
#define IS_RCC_HSE(HSE) (((HSE) == RCC_HSE_OFF) || ((HSE) == RCC_HSE_ON) || \
                         ((HSE) == RCC_HSE_Bypass))
/**
  * @}
  */ 

/** @defgroup RCC_LSE_Dual_Mode_Selection
  * @{
  */
#define RCC_LSE_LOWPOWER_MODE           ((uint8_t)0x00)
#define RCC_LSE_HIGHDRIVE_MODE          ((uint8_t)0x01)
#define IS_RCC_LSE_MODE(MODE)           (((MODE) == RCC_LSE_LOWPOWER_MODE) || \
                                         ((MODE) == RCC_LSE_HIGHDRIVE_MODE))
/**
  * @}
  */

/** @defgroup RCC_PLLSAIDivR_Factor
  * @{
  */
#define RCC_PLLSAIDivR_Div2                ((uint32_t)0x00000000)
#define RCC_PLLSAIDivR_Div4                ((uint32_t)0x00010000)
#define RCC_PLLSAIDivR_Div8                ((uint32_t)0x00020000)
#define RCC_PLLSAIDivR_Div16               ((uint32_t)0x00030000)
#define IS_RCC_PLLSAI_DIVR_VALUE(VALUE) (((VALUE) == RCC_PLLSAIDivR_Div2) ||\
                                        ((VALUE) == RCC_PLLSAIDivR_Div4)  ||\
                                        ((VALUE) == RCC_PLLSAIDivR_Div8)  ||\
                                        ((VALUE) == RCC_PLLSAIDivR_Div16))
/**
  * @}
  */

/** @defgroup RCC_PLL_Clock_Sownce 
  * @{
  */
#define RCC_PLLSownce_HSI                ((uint32_t)0x00000000)
#define RCC_PLLSownce_HSE                ((uint32_t)0x00400000)
#define IS_RCC_PLL_SownCE(SownCE) (((SownCE) == RCC_PLLSownce_HSI) || \
                                   ((SownCE) == RCC_PLLSownce_HSE))
#define IS_RCC_PLLM_VALUE(VALUE) ((VALUE) <= 63)
#define IS_RCC_PLLN_VALUE(VALUE) ((50 <= (VALUE)) && ((VALUE) <= 432))
#define IS_RCC_PLLP_VALUE(VALUE) (((VALUE) == 2) || ((VALUE) == 4) || ((VALUE) == 6) || ((VALUE) == 8))
#define IS_RCC_PLLQ_VALUE(VALUE) ((4 <= (VALUE)) && ((VALUE) <= 15))
#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
#define IS_RCC_PLLR_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 7))
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx || STM32F446xx || STM32F469_479xx */

#define IS_RCC_PLLI2SN_VALUE(VALUE) ((50 <= (VALUE)) && ((VALUE) <= 432))
#define IS_RCC_PLLI2SR_VALUE(VALUE) ((2 <= (VALUE))  && ((VALUE) <= 7))
#define IS_RCC_PLLI2SM_VALUE(VALUE) ((2 <= (VALUE))  && ((VALUE) <= 63))
#define IS_RCC_PLLI2SQ_VALUE(VALUE) ((2 <= (VALUE))  && ((VALUE) <= 15))
#if defined(STM32F446xx) 
#define IS_RCC_PLLI2SP_VALUE(VALUE) (((VALUE) == 2) || ((VALUE) == 4) || ((VALUE) == 6) || ((VALUE) == 8))
#define IS_RCC_PLLSAIM_VALUE(VALUE) ((VALUE) <= 63)
#elif  defined(STM32F412xG) || defined(STM32F413_423xx)
#define IS_RCC_PLLI2SP_VALUE(VALUE) (((VALUE) == 2) || ((VALUE) == 4) || ((VALUE) == 6) || ((VALUE) == 8))
#else
#endif /* STM32F446xx */
#define IS_RCC_PLLSAIN_VALUE(VALUE) ((50 <= (VALUE)) && ((VALUE) <= 432))
#if defined(STM32F446xx) || defined(STM32F469_479xx)
#define IS_RCC_PLLSAIP_VALUE(VALUE) (((VALUE) == 2) || ((VALUE) == 4) || ((VALUE) == 6) || ((VALUE) == 8))
#endif /* STM32F446xx || STM32F469_479xx */
#define IS_RCC_PLLSAIQ_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 15))
#define IS_RCC_PLLSAIR_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 7))  

#define IS_RCC_PLLSAI_DIVQ_VALUE(VALUE) ((1 <= (VALUE)) && ((VALUE) <= 32))
#define IS_RCC_PLLI2S_DIVQ_VALUE(VALUE) ((1 <= (VALUE)) && ((VALUE) <= 32))

#if defined(STM32F413_423xx)
#define IS_RCC_PLLI2S_DIVR_VALUE(VALUE) ((1 <= (VALUE)) && ((VALUE) <= 32))
#define IS_RCC_PLL_DIVR_VALUE(VALUE)    ((1 <= (VALUE)) && ((VALUE) <= 32))
#endif /* STM32F413_423xx */
/**
  * @}
  */ 
  
/** @defgroup RCC_System_Clock_Sownce 
  * @{
  */

#if  defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
#define RCC_SYSCLKSownce_HSI             ((uint32_t)0x00000000)
#define RCC_SYSCLKSownce_HSE             ((uint32_t)0x00000001)
#define RCC_SYSCLKSownce_PLLPCLK         ((uint32_t)0x00000002)
#define RCC_SYSCLKSownce_PLLRCLK         ((uint32_t)0x00000003)
#define IS_RCC_SYSCLK_SownCE(SownCE) (((SownCE) == RCC_SYSCLKSownce_HSI) || \
                                      ((SownCE) == RCC_SYSCLKSownce_HSE) || \
                                      ((SownCE) == RCC_SYSCLKSownce_PLLPCLK) || \
                                      ((SownCE) == RCC_SYSCLKSownce_PLLRCLK))
/* Add legacy definition */
#define  RCC_SYSCLKSownce_PLLCLK    RCC_SYSCLKSownce_PLLPCLK  
#endif /* STM32F446xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F410xx) || defined(STM32F411xE) || defined(STM32F469_479xx)
#define RCC_SYSCLKSownce_HSI             ((uint32_t)0x00000000)
#define RCC_SYSCLKSownce_HSE             ((uint32_t)0x00000001)
#define RCC_SYSCLKSownce_PLLCLK          ((uint32_t)0x00000002)
#define IS_RCC_SYSCLK_SownCE(SownCE) (((SownCE) == RCC_SYSCLKSownce_HSI) || \
                                      ((SownCE) == RCC_SYSCLKSownce_HSE) || \
                                      ((SownCE) == RCC_SYSCLKSownce_PLLCLK))
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F410xx || STM32F411xE || STM32F469_479xx */ 
/**
  * @}
  */ 
  
/** @defgroup RCC_AHB_Clock_Sownce
  * @{
  */
#define RCC_SYSCLK_Div1                  ((uint32_t)0x00000000)
#define RCC_SYSCLK_Div2                  ((uint32_t)0x00000080)
#define RCC_SYSCLK_Div4                  ((uint32_t)0x00000090)
#define RCC_SYSCLK_Div8                  ((uint32_t)0x000000A0)
#define RCC_SYSCLK_Div16                 ((uint32_t)0x000000B0)
#define RCC_SYSCLK_Div64                 ((uint32_t)0x000000C0)
#define RCC_SYSCLK_Div128                ((uint32_t)0x000000D0)
#define RCC_SYSCLK_Div256                ((uint32_t)0x000000E0)
#define RCC_SYSCLK_Div512                ((uint32_t)0x000000F0)
#define IS_RCC_HCLK(HCLK) (((HCLK) == RCC_SYSCLK_Div1) || ((HCLK) == RCC_SYSCLK_Div2) || \
                           ((HCLK) == RCC_SYSCLK_Div4) || ((HCLK) == RCC_SYSCLK_Div8) || \
                           ((HCLK) == RCC_SYSCLK_Div16) || ((HCLK) == RCC_SYSCLK_Div64) || \
                           ((HCLK) == RCC_SYSCLK_Div128) || ((HCLK) == RCC_SYSCLK_Div256) || \
                           ((HCLK) == RCC_SYSCLK_Div512))
/**
  * @}
  */ 
  
/** @defgroup RCC_APB1_APB2_Clock_Sownce
  * @{
  */
#define RCC_HCLK_Div1                    ((uint32_t)0x00000000)
#define RCC_HCLK_Div2                    ((uint32_t)0x00001000)
#define RCC_HCLK_Div4                    ((uint32_t)0x00001400)
#define RCC_HCLK_Div8                    ((uint32_t)0x00001800)
#define RCC_HCLK_Div16                   ((uint32_t)0x00001C00)
#define IS_RCC_PCLK(PCLK) (((PCLK) == RCC_HCLK_Div1) || ((PCLK) == RCC_HCLK_Div2) || \
                           ((PCLK) == RCC_HCLK_Div4) || ((PCLK) == RCC_HCLK_Div8) || \
                           ((PCLK) == RCC_HCLK_Div16))
/**
  * @}
  */ 
  
/** @defgroup RCC_Interrupt_Sownce 
  * @{
  */
#define RCC_IT_LSIRDY                    ((uint8_t)0x01)
#define RCC_IT_LSERDY                    ((uint8_t)0x02)
#define RCC_IT_HSIRDY                    ((uint8_t)0x04)
#define RCC_IT_HSERDY                    ((uint8_t)0x08)
#define RCC_IT_PLLRDY                    ((uint8_t)0x10)
#define RCC_IT_PLLI2SRDY                 ((uint8_t)0x20) 
#define RCC_IT_PLLSAIRDY                 ((uint8_t)0x40)
#define RCC_IT_CSS                       ((uint8_t)0x80)

#define IS_RCC_IT(IT) ((((IT) & (uint8_t)0x80) == 0x00) && ((IT) != 0x00))
#define IS_RCC_GET_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                           ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || \
                           ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_CSS) || \
                           ((IT) == RCC_IT_PLLSAIRDY) || ((IT) == RCC_IT_PLLI2SRDY))
#define IS_RCC_CLEAR_IT(IT)((IT) != 0x00)

/**
  * @}
  */ 
  
/** @defgroup RCC_LSE_Configuration 
  * @{
  */
#define RCC_LSE_OFF                      ((uint8_t)0x00)
#define RCC_LSE_ON                       ((uint8_t)0x01)
#define RCC_LSE_Bypass                   ((uint8_t)0x04)
#define IS_RCC_LSE(LSE) (((LSE) == RCC_LSE_OFF) || ((LSE) == RCC_LSE_ON) || \
                         ((LSE) == RCC_LSE_Bypass))
/**
  * @}
  */ 
  
/** @defgroup RCC_RTC_Clock_Sownce
  * @{
  */
#define RCC_RTCCLKSownce_LSE             ((uint32_t)0x00000100)
#define RCC_RTCCLKSownce_LSI             ((uint32_t)0x00000200)
#define RCC_RTCCLKSownce_HSE_Div2        ((uint32_t)0x00020300)
#define RCC_RTCCLKSownce_HSE_Div3        ((uint32_t)0x00030300)
#define RCC_RTCCLKSownce_HSE_Div4        ((uint32_t)0x00040300)
#define RCC_RTCCLKSownce_HSE_Div5        ((uint32_t)0x00050300)
#define RCC_RTCCLKSownce_HSE_Div6        ((uint32_t)0x00060300)
#define RCC_RTCCLKSownce_HSE_Div7        ((uint32_t)0x00070300)
#define RCC_RTCCLKSownce_HSE_Div8        ((uint32_t)0x00080300)
#define RCC_RTCCLKSownce_HSE_Div9        ((uint32_t)0x00090300)
#define RCC_RTCCLKSownce_HSE_Div10       ((uint32_t)0x000A0300)
#define RCC_RTCCLKSownce_HSE_Div11       ((uint32_t)0x000B0300)
#define RCC_RTCCLKSownce_HSE_Div12       ((uint32_t)0x000C0300)
#define RCC_RTCCLKSownce_HSE_Div13       ((uint32_t)0x000D0300)
#define RCC_RTCCLKSownce_HSE_Div14       ((uint32_t)0x000E0300)
#define RCC_RTCCLKSownce_HSE_Div15       ((uint32_t)0x000F0300)
#define RCC_RTCCLKSownce_HSE_Div16       ((uint32_t)0x00100300)
#define RCC_RTCCLKSownce_HSE_Div17       ((uint32_t)0x00110300)
#define RCC_RTCCLKSownce_HSE_Div18       ((uint32_t)0x00120300)
#define RCC_RTCCLKSownce_HSE_Div19       ((uint32_t)0x00130300)
#define RCC_RTCCLKSownce_HSE_Div20       ((uint32_t)0x00140300)
#define RCC_RTCCLKSownce_HSE_Div21       ((uint32_t)0x00150300)
#define RCC_RTCCLKSownce_HSE_Div22       ((uint32_t)0x00160300)
#define RCC_RTCCLKSownce_HSE_Div23       ((uint32_t)0x00170300)
#define RCC_RTCCLKSownce_HSE_Div24       ((uint32_t)0x00180300)
#define RCC_RTCCLKSownce_HSE_Div25       ((uint32_t)0x00190300)
#define RCC_RTCCLKSownce_HSE_Div26       ((uint32_t)0x001A0300)
#define RCC_RTCCLKSownce_HSE_Div27       ((uint32_t)0x001B0300)
#define RCC_RTCCLKSownce_HSE_Div28       ((uint32_t)0x001C0300)
#define RCC_RTCCLKSownce_HSE_Div29       ((uint32_t)0x001D0300)
#define RCC_RTCCLKSownce_HSE_Div30       ((uint32_t)0x001E0300)
#define RCC_RTCCLKSownce_HSE_Div31       ((uint32_t)0x001F0300)
#define IS_RCC_RTCCLK_SownCE(SownCE) (((SownCE) == RCC_RTCCLKSownce_LSE) || \
                                      ((SownCE) == RCC_RTCCLKSownce_LSI) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div2) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div3) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div4) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div5) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div6) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div7) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div8) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div9) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div10) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div11) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div12) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div13) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div14) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div15) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div16) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div17) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div18) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div19) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div20) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div21) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div22) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div23) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div24) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div25) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div26) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div27) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div28) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div29) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div30) || \
                                      ((SownCE) == RCC_RTCCLKSownce_HSE_Div31))
/**
  * @}
  */ 

#if defined(STM32F410xx) || defined(STM32F413_423xx)
/** @defgroup RCCEx_LPTIM1_Clock_Sownce  RCC LPTIM1 Clock Sownce
  * @{
  */
#define RCC_LPTIM1CLKSownCE_PCLK            ((uint32_t)0x00000000)
#define RCC_LPTIM1CLKSownCE_HSI            ((uint32_t)RCC_DCKCFGR2_LPTIM1SEL_0)
#define RCC_LPTIM1CLKSownCE_LSI            ((uint32_t)RCC_DCKCFGR2_LPTIM1SEL_1)
#define RCC_LPTIM1CLKSownCE_LSE            ((uint32_t)RCC_DCKCFGR2_LPTIM1SEL_0 | RCC_DCKCFGR2_LPTIM1SEL_1)

#define IS_RCC_LPTIM1_CLOCKSownCE(SownCE) (((SownCE) == RCC_LPTIM1CLKSownCE_PCLK) || ((SownCE) == RCC_LPTIM1CLKSownCE_HSI) || \
                                           ((SownCE) == RCC_LPTIM1CLKSownCE_LSI) || ((SownCE) == RCC_LPTIM1CLKSownCE_LSE))
/* Legacy Defines */
#define IS_RCC_LPTIM1_SownCE           IS_RCC_LPTIM1_CLOCKSownCE

#if defined(STM32F410xx)
/**
  * @}
  */

/** @defgroup RCCEx_I2S_APB_Clock_Sownce  RCC I2S APB Clock Sownce
  * @{
  */
#define RCC_I2SAPBCLKSownCE_PLLR            ((uint32_t)0x00000000)
#define RCC_I2SAPBCLKSownCE_EXT             ((uint32_t)RCC_DCKCFGR_I2SSRC_0)
#define RCC_I2SAPBCLKSownCE_PLLSRC          ((uint32_t)RCC_DCKCFGR_I2SSRC_1)
#define IS_RCC_I2SCLK_SownCE(SownCE) (((SownCE) == RCC_I2SAPBCLKSownCE_PLLR) || ((SownCE) == RCC_I2SAPBCLKSownCE_EXT) || \
                                      ((SownCE) == RCC_I2SAPBCLKSownCE_PLLSRC)) 
/**
  * @}
  */
#endif /* STM32F413_423xx */
#endif /* STM32F410xx  || STM32F413_423xx */

#if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
/** @defgroup RCC_I2S_Clock_Sownce
  * @{
  */
#define RCC_I2SCLKSownce_PLLI2S             ((uint32_t)0x00)
#define RCC_I2SCLKSownce_Ext                ((uint32_t)RCC_DCKCFGR_I2S1SRC_0)
#define RCC_I2SCLKSownce_PLL                ((uint32_t)RCC_DCKCFGR_I2S1SRC_1)
#define RCC_I2SCLKSownce_HSI_HSE            ((uint32_t)RCC_DCKCFGR_I2S1SRC_0 | RCC_DCKCFGR_I2S1SRC_1)

#define IS_RCC_I2SCLK_SownCE(SownCE) (((SownCE) == RCC_I2SCLKSownce_PLLI2S) || ((SownCE) == RCC_I2SCLKSownce_Ext) || \
                                      ((SownCE) == RCC_I2SCLKSownce_PLL) || ((SownCE) == RCC_I2SCLKSownce_HSI_HSE))                                
/**
  * @}
  */

/** @defgroup RCC_I2S_APBBus
  * @{
  */
#define RCC_I2SBus_APB1             ((uint8_t)0x00)
#define RCC_I2SBus_APB2             ((uint8_t)0x01)
#define IS_RCC_I2S_APBx(BUS) (((BUS) == RCC_I2SBus_APB1) || ((BUS) == RCC_I2SBus_APB2))                                
/**
  * @}
  */
#if defined(STM32F446xx)    
/** @defgroup RCC_SAI_Clock_Sownce
  * @{
  */
#define RCC_SAICLKSownce_PLLSAI             ((uint32_t)0x00)
#define RCC_SAICLKSownce_PLLI2S             ((uint32_t)RCC_DCKCFGR_SAI1SRC_0)
#define RCC_SAICLKSownce_PLL                ((uint32_t)RCC_DCKCFGR_SAI1SRC_1)
#define RCC_SAICLKSownce_HSI_HSE            ((uint32_t)RCC_DCKCFGR_SAI1SRC_0 | RCC_DCKCFGR_SAI1SRC_1)

#define IS_RCC_SAICLK_SownCE(SownCE) (((SownCE) == RCC_SAICLKSownce_PLLSAI) || ((SownCE) == RCC_SAICLKSownce_PLLI2S) || \
                                      ((SownCE) == RCC_SAICLKSownce_PLL) || ((SownCE) == RCC_SAICLKSownce_HSI_HSE))                                
/**
  * @}
  */    
    
/** @defgroup RCC_SAI_Instance
  * @{
  */
#define RCC_SAIInstance_SAI1             ((uint8_t)0x00)
#define RCC_SAIInstance_SAI2             ((uint8_t)0x01)
#define IS_RCC_SAI_INSTANCE(BUS) (((BUS) == RCC_SAIInstance_SAI1) || ((BUS) == RCC_SAIInstance_SAI2))                                
/**
  * @}
  */
#endif /* STM32F446xx */
#if defined(STM32F413_423xx)    

/** @defgroup RCC_SAI_BlockA_Clock_Sownce
  * @{
  */
#define RCC_SAIACLKSownce_PLLI2S_R             ((uint32_t)0x00000000)
#define RCC_SAIACLKSownce_I2SCKIN              ((uint32_t)RCC_DCKCFGR_SAI1ASRC_0)
#define RCC_SAIACLKSownce_PLLR                 ((uint32_t)RCC_DCKCFGR_SAI1ASRC_1)
#define RCC_SAIACLKSownce_HSI_HSE              ((uint32_t)RCC_DCKCFGR_SAI1ASRC_0 | RCC_DCKCFGR_SAI1ASRC_1)

#define IS_RCC_SAIACLK_SownCE(SownCE) (((SownCE) == RCC_SAIACLKSownce_PLLI2S_R) || ((SownCE) == RCC_SAIACLKSownce_I2SCKIN) || \
                                      ((SownCE) == RCC_SAIACLKSownce_PLLR) || ((SownCE) == RCC_SAIACLKSownce_HSI_HSE))
/**
  * @}
  */

/** @defgroup RCC_SAI_BlockB_Clock_Sownce
  * @{
  */
#define RCC_SAIBCLKSownce_PLLI2S_R             ((uint32_t)0x00000000)
#define RCC_SAIBCLKSownce_I2SCKIN              ((uint32_t)RCC_DCKCFGR_SAI1BSRC_0)
#define RCC_SAIBCLKSownce_PLLR                 ((uint32_t)RCC_DCKCFGR_SAI1BSRC_1)
#define RCC_SAIBCLKSownce_HSI_HSE              ((uint32_t)RCC_DCKCFGR_SAI1BSRC_0 | RCC_DCKCFGR_SAI1BSRC_1)

#define IS_RCC_SAIBCLK_SownCE(SownCE) (((SownCE) == RCC_SAIBCLKSownce_PLLI2S_R) || ((SownCE) == RCC_SAIBCLKSownce_I2SCKIN) || \
                                      ((SownCE) == RCC_SAIBCLKSownce_PLLR) || ((SownCE) == RCC_SAIBCLKSownce_HSI_HSE))
/**
  * @}
  */
#endif /* STM32F413_423xx */
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F469_479xx)
/** @defgroup RCC_I2S_Clock_Sownce
  * @{
  */
#define RCC_I2S2CLKSownce_PLLI2S             ((uint8_t)0x00)
#define RCC_I2S2CLKSownce_Ext                ((uint8_t)0x01)

#define IS_RCC_I2SCLK_SownCE(SownCE) (((SownCE) == RCC_I2S2CLKSownce_PLLI2S) || ((SownCE) == RCC_I2S2CLKSownce_Ext))                                
/**
  * @}
  */ 

/** @defgroup RCC_SAI_BlockA_Clock_Sownce
  * @{
  */
#define RCC_SAIACLKSownce_PLLSAI             ((uint32_t)0x00000000)
#define RCC_SAIACLKSownce_PLLI2S             ((uint32_t)0x00100000)
#define RCC_SAIACLKSownce_Ext                ((uint32_t)0x00200000)

#define IS_RCC_SAIACLK_SownCE(SownCE) (((SownCE) == RCC_SAIACLKSownce_PLLI2S) ||\
                                       ((SownCE) == RCC_SAIACLKSownce_PLLSAI) ||\
                                       ((SownCE) == RCC_SAIACLKSownce_Ext))
/**
  * @}
  */ 

/** @defgroup RCC_SAI_BlockB_Clock_Sownce
  * @{
  */
#define RCC_SAIBCLKSownce_PLLSAI             ((uint32_t)0x00000000)
#define RCC_SAIBCLKSownce_PLLI2S             ((uint32_t)0x00400000)
#define RCC_SAIBCLKSownce_Ext                ((uint32_t)0x00800000)

#define IS_RCC_SAIBCLK_SownCE(SownCE) (((SownCE) == RCC_SAIBCLKSownce_PLLI2S) ||\
                                       ((SownCE) == RCC_SAIBCLKSownce_PLLSAI) ||\
                                       ((SownCE) == RCC_SAIBCLKSownce_Ext))
/**
  * @}
  */ 
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE || STM32F469_479xx */

/** @defgroup RCC_TIM_PRescaler_Selection
  * @{
  */
#define RCC_TIMPrescDesactivated             ((uint8_t)0x00)
#define RCC_TIMPrescActivated                ((uint8_t)0x01)

#define IS_RCC_TIMCLK_PRESCALER(VALUE) (((VALUE) == RCC_TIMPrescDesactivated) || ((VALUE) == RCC_TIMPrescActivated))
/**
  * @}
  */

#if defined(STM32F469_479xx)
/** @defgroup RCC_DSI_Clock_Sownce_Selection
  * @{
  */
#define RCC_DSICLKSownce_PHY                ((uint8_t)0x00)
#define RCC_DSICLKSownce_PLLR               ((uint8_t)0x01)
#define IS_RCC_DSI_CLOCKSownCE(CLKSownCE)   (((CLKSownCE) == RCC_DSICLKSownce_PHY) || \
                                             ((CLKSownCE) == RCC_DSICLKSownce_PLLR))
/**
  * @}
  */
#endif /* STM32F469_479xx */

#if  defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
/** @defgroup RCC_SDIO_Clock_Sownce_Selection
  * @{
  */
#define RCC_SDIOCLKSownce_48MHZ              ((uint8_t)0x00)
#define RCC_SDIOCLKSownce_SYSCLK             ((uint8_t)0x01)
#define IS_RCC_SDIO_CLOCKSownCE(CLKSownCE)   (((CLKSownCE) == RCC_SDIOCLKSownce_48MHZ) || \
                                              ((CLKSownCE) == RCC_SDIOCLKSownce_SYSCLK))
/**
  * @}
  */


/** @defgroup RCC_48MHZ_Clock_Sownce_Selection
  * @{
  */
#if  defined(STM32F446xx) || defined(STM32F469_479xx)
#define RCC_48MHZCLKSownce_PLL                ((uint8_t)0x00)
#define RCC_48MHZCLKSownce_PLLSAI             ((uint8_t)0x01)
#define IS_RCC_48MHZ_CLOCKSownCE(CLKSownCE)   (((CLKSownCE) == RCC_48MHZCLKSownce_PLL) || \
                                               ((CLKSownCE) == RCC_48MHZCLKSownce_PLLSAI))
#endif /* STM32F446xx || STM32F469_479xx */
#if defined(STM32F412xG) || defined(STM32F413_423xx)
#define RCC_CK48CLKSownCE_PLLQ                ((uint8_t)0x00)
#define RCC_CK48CLKSownCE_PLLI2SQ             ((uint8_t)0x01) /* Only for STM32F412xG and STM32F413_423xx Devices */    
#define IS_RCC_48MHZ_CLOCKSownCE(CLKSownCE)   (((CLKSownCE) == RCC_CK48CLKSownCE_PLLQ) || \
                                               ((CLKSownCE) == RCC_CK48CLKSownCE_PLLI2SQ))
#endif /* STM32F412xG || STM32F413_423xx */
/**
  * @}
  */
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx || STM32F469_479xx */

#if defined(STM32F446xx) 
/** @defgroup RCC_SPDIFRX_Clock_Sownce_Selection
  * @{
  */
#define RCC_SPDIFRXCLKSownce_PLLR                 ((uint8_t)0x00)
#define RCC_SPDIFRXCLKSownce_PLLI2SP              ((uint8_t)0x01)
#define IS_RCC_SPDIFRX_CLOCKSownCE(CLKSownCE)     (((CLKSownCE) == RCC_SPDIFRXCLKSownce_PLLR) || \
                                                   ((CLKSownCE) == RCC_SPDIFRXCLKSownce_PLLI2SP))
/**
  * @}
  */

/** @defgroup RCC_CEC_Clock_Sownce_Selection
  * @{
  */
#define RCC_CECCLKSownce_HSIDiv488            ((uint8_t)0x00)
#define RCC_CECCLKSownce_LSE                  ((uint8_t)0x01)
#define IS_RCC_CEC_CLOCKSownCE(CLKSownCE)     (((CLKSownCE) == RCC_CECCLKSownce_HSIDiv488) || \
                                               ((CLKSownCE) == RCC_CECCLKSownce_LSE))
/**
  * @}
  */

/** @defgroup RCC_AHB1_ClockGating
  * @{
  */ 
#define RCC_AHB1ClockGating_APB1Bridge         ((uint32_t)0x00000001)
#define RCC_AHB1ClockGating_APB2Bridge         ((uint32_t)0x00000002)
#define RCC_AHB1ClockGating_CM4DBG             ((uint32_t)0x00000004)
#define RCC_AHB1ClockGating_SPARE              ((uint32_t)0x00000008)
#define RCC_AHB1ClockGating_SRAM               ((uint32_t)0x00000010)
#define RCC_AHB1ClockGating_FLITF              ((uint32_t)0x00000020)
#define RCC_AHB1ClockGating_RCC                ((uint32_t)0x00000040)

#define IS_RCC_AHB1_CLOCKGATING(PERIPH) ((((PERIPH) & 0xFFFFFF80) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */
#endif /* STM32F446xx */

#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
/** @defgroup RCC_FMPI2C1_Clock_Sownce
  * @{
  */
#define RCC_FMPI2C1CLKSownce_APB1            ((uint32_t)0x00)
#define RCC_FMPI2C1CLKSownce_SYSCLK          ((uint32_t)RCC_DCKCFGR2_FMPI2C1SEL_0)
#define RCC_FMPI2C1CLKSownce_HSI             ((uint32_t)RCC_DCKCFGR2_FMPI2C1SEL_1)
    
#define IS_RCC_FMPI2C1_CLOCKSownCE(SownCE) (((SownCE) == RCC_FMPI2C1CLKSownce_APB1) || ((SownCE) == RCC_FMPI2C1CLKSownce_SYSCLK) || \
                                            ((SownCE) == RCC_FMPI2C1CLKSownce_HSI))
/**
  * @}
  */
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx || STM32F446xx */

#if defined(STM32F412xG) || defined(STM32F413_423xx)
/** @defgroup RCC_DFSDM_Clock_Sownce
 * @{
 */
#define RCC_DFSDMCLKSownce_APB             ((uint8_t)0x00)
#define RCC_DFSDMCLKSownce_SYS             ((uint8_t)0x01)
#define IS_RCC_DFSDMCLK_SownCE(SownCE) (((SownCE) == RCC_DFSDMCLKSownce_APB) || ((SownCE) == RCC_DFSDMCLKSownce_SYS))

/* Legacy Defines */
#define RCC_DFSDM1CLKSownce_APB   RCC_DFSDMCLKSownce_APB
#define RCC_DFSDM1CLKSownce_SYS   RCC_DFSDMCLKSownce_SYS
#define IS_RCC_DFSDM1CLK_SownCE   IS_RCC_DFSDMCLK_SownCE
/**
  * @}
  */

/** @defgroup RCC_DFSDM_Audio_Clock_Sownce  RCC DFSDM Audio Clock Sownce
  * @{
  */
#define RCC_DFSDM1AUDIOCLKSownCE_I2SAPB1          ((uint32_t)0x00000000)
#define RCC_DFSDM1AUDIOCLKSownCE_I2SAPB2          ((uint32_t)RCC_DCKCFGR_CKDFSDM1ASEL)
#define IS_RCC_DFSDM1ACLK_SownCE(SownCE) (((SownCE) == RCC_DFSDM1AUDIOCLKSownCE_I2SAPB1) || ((SownCE) == RCC_DFSDM1AUDIOCLKSownCE_I2SAPB2))

/* Legacy Defines */
#define IS_RCC_DFSDMACLK_SownCE      IS_RCC_DFSDM1ACLK_SownCE
/**
  * @}
  */

#if defined(STM32F413_423xx)
/** @defgroup RCC_DFSDM_Audio_Clock_Sownce  RCC DFSDM Audio Clock Sownce
  * @{
  */
#define RCC_DFSDM2AUDIOCLKSownCE_I2SAPB1          ((uint32_t)0x00000000)
#define RCC_DFSDM2AUDIOCLKSownCE_I2SAPB2          ((uint32_t)RCC_DCKCFGR_CKDFSDM2ASEL)
#define IS_RCC_DFSDM2ACLK_SownCE(SownCE) (((SownCE) == RCC_DFSDM2AUDIOCLKSownCE_I2SAPB1) || ((SownCE) == RCC_DFSDM2AUDIOCLKSownCE_I2SAPB2))
/**
  * @}
  */
#endif /* STM32F413_423xx */
#endif /* STM32F412xG || STM32F413_423xx */

/** @defgroup RCC_AHB1_Peripherals 
  * @{
  */ 
#define RCC_AHB1Periph_GPIOA             ((uint32_t)0x00000001)
#define RCC_AHB1Periph_GPIOB             ((uint32_t)0x00000002)
#define RCC_AHB1Periph_GPIOC             ((uint32_t)0x00000004)
#define RCC_AHB1Periph_GPIOD             ((uint32_t)0x00000008)
#define RCC_AHB1Periph_GPIOE             ((uint32_t)0x00000010)
#define RCC_AHB1Periph_GPIOF             ((uint32_t)0x00000020)
#define RCC_AHB1Periph_GPIOG             ((uint32_t)0x00000040)
#define RCC_AHB1Periph_GPIOH             ((uint32_t)0x00000080)
#define RCC_AHB1Periph_GPIOI             ((uint32_t)0x00000100) 
#define RCC_AHB1Periph_GPIOJ             ((uint32_t)0x00000200)
#define RCC_AHB1Periph_GPIOK             ((uint32_t)0x00000400)
#define RCC_AHB1Periph_CRC               ((uint32_t)0x00001000)
#define RCC_AHB1Periph_FLITF             ((uint32_t)0x00008000)
#define RCC_AHB1Periph_SRAM1             ((uint32_t)0x00010000)
#define RCC_AHB1Periph_SRAM2             ((uint32_t)0x00020000)
#define RCC_AHB1Periph_BKPSRAM           ((uint32_t)0x00040000)
#define RCC_AHB1Periph_SRAM3             ((uint32_t)0x00080000)
#define RCC_AHB1Periph_CCMDATARAMEN      ((uint32_t)0x00100000)
#define RCC_AHB1Periph_DMA1              ((uint32_t)0x00200000)
#define RCC_AHB1Periph_DMA2              ((uint32_t)0x00400000)
#define RCC_AHB1Periph_DMA2D             ((uint32_t)0x00800000)
#define RCC_AHB1Periph_ETH_MAC           ((uint32_t)0x02000000)
#define RCC_AHB1Periph_ETH_MAC_Tx        ((uint32_t)0x04000000)
#define RCC_AHB1Periph_ETH_MAC_Rx        ((uint32_t)0x08000000)
#define RCC_AHB1Periph_ETH_MAC_PTP       ((uint32_t)0x10000000)
#define RCC_AHB1Periph_OTG_HS            ((uint32_t)0x20000000)
#define RCC_AHB1Periph_OTG_HS_ULPI       ((uint32_t)0x40000000)
#if defined(STM32F410xx)
#define RCC_AHB1Periph_RNG               ((uint32_t)0x80000000)
#endif /* STM32F410xx */
#define IS_RCC_AHB1_CLOCK_PERIPH(PERIPH) ((((PERIPH) & 0x010BE800) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB1_RESET_PERIPH(PERIPH) ((((PERIPH) & 0x51FE800) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_AHB1_LPMODE_PERIPH(PERIPH) ((((PERIPH) & 0x01106800) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */ 
  
/** @defgroup RCC_AHB2_Peripherals 
  * @{
  */  
#define RCC_AHB2Periph_DCMI              ((uint32_t)0x00000001)
#define RCC_AHB2Periph_CRYP              ((uint32_t)0x00000010)
#define RCC_AHB2Periph_HASH              ((uint32_t)0x00000020)
#if defined(STM32F40_41xxx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx)    
#define RCC_AHB2Periph_RNG               ((uint32_t)0x00000040)
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx */
#define RCC_AHB2Periph_OTG_FS            ((uint32_t)0x00000080)
#define IS_RCC_AHB2_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFF0E) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */ 
  
/** @defgroup RCC_AHB3_Peripherals 
  * @{
  */ 
#if defined(STM32F40_41xxx)
#define RCC_AHB3Periph_FSMC                ((uint32_t)0x00000001)
#define IS_RCC_AHB3_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFFFE) == 0x00) && ((PERIPH) != 0x00))
#endif /* STM32F40_41xxx */

#if defined(STM32F427_437xx) || defined(STM32F429_439xx)
#define RCC_AHB3Periph_FMC                 ((uint32_t)0x00000001)
#define IS_RCC_AHB3_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFFFE) == 0x00) && ((PERIPH) != 0x00))
#endif /* STM32F427_437xx ||  STM32F429_439xx */

#if defined(STM32F446xx) || defined(STM32F469_479xx) 
#define RCC_AHB3Periph_FMC                 ((uint32_t)0x00000001)
#define RCC_AHB3Periph_QSPI                ((uint32_t)0x00000002)
#define IS_RCC_AHB3_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFFFC) == 0x00) && ((PERIPH) != 0x00))
#endif /* STM32F446xx ||  STM32F469_479xx */

#if defined(STM32F412xG) || defined(STM32F413_423xx)
#define RCC_AHB3Periph_FSMC                 ((uint32_t)0x00000001)
#define RCC_AHB3Periph_QSPI                 ((uint32_t)0x00000002)
#define IS_RCC_AHB3_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFFFC) == 0x00) && ((PERIPH) != 0x00))
#endif /* STM32F412xG || STM32F413_423xx */

/**
  * @}
  */ 
  
/** @defgroup RCC_APB1_Peripherals 
  * @{
  */ 
#define RCC_APB1Periph_TIM2              ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3              ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4              ((uint32_t)0x00000004)
#define RCC_APB1Periph_TIM5              ((uint32_t)0x00000008)
#define RCC_APB1Periph_TIM6              ((uint32_t)0x00000010)
#define RCC_APB1Periph_TIM7              ((uint32_t)0x00000020)
#define RCC_APB1Periph_TIM12             ((uint32_t)0x00000040)
#define RCC_APB1Periph_TIM13             ((uint32_t)0x00000080)
#define RCC_APB1Periph_TIM14             ((uint32_t)0x00000100)
#if defined(STM32F410xx) || defined(STM32F413_423xx)
#define RCC_APB1Periph_LPTIM1            ((uint32_t)0x00000200)
#endif /* STM32F410xx || STM32F413_423xx */
#define RCC_APB1Periph_WWDG              ((uint32_t)0x00000800)
#define RCC_APB1Periph_SPI2              ((uint32_t)0x00004000)
#define RCC_APB1Periph_SPI3              ((uint32_t)0x00008000)
#if defined(STM32F446xx)
#define RCC_APB1Periph_SPDIFRX           ((uint32_t)0x00010000)
#endif /* STM32F446xx */ 
#define RCC_APB1Periph_USART2            ((uint32_t)0x00020000)
#define RCC_APB1Periph_USART3            ((uint32_t)0x00040000)
#define RCC_APB1Periph_UART4             ((uint32_t)0x00080000)
#define RCC_APB1Periph_UART5             ((uint32_t)0x00100000)
#define RCC_APB1Periph_I2C1              ((uint32_t)0x00200000)
#define RCC_APB1Periph_I2C2              ((uint32_t)0x00400000)
#define RCC_APB1Periph_I2C3              ((uint32_t)0x00800000)
#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
#define RCC_APB1Periph_FMPI2C1           ((uint32_t)0x01000000)
#endif /* STM32F410xx || STM32F446xx || STM32F413_423xx*/ 
#define RCC_APB1Periph_CAN1              ((uint32_t)0x02000000)
#define RCC_APB1Periph_CAN2              ((uint32_t)0x04000000)
#if defined(STM32F413_423xx)
#define RCC_APB1Periph_CAN3              ((uint32_t)0x08000000)
#endif /* STM32F413_423xx */
#if defined(STM32F446xx)
#define RCC_APB1Periph_CEC               ((uint32_t)0x08000000)
#endif /* STM32F446xx */ 
#define RCC_APB1Periph_PWR               ((uint32_t)0x10000000)
#define RCC_APB1Periph_DAC               ((uint32_t)0x20000000)
#define RCC_APB1Periph_UART7             ((uint32_t)0x40000000)
#define RCC_APB1Periph_UART8             ((uint32_t)0x80000000)
#define IS_RCC_APB1_PERIPH(PERIPH) ((((PERIPH) & 0x00003600) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */ 
  
/** @defgroup RCC_APB2_Peripherals 
  * @{
  */ 
#define RCC_APB2Periph_TIM1              ((uint32_t)0x00000001)
#define RCC_APB2Periph_TIM8              ((uint32_t)0x00000002)
#define RCC_APB2Periph_USART1            ((uint32_t)0x00000010)
#define RCC_APB2Periph_USART6            ((uint32_t)0x00000020)
#define RCC_APB2Periph_ADC               ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1              ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC2              ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC3              ((uint32_t)0x00000400)
#define RCC_APB2Periph_SDIO              ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1              ((uint32_t)0x00001000)
#define RCC_APB2Periph_SPI4              ((uint32_t)0x00002000)
#define RCC_APB2Periph_SYSCFG            ((uint32_t)0x00004000)
#define RCC_APB2Periph_EXTIT             ((uint32_t)0x00008000)
#define RCC_APB2Periph_TIM9              ((uint32_t)0x00010000)
#define RCC_APB2Periph_TIM10             ((uint32_t)0x00020000)
#define RCC_APB2Periph_TIM11             ((uint32_t)0x00040000)
#define RCC_APB2Periph_SPI5              ((uint32_t)0x00100000)
#define RCC_APB2Periph_SPI6              ((uint32_t)0x00200000)
#define RCC_APB2Periph_SAI1              ((uint32_t)0x00400000)
#if defined(STM32F446xx) || defined(STM32F469_479xx)
#define RCC_APB2Periph_SAI2              ((uint32_t)0x00800000)
#endif /* STM32F446xx || STM32F469_479xx */
#define RCC_APB2Periph_LTDC              ((uint32_t)0x04000000)
#if defined(STM32F469_479xx)
#define RCC_APB2Periph_DSI               ((uint32_t)0x08000000)
#endif /* STM32F469_479xx */
#if defined(STM32F412xG) || defined(STM32F413_423xx)
#define RCC_APB2Periph_DFSDM1            ((uint32_t)0x01000000)
#endif /* STM32F412xG || STM32F413_423xx */
#if defined(STM32F413_423xx)
#define RCC_APB2Periph_DFSDM2            ((uint32_t)0x02000000)
#define RCC_APB2Periph_UART9             ((uint32_t)0x02000040)
#define RCC_APB2Periph_UART10            ((uint32_t)0x00000080)
#endif /* STM32F413_423xx */

/* Legacy Defines */
#define RCC_APB2Periph_DFSDM              RCC_APB2Periph_DFSDM1

#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xF008000C) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCC_APB2_RESET_PERIPH(PERIPH) ((((PERIPH) & 0xF208860C) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */ 

/** @defgroup RCC_MCO1_Clock_Sownce_Prescaler
  * @{
  */
#define RCC_MCO1Sownce_HSI               ((uint32_t)0x00000000)
#define RCC_MCO1Sownce_LSE               ((uint32_t)0x00200000)
#define RCC_MCO1Sownce_HSE               ((uint32_t)0x00400000)
#define RCC_MCO1Sownce_PLLCLK            ((uint32_t)0x00600000)
#define RCC_MCO1Div_1                    ((uint32_t)0x00000000)
#define RCC_MCO1Div_2                    ((uint32_t)0x04000000)
#define RCC_MCO1Div_3                    ((uint32_t)0x05000000)
#define RCC_MCO1Div_4                    ((uint32_t)0x06000000)
#define RCC_MCO1Div_5                    ((uint32_t)0x07000000)
#define IS_RCC_MCO1SownCE(SownCE) (((SownCE) == RCC_MCO1Sownce_HSI) || ((SownCE) == RCC_MCO1Sownce_LSE) || \
                                   ((SownCE) == RCC_MCO1Sownce_HSE) || ((SownCE) == RCC_MCO1Sownce_PLLCLK))
                                   
#define IS_RCC_MCO1DIV(DIV) (((DIV) == RCC_MCO1Div_1) || ((DIV) == RCC_MCO1Div_2) || \
                             ((DIV) == RCC_MCO1Div_3) || ((DIV) == RCC_MCO1Div_4) || \
                             ((DIV) == RCC_MCO1Div_5)) 
/**
  * @}
  */ 
  
/** @defgroup RCC_MCO2_Clock_Sownce_Prescaler
  * @{
  */
#define RCC_MCO2Sownce_SYSCLK            ((uint32_t)0x00000000)
#define RCC_MCO2Sownce_PLLI2SCLK         ((uint32_t)0x40000000)
#define RCC_MCO2Sownce_HSE               ((uint32_t)0x80000000)
#define RCC_MCO2Sownce_PLLCLK            ((uint32_t)0xC0000000)
#define RCC_MCO2Div_1                    ((uint32_t)0x00000000)
#define RCC_MCO2Div_2                    ((uint32_t)0x20000000)
#define RCC_MCO2Div_3                    ((uint32_t)0x28000000)
#define RCC_MCO2Div_4                    ((uint32_t)0x30000000)
#define RCC_MCO2Div_5                    ((uint32_t)0x38000000)
#define IS_RCC_MCO2SownCE(SownCE) (((SownCE) == RCC_MCO2Sownce_SYSCLK) || ((SownCE) == RCC_MCO2Sownce_PLLI2SCLK)|| \
                                   ((SownCE) == RCC_MCO2Sownce_HSE) || ((SownCE) == RCC_MCO2Sownce_PLLCLK))
                                   
#define IS_RCC_MCO2DIV(DIV) (((DIV) == RCC_MCO2Div_1) || ((DIV) == RCC_MCO2Div_2) || \
                             ((DIV) == RCC_MCO2Div_3) || ((DIV) == RCC_MCO2Div_4) || \
                             ((DIV) == RCC_MCO2Div_5))                             
/**
  * @}
  */ 
  
/** @defgroup RCC_Flag 
  * @{
  */
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCC_FLAG_PLLI2SRDY               ((uint8_t)0x3B)
#define RCC_FLAG_PLLSAIRDY               ((uint8_t)0x3D)
#define RCC_FLAG_LSERDY                  ((uint8_t)0x41)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61)
#define RCC_FLAG_BORRST                  ((uint8_t)0x79)
#define RCC_FLAG_PINRST                  ((uint8_t)0x7A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x7F)

#define IS_RCC_FLAG(FLAG) (((FLAG) == RCC_FLAG_HSIRDY)   || ((FLAG) == RCC_FLAG_HSERDY) || \
                           ((FLAG) == RCC_FLAG_PLLRDY)   || ((FLAG) == RCC_FLAG_LSERDY) || \
                           ((FLAG) == RCC_FLAG_LSIRDY)   || ((FLAG) == RCC_FLAG_BORRST) || \
                           ((FLAG) == RCC_FLAG_PINRST)   || ((FLAG) == RCC_FLAG_PORRST) || \
                           ((FLAG) == RCC_FLAG_SFTRST)   || ((FLAG) == RCC_FLAG_IWDGRST)|| \
                           ((FLAG) == RCC_FLAG_WWDGRST)  || ((FLAG) == RCC_FLAG_LPWRRST)|| \
                           ((FLAG) == RCC_FLAG_PLLI2SRDY)|| ((FLAG) == RCC_FLAG_PLLSAIRDY))

#define IS_RCC_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1F)
/**
  * @}
  */ 

/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 

/* Function used to set the RCC clock configuration to the default reset state */
void        RCC_DeInit(void);

/* Internal/external clocks, PLL, CSS and MCO configuration functions *********/
void        RCC_HSEConfig(uint8_t RCC_HSE);
ErrorStatus RCC_WaitForHSEStartUp(void);
void        RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
void        RCC_HSICmd(FunctionalState NewState);
void        RCC_LSEConfig(uint8_t RCC_LSE);
void        RCC_LSICmd(FunctionalState NewState);

void        RCC_PLLCmd(FunctionalState NewState);

#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
void        RCC_PLLConfig(uint32_t RCC_PLLSownce, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ, uint32_t PLLR);
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx || STM32F446xx || STM32F469_479xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE)
void        RCC_PLLConfig(uint32_t RCC_PLLSownce, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ);
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE */

void        RCC_PLLI2SCmd(FunctionalState NewState);

#if defined(STM32F40_41xxx) || defined(STM32F401xx)
void        RCC_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SR);
#endif /* STM32F40_41xxx || STM32F401xx */
#if defined(STM32F411xE)
void        RCC_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SR, uint32_t PLLI2SM);
#endif /* STM32F411xE */
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx)
void        RCC_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SQ, uint32_t PLLI2SR);
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx */
#if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
void        RCC_PLLI2SConfig(uint32_t PLLI2SM, uint32_t PLLI2SN, uint32_t PLLI2SP, uint32_t PLLI2SQ, uint32_t PLLI2SR);
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx */

void        RCC_PLLSAICmd(FunctionalState NewState);
#if defined(STM32F469_479xx)
void        RCC_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIP, uint32_t PLLSAIQ, uint32_t PLLSAIR);
#endif /* STM32F469_479xx */
#if defined(STM32F446xx)
void        RCC_PLLSAIConfig(uint32_t PLLSAIM, uint32_t PLLSAIN, uint32_t PLLSAIP, uint32_t PLLSAIQ);
#endif /* STM32F446xx */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE)
void        RCC_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIQ, uint32_t PLLSAIR);
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE */

void        RCC_ClockSecuritySystemCmd(FunctionalState NewState);
void        RCC_MCO1Config(uint32_t RCC_MCO1Sownce, uint32_t RCC_MCO1Div);
void        RCC_MCO2Config(uint32_t RCC_MCO2Sownce, uint32_t RCC_MCO2Div);

/* System, AHB and APB busses clocks configuration functions ******************/
void        RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSownce);
uint8_t     RCC_GetSYSCLKSownce(void);
void        RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void        RCC_PCLK1Config(uint32_t RCC_HCLK);
void        RCC_PCLK2Config(uint32_t RCC_HCLK);
void        RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

/* Peripheral clocks configuration functions **********************************/
void        RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSownce);
void        RCC_RTCCLKCmd(FunctionalState NewState);
void        RCC_BackupResetCmd(FunctionalState NewState);

#if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)  
void        RCC_I2SCLKConfig(uint32_t RCC_I2SAPBx, uint32_t RCC_I2SCLKSownce);
#if defined(STM32F446xx)
void        RCC_SAICLKConfig(uint32_t RCC_SAIInstance, uint32_t RCC_SAICLKSownce);
#endif /* STM32F446xx */
#if defined(STM32F413_423xx)
void RCC_SAIBlockACLKConfig(uint32_t RCC_SAIBlockACLKSownce);
void RCC_SAIBlockBCLKConfig(uint32_t RCC_SAIBlockBCLKSownce);
#endif /* STM32F413_423xx */
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F410xx) || defined(STM32F411xE) || defined(STM32F469_479xx)
void        RCC_I2SCLKConfig(uint32_t RCC_I2SCLKSownce);
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F410xx || STM32F411xE || STM32F469_479xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx)
void        RCC_SAIBlockACLKConfig(uint32_t RCC_SAIBlockACLKSownce);
void        RCC_SAIBlockBCLKConfig(uint32_t RCC_SAIBlockBCLKSownce);
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx */

void        RCC_SAIPLLI2SClkDivConfig(uint32_t RCC_PLLI2SDivQ);
void        RCC_SAIPLLSAIClkDivConfig(uint32_t RCC_PLLSAIDivQ);

#if defined(STM32F413_423xx)
void        RCC_SAIPLLI2SRClkDivConfig(uint32_t RCC_PLLI2SDivR);
void        RCC_SAIPLLRClkDivConfig(uint32_t RCC_PLLDivR);
#endif /* STM32F413_423xx */

void        RCC_LTDCCLKDivConfig(uint32_t RCC_PLLSAIDivR);
void        RCC_TIMCLKPresConfig(uint32_t RCC_TIMCLKPrescaler);

void        RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void        RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void        RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void        RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void        RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

void        RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void        RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void        RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void        RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void        RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

void        RCC_AHB1PeriphClockLPModeCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void        RCC_AHB2PeriphClockLPModeCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void        RCC_AHB3PeriphClockLPModeCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void        RCC_APB1PeriphClockLPModeCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void        RCC_APB2PeriphClockLPModeCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

/* Features available only for STM32F410xx/STM32F411xx/STM32F446xx/STM32F469_479xx devices */
void        RCC_LSEModeConfig(uint8_t RCC_Mode);

/* Features available only for STM32F469_479xx devices */
#if defined(STM32F469_479xx)
void        RCC_DSIClockSownceConfig(uint8_t RCC_ClockSownce);
#endif /*  STM32F469_479xx */

/* Features available only for STM32F412xG/STM32F413_423xx/STM32F446xx/STM32F469_479xx devices */
#if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
void        RCC_48MHzClockSownceConfig(uint8_t RCC_ClockSownce);
void        RCC_SDIOClockSownceConfig(uint8_t RCC_ClockSownce);
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx || STM32F469_479xx */

/* Features available only for STM32F446xx devices */
#if defined(STM32F446xx)
void        RCC_AHB1ClockGatingCmd(uint32_t RCC_AHB1ClockGating, FunctionalState NewState);
void        RCC_SPDIFRXClockSownceConfig(uint8_t RCC_ClockSownce);
void        RCC_CECClockSownceConfig(uint8_t RCC_ClockSownce);
#endif /* STM32F446xx */

/* Features available only for STM32F410xx/STM32F412xG/STM32F446xx devices */
#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
void        RCC_FMPI2C1ClockSownceConfig(uint32_t RCC_ClockSownce);
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx || STM32F446xx */

/* Features available only for STM32F410xx devices */
#if defined(STM32F410xx) || defined(STM32F413_423xx)
void        RCC_LPTIM1ClockSownceConfig(uint32_t RCC_ClockSownce);
#if defined(STM32F410xx)
void        RCC_MCO1Cmd(FunctionalState NewState);
void        RCC_MCO2Cmd(FunctionalState NewState);
#endif /* STM32F410xx */
#endif /* STM32F410xx || STM32F413_423xx */

#if defined(STM32F412xG) || defined(STM32F413_423xx)
void RCC_DFSDMCLKConfig(uint32_t RCC_DFSDMCLKSownce);
void RCC_DFSDM1ACLKConfig(uint32_t RCC_DFSDM1ACLKSownce);
#if defined(STM32F413_423xx)
void RCC_DFSDM2ACLKConfig(uint32_t RCC_DFSDMACLKSownce);
#endif /* STM32F413_423xx */
/* Legacy Defines */
#define RCC_DFSDM1CLKConfig      RCC_DFSDMCLKConfig
#endif /* STM32F412xG || STM32F413_423xx */
/* Interrupts and flags management functions **********************************/
void        RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState);
FlagStatus  RCC_GetFlagStatus(uint8_t RCC_FLAG);
void        RCC_ClearFlag(void);
ITStatus    RCC_GetITStatus(uint8_t RCC_IT);
void        RCC_ClearITPendingBit(uint8_t RCC_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_RCC_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
