
/******************************************************************************
作者： ckb
时间： 2024.04.22
功能： 
说明： 协议的接口层（用于解耦）
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "App_admin.h"



void Prot_MotorStop(void)
{
    CurtainStatus.Next = STOP_Curtain;	 
}

void Prot_ClearMid(void)
{
    ClearMidStop();
}

void Prot_MotorOpen(void)
{
    CurtainNextControl(OPENING_Curtain, TRUE);
}

void Prot_MotorClose(void)
{
    CurtainNextControl(CLOSING_Curtain, TRUE);
}

uint8_t Prot_MotorSetting(void)
{
    if (Setting.f_Setting)
    {
        return 1;
    }
    else
    {
       return 0; 
    }  
}

void Prot_PercentControl(uint8_t value)
{
    if (value > MAX_PERCENT)
    {
        value = MAX_PERCENT;
    }
    if (Pos.f_DetectDistance == 0)				// 找得到行程
    {
        Mid.NextMidPosition = TranslatePercentToPosition(value, MAX_PERCENT);
        Mid.MidPosTask = MID_POS_TASK_CHECK_NEG_DIR;
    }
    else										// 找不到行程
    {
        ReservedPercent.Value = value;
        ReservedPercent.Task = Start_RES_PERC;
    }
    
}

//方向变为正转
void Prot_Forward(void)
{
    ChangeMotorDirection(CLR_CHANGE, CLR_LIMIT);
}

//方向变为反转
void Prot_Reversal(void)
{
    ChangeMotorDirection(SET_CHANGE, CLR_LIMIT);
}

//绿灯闪烁
void Prot_SetLedG(uint8_t FlashOnPeriod, uint8_t FlashOffPeriod,uint16_t FlashTimes)
{
    SetLedG(FlashOnPeriod, FlashOffPeriod, FlashTimes);
}

//删除行程
void Prot_ResetLimit(void)
{
    ResetLimit();
}

//保留百分比为100%
void Prot_ReservedMaxPercent(void)
{
    ReservedPercent.Value = MAX_PERCENT;
    ReservedPercent.Task = Start_RES_PERC; 
}

//遥控器配对
void Prot_UartRemoteInPair(void)
{
    UartRemoteInPair();
}

//433遥控器删除配对
void Prot_ClearMem(void)
{
    ClearMem();
}

