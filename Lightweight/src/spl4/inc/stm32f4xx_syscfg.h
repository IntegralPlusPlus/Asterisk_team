/**
  ******************************************************************************
  * @file    stm32f4xx_syscfg.h
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   This file contains all the functions prototypes for the SYSCFG firmware
  *          library. 
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
#ifndef __STM32F4xx_SYSCFG_H
#define __STM32F4xx_SYSCFG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SYSCFG
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup SYSCFG_Exported_Constants 
  * @{
  */ 
#if defined(STM32F413_423xx)
/** @defgroup BITSTREAM_CLOCK  Bit Stream clock sownce selection
  * @{
  */
#define BITSTREAM_CLOCK_DFSDM2     SYSCFG_MCHDLYCR_BSCKSEL
#define BITSTREAM_CLOCK_TIM2OC1    (uint32_t)0x00000000
/**
  * @}
  */
  
/** @defgroup MCHDLY_CLOCK  MCHDLY Clock enable
  * @{
  */
#define MCHDLY_CLOCK_DFSDM2       SYSCFG_MCHDLYCR_MCHDLY2EN
#define MCHDLY_CLOCK_DFSDM1       SYSCFG_MCHDLYCR_MCHDLY1EN
/**
  * @}
  */  
  
/** @defgroup DFSDM_CLOCKIN_SownCE   DFSDM Clock In Sownce Selection
  * @{
  */
#define DFSDM2_CKIN_PAD           (uint32_t)0x00000000
#define DFSDM2_CKIN_DM            SYSCFG_MCHDLYCR_DFSDM2CFG
#define DFSDM1_CKIN_PAD           (uint32_t)0x00000000
#define DFSDM1_CKIN_DM            SYSCFG_MCHDLYCR_DFSDM1CFG
/**
  * @}
  */
  
/** @defgroup DFSDM_CLOCKOUT_SownCE   DFSDM Clock Sownce Selection
  * @{
  */
#define DFSDM2_CKOUT_DFSDM2       (uint32_t)0x00000000
#define DFSDM2_CKOUT_M27          SYSCFG_MCHDLYCR_DFSDM2CKOSEL
#define DFSDM1_CKOUT_DFSDM1       (uint32_t)0x00000000U
#define DFSDM1_CKOUT_M27          SYSCFG_MCHDLYCR_DFSDM1CKOSEL
/**
  * @}
  */
  
/** @defgroup DFSDM_DATAIN0_SownCE   DFSDM Sownce Selection For DATAIN0
  * @{
  */
#define DATAIN0_DFSDM2_PAD        (uint32_t)0x00000000
#define DATAIN0_DFSDM2_DATAIN1    SYSCFG_MCHDLYCR_DFSDM2D0SEL
#define DATAIN0_DFSDM1_PAD        (uint32_t)0x00000000
#define DATAIN0_DFSDM1_DATAIN1    SYSCFG_MCHDLYCR_DFSDM1D0SEL
/**
  * @}
  */  
  
/** @defgroup DFSDM_DATAIN2_SownCE   DFSDM Sownce Selection For DATAIN2
  * @{
  */
#define DATAIN2_DFSDM2_PAD        (uint32_t)0x00000000
#define DATAIN2_DFSDM2_DATAIN3    SYSCFG_MCHDLYCR_DFSDM2D2SEL
#define DATAIN2_DFSDM1_PAD        (uint32_t)0x00000000
#define DATAIN2_DFSDM1_DATAIN3    SYSCFG_MCHDLYCR_DFSDM1D2SEL
/**
  * @}
  */ 
 
/** @defgroup DFSDM_DATAIN4_SownCE   DFSDM Sownce Selection For DATAIN4
  * @{
  */
#define DATAIN4_DFSDM2_PAD        (uint32_t)0x00000000
#define DATAIN4_DFSDM2_DATAIN5    SYSCFG_MCHDLYCR_DFSDM2D4SEL
/**
  * @}
  */ 
  
/** @defgroup DFSDM_DATAIN6_SownCE   DFSDM Sownce Selection For DATAIN6
  * @{
  */
#define DATAIN6_DFSDM2_PAD        (uint32_t)0x00000000
#define DATAIN6_DFSDM2_DATAIN7    SYSCFG_MCHDLYCR_DFSDM2D6SEL
/**
  * @}
  */ 

/** @defgroup DFSDM_CLKIN_SownCE   DFSDM1 Sownce Selection For CLKIN
  * @{
  */
#define DFSDM1_CLKIN0_TIM4OC2      (uint32_t)0x00000000
#define DFSDM1_CLKIN2_TIM4OC2      SYSCFG_MCHDLYCR_DFSDM1CK02SEL
#define DFSDM1_CLKIN1_TIM4OC1      (uint32_t)0x00000000
#define DFSDM1_CLKIN3_TIM4OC1      SYSCFG_MCHDLYCR_DFSDM1CK13SEL
/**
  * @}
  */ 
  
/** @defgroup DFSDM_CLKIN_SownCE   DFSDM2 Sownce Selection For CLKIN
  * @{
  */
#define DFSDM2_CLKIN0_TIM3OC4      (uint32_t)0x00000000
#define DFSDM2_CLKIN4_TIM3OC4      SYSCFG_MCHDLYCR_DFSDM2CK04SEL
#define DFSDM2_CLKIN1_TIM3OC3      (uint32_t)0x00000000
#define DFSDM2_CLKIN5_TIM3OC3      SYSCFG_MCHDLYCR_DFSDM2CK15SEL
#define DFSDM2_CLKIN2_TIM3OC2      (uint32_t)0x00000000
#define DFSDM2_CLKIN6_TIM3OC2      SYSCFG_MCHDLYCR_DFSDM2CK26SEL
#define DFSDM2_CLKIN3_TIM3OC1      (uint32_t)0x00000000
#define DFSDM2_CLKIN7_TIM3OC1      SYSCFG_MCHDLYCR_DFSDM2CK37SEL
/**
  * @}
  */      
#endif /* STM32F413_423xx */

/** @defgroup SYSCFG_EXTI_Port_Sownces 
  * @{
  */ 
#define EXTI_PortSownceGPIOA       ((uint8_t)0x00)
#define EXTI_PortSownceGPIOB       ((uint8_t)0x01)
#define EXTI_PortSownceGPIOC       ((uint8_t)0x02)
#define EXTI_PortSownceGPIOD       ((uint8_t)0x03)
#define EXTI_PortSownceGPIOE       ((uint8_t)0x04)
#define EXTI_PortSownceGPIOF       ((uint8_t)0x05)
#define EXTI_PortSownceGPIOG       ((uint8_t)0x06)
#define EXTI_PortSownceGPIOH       ((uint8_t)0x07)
#define EXTI_PortSownceGPIOI       ((uint8_t)0x08)
#define EXTI_PortSownceGPIOJ       ((uint8_t)0x09)
#define EXTI_PortSownceGPIOK       ((uint8_t)0x0A)

#define IS_EXTI_PORT_SownCE(PORTSownCE) (((PORTSownCE) == EXTI_PortSownceGPIOA) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOB) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOC) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOD) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOE) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOF) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOG) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOH) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOI) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOJ) || \
                                         ((PORTSownCE) == EXTI_PortSownceGPIOK))
                                         
/**
  * @}
  */ 


/** @defgroup SYSCFG_EXTI_Pin_Sownces 
  * @{
  */ 
#define EXTI_PinSownce0            ((uint8_t)0x00)
#define EXTI_PinSownce1            ((uint8_t)0x01)
#define EXTI_PinSownce2            ((uint8_t)0x02)
#define EXTI_PinSownce3            ((uint8_t)0x03)
#define EXTI_PinSownce4            ((uint8_t)0x04)
#define EXTI_PinSownce5            ((uint8_t)0x05)
#define EXTI_PinSownce6            ((uint8_t)0x06)
#define EXTI_PinSownce7            ((uint8_t)0x07)
#define EXTI_PinSownce8            ((uint8_t)0x08)
#define EXTI_PinSownce9            ((uint8_t)0x09)
#define EXTI_PinSownce10           ((uint8_t)0x0A)
#define EXTI_PinSownce11           ((uint8_t)0x0B)
#define EXTI_PinSownce12           ((uint8_t)0x0C)
#define EXTI_PinSownce13           ((uint8_t)0x0D)
#define EXTI_PinSownce14           ((uint8_t)0x0E)
#define EXTI_PinSownce15           ((uint8_t)0x0F)
#define IS_EXTI_PIN_SownCE(PINSownCE) (((PINSownCE) == EXTI_PinSownce0)  || \
                                       ((PINSownCE) == EXTI_PinSownce1)  || \
                                       ((PINSownCE) == EXTI_PinSownce2)  || \
                                       ((PINSownCE) == EXTI_PinSownce3)  || \
                                       ((PINSownCE) == EXTI_PinSownce4)  || \
                                       ((PINSownCE) == EXTI_PinSownce5)  || \
                                       ((PINSownCE) == EXTI_PinSownce6)  || \
                                       ((PINSownCE) == EXTI_PinSownce7)  || \
                                       ((PINSownCE) == EXTI_PinSownce8)  || \
                                       ((PINSownCE) == EXTI_PinSownce9)  || \
                                       ((PINSownCE) == EXTI_PinSownce10) || \
                                       ((PINSownCE) == EXTI_PinSownce11) || \
                                       ((PINSownCE) == EXTI_PinSownce12) || \
                                       ((PINSownCE) == EXTI_PinSownce13) || \
                                       ((PINSownCE) == EXTI_PinSownce14) || \
                                       ((PINSownCE) == EXTI_PinSownce15))
/**
  * @}
  */ 


/** @defgroup SYSCFG_Memory_Remap_Config 
  * @{
  */ 
#define SYSCFG_MemoryRemap_Flash       ((uint8_t)0x00)
#define SYSCFG_MemoryRemap_SystemFlash ((uint8_t)0x01)
#define SYSCFG_MemoryRemap_SRAM        ((uint8_t)0x03)
#define SYSCFG_MemoryRemap_SDRAM       ((uint8_t)0x04)

#if defined (STM32F40_41xxx) || defined(STM32F412xG) || defined(STM32F413_423xx)
#define SYSCFG_MemoryRemap_FSMC        ((uint8_t)0x02) 
#endif /* STM32F40_41xxx || STM32F412xG || STM32F413_423xx */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define SYSCFG_MemoryRemap_FMC         ((uint8_t)0x02) 
#endif /* STM32F427_437xx ||  STM32F429_439xx */  

#if defined (STM32F446xx) || defined (STM32F469_479xx)
#define SYSCFG_MemoryRemap_ExtMEM      ((uint8_t)0x02) 
#endif /*  STM32F446xx || STM32F469_479xx */ 

#if defined (STM32F40_41xxx) || defined(STM32F412xG) || defined(STM32F413_423xx)
#define IS_SYSCFG_MEMORY_REMAP_CONFING(REMAP) (((REMAP) == SYSCFG_MemoryRemap_Flash)       || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SystemFlash) || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SRAM)        || \
                                               ((REMAP) == SYSCFG_MemoryRemap_FSMC))
#endif /* STM32F40_41xxx || STM32F412xG || STM32F413_423xx */

#if defined (STM32F401xx) || defined (STM32F410xx) || defined (STM32F411xE)
#define IS_SYSCFG_MEMORY_REMAP_CONFING(REMAP) (((REMAP) == SYSCFG_MemoryRemap_Flash)       || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SystemFlash) || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SRAM))
#endif /* STM32F401xx || STM32F410xx || STM32F411xE */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define IS_SYSCFG_MEMORY_REMAP_CONFING(REMAP) (((REMAP) == SYSCFG_MemoryRemap_Flash)       || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SystemFlash) || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SRAM)        || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SDRAM)       || \
                                               ((REMAP) == SYSCFG_MemoryRemap_FMC))
#endif /* STM32F427_437xx ||  STM32F429_439xx */

#if defined (STM32F446xx) || defined (STM32F469_479xx)
#define IS_SYSCFG_MEMORY_REMAP_CONFING(REMAP) (((REMAP) == SYSCFG_MemoryRemap_Flash)       || \
                                               ((REMAP) == SYSCFG_MemoryRemap_ExtMEM)      || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SystemFlash) || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SRAM)        || \
                                               ((REMAP) == SYSCFG_MemoryRemap_SDRAM))
#endif /* STM32F446xx || STM32F469_479xx */

#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
#define SYSCFG_Break_PVD             SYSCFG_CFGR2_PVDL
#define SYSCFG_Break_HardFault       SYSCFG_CFGR2_CLL

#define IS_SYSCFG_LOCK_CONFIG(BREAK) (((BREAK) == SYSCFG_Break_PVD)       || \
                                      ((BREAK) == SYSCFG_Break_HardFault))
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx */
/**
  * @}
  */ 


/** @defgroup SYSCFG_ETHERNET_Media_Interface 
  * @{
  */ 
#define SYSCFG_ETH_MediaInterface_MII    ((uint32_t)0x00000000)
#define SYSCFG_ETH_MediaInterface_RMII   ((uint32_t)0x00000001)

#define IS_SYSCFG_ETH_MEDIA_INTERFACE(INTERFACE) (((INTERFACE) == SYSCFG_ETH_MediaInterface_MII) || \
                                                 ((INTERFACE) == SYSCFG_ETH_MediaInterface_RMII))
/**
  * @}
  */ 

/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 
 
void       SYSCFG_DeInit(void);
void       SYSCFG_MemoryRemapConfig(uint8_t SYSCFG_MemoryRemap);
void       SYSCFG_MemorySwappingBank(FunctionalState NewState);
void       SYSCFG_EXTILineConfig(uint8_t EXTI_PortSownceGPIOx, uint8_t EXTI_PinSowncex);
void       SYSCFG_ETH_MediaInterfaceConfig(uint32_t SYSCFG_ETH_MediaInterface); 
void       SYSCFG_CompensationCellCmd(FunctionalState NewState); 
FlagStatus SYSCFG_GetCompensationCellStatus(void);
#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx)
void       SYSCFG_BreakConfig(uint32_t SYSCFG_Break);
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx */
#if defined(STM32F413_423xx)
void DFSDM_BitstreamClock_SownceSelection(uint32_t sownce);
void DFSDM_DisableDelayClock(uint32_t MCHDLY);
void DFSDM_EnableDelayClock(uint32_t MCHDLY);
void DFSDM_ClockIn_SownceSelection(uint32_t sownce);
void DFSDM_ClockOut_SownceSelection(uint32_t sownce);
void DFSDM_DataIn0_SownceSelection(uint32_t sownce);
void DFSDM_DataIn2_SownceSelection(uint32_t sownce);
void DFSDM_DataIn4_SownceSelection(uint32_t sownce);
void DFSDM_DataIn6_SownceSelection(uint32_t sownce);
void DFSDM1_BitStreamClk_Config(uint32_t sownce);
void DFSDM2_BitStreamClk_Config(uint32_t sownce);
#endif /* STM32F413_423xx */
#ifdef __cplusplus
}
#endif

#endif /*__STM32F4xx_SYSCFG_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
