/******************************************************************************
作者： ckb
时间： 2024.04.22
功能： 
说明： 	
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MY_PROTOCOL_INTERFACE_H
#define MY_PROTOCOL_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "App_admin.h"

#define PROT_PERCENT            VirtualAddr[1]
#define PROT_MOTORSTATUS        VirtualAddr[0]
#define PROT_DERECTION          VirtualAddr[5]
#define PROT_HASDISTANCE        VirtualAddr[3]

extern void Prot_MotorStop(void);
extern void Prot_ClearMid(void);
extern void Prot_MotorOpen(void);
extern void Prot_MotorClose(void);
extern uint8_t Prot_MotorSetting(void);
extern void Prot_PercentControl(uint8_t value);
extern void Prot_Forward(void);
extern void Prot_Reversal(void);
extern void Prot_SetLedG(uint8_t FlashOnPeriod, uint8_t FlashOffPeriod,uint16_t FlashTimes);
extern void Prot_ReservedMaxPercent(void);
extern void Prot_UartRemoteInPair(void);
extern void Prot_ClearMem(void);

#endif
