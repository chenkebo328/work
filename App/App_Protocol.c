
/******************************************************************************
作者： name
时间： 2020.02.26
功能： 
说明： 	
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "App_admin.h"

/* Private Macro Define ------------------------------------------------------*/

/* Private Types Define ------------------------------------------------------*/
uint8_t Pack_Num = 0;

unsigned short const crc16_table[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};//萤石协议用CRC校验

typedef enum
{
    RS485_TASK_Free = 0,

    RS485_TASK_READ,            // RS485 读      任务
    RS485_TASK_WRITE,           // RS485 写      任务
    RS485_TASK_CONTROL,         // RS485 控制    任务

    RS485_TASK_Sucess,
    RS485_TASK_Error,
    RS485_TASK_End
    
}RS485_Task_TypeDef;

typedef enum
{
	WIFI_OFF_LINE = 0,
	WIFI_ON_LINE = 1,
	WIFI_ON_LINE_ing = 2,
	WIFI_OFF_LING_ing = 3,
	
	WIFI_INIT = 0xFF
	
}Wifi_StatusEnum_TypeDef;

typedef struct
{
	Wifi_StatusEnum_TypeDef Last;
	Wifi_StatusEnum_TypeDef Now;
	
}Wifi_Status_TypeDef;

typedef enum
{
	YINGSHI_WIFI_OFF_LINE = 1,
	YINGSHI_WIFI_ON_LINE_ing = 2,
	YINGSHI_WIFI_ON_LINE = 3,

	YINGSHI_WIFI_INIT = 0xFF
	
}YingShi_Wifi_StatusEnum_TypeDef;//萤石协议用网络状态

typedef struct
{
	YingShi_Wifi_StatusEnum_TypeDef Last;
	YingShi_Wifi_StatusEnum_TypeDef Now;
	
}YingShi_Wifi_Status_TypeDef;//萤石协议用网络状态

/* Variable Define -----------------------------------------------------------*/
uint8_t f_EnWrite485;
uint8_t Motor_ID_L;
uint8_t Motor_ID_H;
Wifi_Status_TypeDef WifiStatus = {WIFI_INIT, WIFI_INIT};
YingShi_Wifi_Status_TypeDef YingShi_WifiStatus = {YINGSHI_WIFI_INIT, YINGSHI_WIFI_INIT};


// 协议类型
ProtocolType_TypeDef ProtocolType;

// 
TEST_MODULE_TXRX_STATUS_2G3G TestModuleTxRxStatus_2G3G;


/* Functions Define ----------------------------------------------------------*/

/****************************************************************
** 功能描述： 研发测试命令
** 输入参数： 
** 返 回 值： 
** 调用方式： 在 ProcessReceivePacket() 中调用
** 函数说明： 
*****************************************************************/
//static void DebugCmd()
//{
//	if (DBG_R_DATA[0] == 0xA5)
//	{
//		// 研发测试命令
//		if (DBG_R_DATA[1] == 0x05)
//		{
//			// 测试 PWM 与转速的关系
//			if (DBG_R_DATA[2] == 0x01)
//			{
//				if (DBG_R_DATA[3])
//				{
//					PID.SettedSpeed = DBG_R_DATA[3];
//					DingPWM = 0;
//				}
//				else
//				{
//					DingPWM = DBG_R_DATA[4];
//					DingPWM = (DingPWM<<8)+DBG_R_DATA[5];
//				}
//			}
//		}
//	}
//}

/****************************************************************
** 功能描述： 杜亚通用串口协议
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
static void DooyaCommonUartProtocol(void)
{
	uint16_t TempCrc16;

	// 头码判断
	if (COM_R_DATA[0] != 0x55)
	{
		return;
	}

	// 接收最少数据个数限制
	if (COM_R_NUM <= 5)
	{
		return;
	}

	// 固定 ID 号比较
	if ((COM_R_DATA[1]!=0xFE) || (COM_R_DATA[2]!=0xFE))
	{
		return;
	}

	// 校验crc
	if (Calc_crc16(0xFFFF, &(COM_R_DATA[0]), COM_R_NUM-2) != (((uint16_t)COM_R_DATA[COM_R_NUM-2]<<8)+COM_R_DATA[COM_R_NUM-1]))
	{
		return;
	}

	// 配置电机协议为 通用串口协议
	if (ProtocolType != DOOYA_COMMON_UART)
	{
		ProtocolType = DOOYA_COMMON_UART;
		Write32BitToEE(ADDR_PROTOCOL_TYPE, ProtocolType);
	}

	switch(COM_R_DATA[3])
	{
		// 读
		case 0x01:	
			GetAllMotorInformation();
			
			// 一次性 最多读写 16 个数据 
			if (COM_R_DATA[DATA_INFO_POS+1] <= 0x10)
			{
				COM_T_DATA[HEAD_POS] = RS485_HEAD;	  
				COM_T_DATA[IDL_POS] = 0xFE;
				COM_T_DATA[IDH_POS] = 0xFE;

				COM_T_DATA[3] = 0x01;
				COM_T_DATA[DATA_INFO_POS] = COM_R_DATA[DATA_INFO_POS];		  // 数据地址
				COM_T_DATA[DATA_INFO_POS+1] = COM_R_DATA[DATA_INFO_POS+1];	  // 数据长度
				memcpy(&COM_T_DATA[DATA_INFO_POS+2], &VirtualAddr[COM_R_DATA[DATA_INFO_POS]], COM_R_DATA[DATA_INFO_POS+1]);
				TempCrc16 = Calc_crc16(0xFFFF, &COM_T_DATA[HEAD_POS], 6+COM_R_DATA[DATA_INFO_POS+1]);
				COM_T_DATA[6+COM_R_DATA[DATA_INFO_POS+1]] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[7+COM_R_DATA[DATA_INFO_POS+1]] = (uint8_t)TempCrc16;
				
				RequestTransmit(&COM_T, 8+COM_R_DATA[DATA_INFO_POS+1]);
			}
		break;

		// 写
		case 0x02:
			// 一次性 最多读写 16 个数据 
			if (COM_R_DATA[DATA_INFO_POS+1] <= 0x10)
			{
							  // 方向
				if ((V_DIR>=COM_R_DATA[DATA_INFO_POS]) && (V_DIR<(COM_R_DATA[DATA_INFO_POS]+COM_R_DATA[DATA_INFO_POS+1])))
				{
					if (COM_R_DATA[V_DIR-COM_R_DATA[DATA_INFO_POS]+6] == 0x00)
					{
						ChangeMotorDirection(CLR_CHANGE, CLR_LIMIT);
					}
					else if (COM_R_DATA[V_DIR-COM_R_DATA[DATA_INFO_POS]+6] == 0x01)
					{
						ChangeMotorDirection(SET_CHANGE, CLR_LIMIT);
					}
				}
							  // 手拉
				if ((V_PULLSTART>=COM_R_DATA[DATA_INFO_POS]) && (V_PULLSTART<(COM_R_DATA[DATA_INFO_POS]+COM_R_DATA[DATA_INFO_POS+1])))
				{
					if (COM_R_DATA[V_PULLSTART-COM_R_DATA[DATA_INFO_POS]+6] == 0x00)
					{
						ChangePullStart(SET_CHANGE);
					}
					else if (COM_R_DATA[V_PULLSTART-COM_R_DATA[DATA_INFO_POS]+6] == 0x01)
					{
						ChangePullStart(CLR_CHANGE);
					}
				}
							  // 弱电开关类型
				if ((V_WEAKKEY_MOD>=COM_R_DATA[DATA_INFO_POS]) && (V_WEAKKEY_MOD<(COM_R_DATA[DATA_INFO_POS]+COM_R_DATA[DATA_INFO_POS+1])))
				{
					if (COM_R_DATA[V_WEAKKEY_MOD-COM_R_DATA[DATA_INFO_POS]+6]<=4)
					{		   
						SwitchMod.Weak = (WeakKeyMod_TypeDef)(COM_R_DATA[V_WEAKKEY_MOD-COM_R_DATA[DATA_INFO_POS]+6]);
						Write32BitToEE(ADDR_WEAK_SW_MOD, (uint32_t)(SwitchMod.Weak));
					}
				}
							  // 强电开关类型
				if ((V_POWERKEY_MOD>=COM_R_DATA[DATA_INFO_POS]) && (V_POWERKEY_MOD<(COM_R_DATA[DATA_INFO_POS]+COM_R_DATA[DATA_INFO_POS+1])))
				{
					if	(COM_R_DATA[V_POWERKEY_MOD-COM_R_DATA[DATA_INFO_POS]+6]<=2)
					{
						SwitchMod.Power = (PowerKeyMod_TypeDef)((COM_R_DATA[V_POWERKEY_MOD-COM_R_DATA[DATA_INFO_POS]+6])+1);
						Write32BitToEE(ADDR_POWER_SW_MOD, (uint32_t)(SwitchMod.Power));
					}
				}

				COM_T_DATA[HEAD_POS] = RS485_HEAD;	  
				COM_T_DATA[IDL_POS] = 0xFE;
				COM_T_DATA[IDH_POS] = 0xFE;

				COM_T_DATA[FUNC_POS] = RS485_WRITE;
				COM_T_DATA[DATA_INFO_POS] = COM_R_DATA[DATA_INFO_POS];	  // 数据地址
				COM_T_DATA[DATA_INFO_POS+1] = COM_R_DATA[DATA_INFO_POS+1];	// 数据长度

				TempCrc16 = Calc_crc16(0xFFFF, &COM_T_DATA[HEAD_POS], 6);
				COM_T_DATA[DATA_INFO_POS+2] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[DATA_INFO_POS+3] = (uint8_t)TempCrc16;

				SetLedG(3, 3, 5);
							
				RequestTransmit(&COM_T, 8);
			}
				
		break;

		// 控制
		case 0x03:
			COM_T_DATA[HEAD_POS] = RS485_HEAD;	  
			COM_T_DATA[IDL_POS] = 0xFE;
			COM_T_DATA[IDH_POS] = 0xFE;

			COM_T_DATA[FUNC_POS] = RS485_CONTROL;
			COM_T_DATA[DATA_INFO_POS] = COM_R_DATA[DATA_INFO_POS];		// 数据地址

			switch (COM_R_DATA[DATA_INFO_POS])
			{
				case RS485_CONTROL_UP:
					if (Setting.f_Setting)
					{
						break;
					}
					CurtainNextControl(OPENING_Curtain, TRUE);					
				break;

				case RS485_CONTROL_DOWN:
					if (Setting.f_Setting)
					{
						break;
					}
					CurtainNextControl(CLOSING_Curtain, TRUE);					

				break;
			
				case RS485_CONTROL_STOP:
					CurtainStatus.Next = STOP_Curtain;	 
					ClearMidStop();

				break;

				case RS485_CONTROL_PERCENT:
					if (Setting.f_Setting)
					{
						break;
					}
				
					if (COM_R_DATA[DATA_INFO_POS+1] >= MAX_PERCENT)
					{
						CurtainNextControl(OPENING_Curtain, TRUE);					
						COM_T_DATA[DATA_INFO_POS+1] = MAX_PERCENT;
					}
					else if (COM_R_DATA[DATA_INFO_POS+1] == 0)
					{
						CurtainNextControl(CLOSING_Curtain, TRUE);					
						COM_T_DATA[DATA_INFO_POS+1] = 0x00;
					}
					else
					{
						if (Pos.f_DetectDistance == 0)				// 找得到行程
						{
							Mid.NextMidPosition = TranslatePercentToPosition(COM_R_DATA[DATA_INFO_POS+1], MAX_PERCENT);
							Mid.MidPosTask = MID_POS_TASK_CHECK_NEG_DIR;
							
							COM_T_DATA[DATA_INFO_POS+1] = COM_R_DATA[DATA_INFO_POS+1];
						}
						else										// 找不到行程
						{
							ReservedPercent.Value = COM_R_DATA[DATA_INFO_POS+1];
							ReservedPercent.Task = Start_RES_PERC;
							
							COM_T_DATA[DATA_INFO_POS+1] = 0xFF;
						}
					}

				break;

				case RS485_CONTROL_SET_CLR_OPEN_GAP:
					if ((COM_R_DATA[DATA_INFO_POS+1] == 0x00) || (COM_R_DATA[DATA_INFO_POS+1] == 0x01))
					{
						if (Pos.f_DetectDistance == 0)
						{
							COM_T_DATA[DATA_INFO_POS+1] = COM_R_DATA[DATA_INFO_POS+1];

							if (COM_R_DATA[DATA_INFO_POS+1] == 0x01)		// 设置
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeClockWiseGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF;
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeAntiClockGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
										SetLedG(1, 1, 10);
									}
								}
							}
							else											// 取消
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeClockWiseGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeAntiClockGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
										SetLedG(1, 1, 10);
									}
								}
							}
						}
						else
						{
							COM_T_DATA[DATA_INFO_POS+1] = 0xFF;
							SetLedG(1, 1, 10);
						}
					}
					else
					{
						SetLedG(1, 1, 10);
					
						return;
					}
				break;

				case RS485_CONTROL_SET_CLR_CLOSE_GAP:
					if ((COM_R_DATA[DATA_INFO_POS+1] == 0x00) || (COM_R_DATA[DATA_INFO_POS+1] == 0x01))
					{
						if (Pos.f_DetectDistance == 0)
						{
						
							COM_T_DATA[DATA_INFO_POS+1] = COM_R_DATA[DATA_INFO_POS+1];

							if (COM_R_DATA[DATA_INFO_POS+1] == 0x01)		// 设置
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeAntiClockGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeClockWiseGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
										SetLedG(1, 1, 10);
									}
								}
							}
							else											// 取消
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeAntiClockGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeClockWiseGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
										SetLedG(1, 1, 10);
									}
								}
							}
						}
						else
						{
							COM_T_DATA[DATA_INFO_POS+1] = 0xFF; 						 
							SetLedG(1, 1, 10);
						}
					}
					else
					{
						SetLedG(1, 1, 10);
						
						return;
					}

				break;

				case RS485_CONTROL_CLR_LIMIT:
					ResetLimit();
					SetLedG(3, 3, 5);
					CurtainStatus.Next = STOP_Curtain;

				break;

				case RS485_CONTROL_RESET_FACTORY:
					MotorReset();
					SetLedG(3, 3, 5);
					CurtainStatus.Next = STOP_Curtain;

				break;
				
				default:
				break;
			}

			if ((COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_UP) || (COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_DOWN) || (COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_STOP) ||
				(COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_CLR_LIMIT) || (COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_RESET_FACTORY))
			{
				TempCrc16 = Calc_crc16(0xFFFF, &COM_T_DATA[HEAD_POS], 5);
				COM_T_DATA[DATA_INFO_POS+1] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[DATA_INFO_POS+2] = (uint8_t)TempCrc16;
								
				RequestTransmit(&COM_T, 7);
			}
			else if ((COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_PERCENT) || (COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_SET_CLR_OPEN_GAP) ||
					(COM_R_DATA[DATA_INFO_POS]==RS485_CONTROL_SET_CLR_CLOSE_GAP))
			{
				TempCrc16 = Calc_crc16(0xFFFF, &COM_T_DATA[HEAD_POS], 6);
				COM_T_DATA[DATA_INFO_POS+2] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[DATA_INFO_POS+3] = (uint8_t)TempCrc16;
				
				RequestTransmit(&COM_T, 8);
			}
		break;

		default:
		break;
	}
}

/****************************************************************
** 功能描述： 统一协议：发送串口命令通知模块进入配网状态
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
void PeiWang(void)
{
	uint16_t TempCrc16;

	COM_T_DATA[0] = 0x55;
	COM_T_DATA[1] = 0xAA;
	COM_T_DATA[2] = 0x04;
	COM_T_DATA[3] = 0x03;
	COM_T_DATA[4] = 0x02;
	TempCrc16 = CRC16_MODBUS(COM_T_DATA, 5);
	COM_T_DATA[5] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[6] = (uint8_t)TempCrc16;
	RequestTransmit(&COM_T, 7);
}

/****************************************************************
** 功能描述： 萤石协议：发送串口命令通知模块进入配网状态
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
void PeiWang_YingShi(void)
{
	uint16_t TempCrc16;

	COM_T_DATA[0] = 0x5A;
	COM_T_DATA[1] = 0x5A;
	COM_T_DATA[2] = 0x01;
	COM_T_DATA[3] = 0x00;
	COM_T_DATA[4] = Pack_Num;
	if (Pack_Num < 0xff)
	{
		Pack_Num+=1;
	}
	COM_T_DATA[5] = Config_Data_From_Mcu;
	COM_T_DATA[6] = 0x00;
	COM_T_DATA[7] = 0x07;
	COM_T_DATA[8] = (uint8_t)(Reset_Module>>8);
	COM_T_DATA[9] = (uint8_t)Reset_Module;
	COM_T_DATA[10] = 0x00;
	COM_T_DATA[11] = 0x04;
	COM_T_DATA[12] = 0x00;
	COM_T_DATA[13] = 0x01;
	COM_T_DATA[14] = 0x01;
	TempCrc16 = CRC16(&COM_T_DATA[0], 15);
	COM_T_DATA[15] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[16] = (uint8_t)TempCrc16;	
	RequestTransmit(&COM_T, 17);

}

/****************************************************************
** 功能描述： 统一协议：发送串口命令通知模块恢复出厂设置
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
void ModelReset(void)
{
	uint16_t TempCrc16;

	COM_T_DATA[0] = 0x55;
	COM_T_DATA[1] = 0xAA;
	COM_T_DATA[2] = 0x04;
	COM_T_DATA[3] = 0x03;
	COM_T_DATA[4] = 0x03;
	TempCrc16 = CRC16_MODBUS(COM_T_DATA, 5);
	COM_T_DATA[5] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[6] = (uint8_t)TempCrc16;
	RequestTransmit(&COM_T, 7);
}

/****************************************************************
** 功能描述： 萤石协议：发送串口命令通知模块恢复出厂设置
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
void ModelReset_YingShi(void)
{
	uint16_t TempCrc16;

	COM_T_DATA[0] = 0x5A;
	COM_T_DATA[1] = 0x5A;
	COM_T_DATA[2] = 0x01;
	COM_T_DATA[3] = 0x00;
	COM_T_DATA[4] = Pack_Num;
	if (Pack_Num < 0xff)
	{
		Pack_Num+=1;
	}
	COM_T_DATA[5] = Config_Data_From_Mcu;
	COM_T_DATA[6] = 0x00;
	COM_T_DATA[7] = 0x07;
	COM_T_DATA[8] = (uint8_t)(Reset_Module>>8);
	COM_T_DATA[9] = (uint8_t)Reset_Module;
	COM_T_DATA[10] = 0x00;
	COM_T_DATA[11] = 0x04;
	COM_T_DATA[12] = 0x00;
	COM_T_DATA[13] = 0x01;
	COM_T_DATA[14] = 0x02;
	TempCrc16 = CRC16(&COM_T_DATA[0], 15);
	COM_T_DATA[15] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[16] = (uint8_t)TempCrc16;	
	RequestTransmit(&COM_T, 17);

}

/****************************************************************
** 功能描述： 统一协议：发送串口命令通知模块进入产测状态，第二代/第三代PCBA自动测试都是通过此命令让模块进入测试状态。
** 输入参数： 
** 返 回 值： 
** 调用方式： 第二代测试中：Self_Test() 会调用到此函数
				第三代测试中：Test_MODULE_3G() 会调用到此函数
** 函数说明： 
				第二代测试中，Self_Test()会通过此函数给模块发送一条串口命令使模块进入测试状态，模块通过串口把测试结果反馈给mcu，
				muc判断测试结果是否合格，合格则给 TestModuleTxRxStatus_2G3G 赋值为TEST_MODULE_STATUS_PASS，不合格赋值为TEST_MODULE_STATUS_NG

				第三代测试中，Test_MODULE_3G()会通过此函数给模块发送一条串口命令使模块进入测试状态，模块通过串口把测试结果反馈给mcu，
				mcu把模块的测试结果反馈给第三代PCBA测试治具，治具根据最终的信号强度判断模块是否合格。
				
TestModuleTxRxStatus_2G3G 变量值的意义：

TEST_MODULE_STATUS_IDLE = 0,			模块测试空闲
TEST_MODULE_STATUS_TESTING = 1,			模块开始测试，测试中
TEST_MODULE_STATUS_PASS = 2,			模块测试成功
TEST_MODULE_STATUS_NG = 3				模块测试失败
*****************************************************************/
void TestModuleTxRx_2G3G(void)
{
	uint16_t TempCrc16;
	COM_T_DATA[0] = 0x55;
	COM_T_DATA[1] = 0xAA;
	COM_T_DATA[2] = 0x04;
	COM_T_DATA[3] = 0x03;
	COM_T_DATA[4] = 0x05;
	TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 5);
	COM_T_DATA[5] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[6] = (uint8_t)TempCrc16; 	
	RequestTransmit(&COM_T, 7);
}


/****************************************************************
** 功能描述： 查询wifi模块的网络状态，显示wifi模块的网络状态
** 输入参数： 
** 返 回 值： 
** 调用方式： 0.1s 调用一次
** 函数说明： 
*****************************************************************/
void CheckAndShowWifiStatus(void)
{
	static uint8_t i=0;
	uint16_t TempCrc16;

	// 显示网络状态
	if (WifiStatus.Now != WifiStatus.Last)
	{
		if (WifiStatus.Now == WIFI_OFF_LINE)
		{			
			SetLedR(10,10, 0);
		}
		else if (WifiStatus.Now == WIFI_ON_LINE_ing)
		{
			SetLedR(1,1, 0);
		}
		else if (WifiStatus.Now == WIFI_ON_LINE)
		{	
			SetLedR(20,20, 1);
		}
		else if (WifiStatus.Now == WIFI_OFF_LING_ing)
		{
			;
		}
		
		WifiStatus.Last = WifiStatus.Now;
	}

	if (i < 15)
	{
		i++;
		return;
	}
	i = 0;

	// 没有数据要发送时才定时查询
	if (COM_T_PENDING == 1 && COM_TX_SPACETIME == 0)
	{
		return;
	}

	COM_T_DATA[0] = 0x55;
	COM_T_DATA[1] = 0xAA;
	COM_T_DATA[2] = 0x04;
	COM_T_DATA[3] = 0x03;
	COM_T_DATA[4] = 0x04;
	TempCrc16 = CRC16_MODBUS(COM_T_DATA, 5);
	COM_T_DATA[5] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[6] = (uint8_t)TempCrc16;
	RequestTransmit(&COM_T, 7);
}

/****************************************************************
** 功能描述：萤石协议显示wifi模块的网络状态
** 输入参数： 
** 返 回 值： 
** 调用方式： 0.1s 调用一次
** 函数说明： 
*****************************************************************/
void ShowWifiStatus_YingShi(void)
{
	// 显示网络状态
	if (YingShi_WifiStatus.Now != YingShi_WifiStatus.Last)
	{
		if (YingShi_WifiStatus.Now == YINGSHI_WIFI_OFF_LINE)
		{			
			SetLedR(10,10, 0);
		}
		else if (YingShi_WifiStatus.Now == YINGSHI_WIFI_ON_LINE_ing)
		{
			SetLedR(1,1, 0);
		}
		else if (YingShi_WifiStatus.Now == YINGSHI_WIFI_ON_LINE)
		{	
			SetLedR(20,20, 1);
		}
		
		YingShi_WifiStatus.Last = YingShi_WifiStatus.Now;
	}
}

/****************************************************************
** 功能描述： 杜亚统一串口协议
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
static void DooyaUnifiedProtocol(void)
{
	uint16_t TempCrc16;
	char typestr[] = MOTORTYPE;
	char softstr[] = SOFTWARE_Rev;
	uint8_t len = strlen(softstr);
	uint8_t typelen = strlen(typestr);

	// 头码判断
	if ((COM_R_DATA[0]!=0x55) || (COM_R_DATA[1]!=0xAA))
	{
		return;
	}
	
	// 接收到的最少数据个数限制
	if (COM_R_NUM <= 5)
	{
		return;
	}
	
	// 校验
	if (CRC16_MODBUS(&COM_R_DATA[0], COM_R_NUM-2) != (((uint16_t)COM_R_DATA[COM_R_NUM-2]<<8)+COM_R_DATA[COM_R_NUM-1]))
	{
		return;
	}

	// 配置电机协议为 统一串口协议
	if (ProtocolType != DOOYA_UNIFIED_UART)
	{
		ProtocolType = DOOYA_UNIFIED_UART;
		Write32BitToEE(ADDR_PROTOCOL_TYPE, ProtocolType);
	}
	
	memcpy(&COM_T_DATA[0], &COM_R_DATA[0], COM_R_NUM);

	// 模块下发命令
	if (COM_R_DATA[3] == 0x01)
	{
		switch (COM_R_DATA[4])
		{
			// 电机状态
			case 0x01:
				if (COM_R_DATA[5] == 0x01)
				{
					if (Setting.f_Setting)
					{
						break;
					}
					CurtainNextControl(OPENING_Curtain, TRUE);					
				}
				else if (COM_R_DATA[5] == 0x02)
				{
					if (Setting.f_Setting)
					{
						break;
					}
					CurtainNextControl(CLOSING_Curtain, TRUE);					
				}
				else if (COM_R_DATA[5] == 0x03)
				{
					CurtainStatus.Next = STOP_Curtain;	 
					ClearMidStop();
				}
				COM_T_NUM = COM_R_NUM;
			break;
			
			// 百分比与角度控制
			case 0x02:
				if (Setting.f_Setting)
				{
					break;
				}
				
				if (COM_R_DATA[5] >= MAX_PERCENT)
				{
					CurtainNextControl(OPENING_Curtain, TRUE);					
					COM_T_DATA[5] = MAX_PERCENT;
				}
				else if (COM_R_DATA[5] == 0)
				{
					CurtainNextControl(CLOSING_Curtain, TRUE);					
					COM_T_DATA[5] = 0x00;
				}
				else
				{
					if (Pos.f_DetectDistance == 0)				// 找得到行程
					{
						Mid.NextMidPosition = TranslatePercentToPosition(COM_R_DATA[5], MAX_PERCENT);
						Mid.MidPosTask = MID_POS_TASK_CHECK_NEG_DIR;
					}
					else										// 找不到行程
					{
						ReservedPercent.Value = COM_R_DATA[5];
						ReservedPercent.Task = Start_RES_PERC;
						
						COM_T_DATA[5] = 0xFF;
					}
				}
				
				COM_T_NUM = COM_R_NUM;
			break;
	
			// 手拉启动功能
			case 0x03:
				if (COM_R_DATA[5] == 0x01)
				{
					ChangePullStart(SET_CHANGE);
	
					SetLedG(3, 3, 5);
				}
				else if (COM_R_DATA[5] == 0x00)
				{
					ChangePullStart(CLR_CHANGE);
	
					SetLedG(3, 3, 5);
				}
				
				COM_T_NUM = COM_R_NUM;
			break;
	
			// 方向设置
			case 0x04:				  
				CurtainStatus.Next = STOP_Curtain;	 
				if (COM_R_DATA[5] == 0x01)
				{
					ChangeMotorDirection(CLR_CHANGE, CLR_LIMIT);
	
					SetLedG(3, 3, 5);
				}
				else if (COM_R_DATA[5] == 0x02)
				{
					ChangeMotorDirection(SET_CHANGE, CLR_LIMIT);
	
					SetLedG(3, 3, 5);
				}
				else if (COM_R_DATA[5] == 0x03)
				{
					ChangeMotorDirection(TOGGLE_CHANGE, CLR_LIMIT);
	
					SetLedG(3, 3, 5);
				}
				
				COM_T_NUM = COM_R_NUM;
			break;
	
			// 设置弱电开关模式
			case 0x05:
				if ((COM_R_DATA[5]>0) && (COM_R_DATA[5]<=4))
				{
					SwitchMod.Weak = (WeakKeyMod_TypeDef)(COM_R_DATA[5]);
					Write32BitToEE(ADDR_WEAK_SW_MOD, (uint32_t)(SwitchMod.Weak));
	
					SetLedG(3, 3, 5);
				}
				
				COM_T_NUM = COM_R_NUM;
			break;
	
			// 设置强电开关模式
			case 0x06:
				if ((COM_R_DATA[5]>0) && (COM_R_DATA[5]<=3))
				{
					SwitchMod.Power = (PowerKeyMod_TypeDef)(COM_R_DATA[5]);
					Write32BitToEE(ADDR_POWER_SW_MOD, (uint32_t)(SwitchMod.Power));
	
					SetLedG(3, 3, 5);
				}
				
				COM_T_NUM = COM_R_NUM;
			break;
	
			// 手动设置/取消打开边界
			case 0x07:
				if ((COM_R_DATA[5] == 0x00) || (COM_R_DATA[5] == 0x01))
				{
					if (Pos.f_DetectDistance == 0)
					{						 
						if (COM_R_DATA[5] == 0x01)	  // 设置
						{
							if (TrackDirection == POSITIVE_DIR)
							{
								if (ChangeClockWiseGap_MT(SET_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;
									SetLedG(1, 1, 10);
								}
							}
							else
							{
								if (ChangeAntiClockGap_MT(SET_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;						   
									SetLedG(1, 1, 10);
								}
							}
						}
						else											// 取消
						{
							if (TrackDirection == POSITIVE_DIR)
							{
								if (ChangeClockWiseGap_MT(CLR_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;						   
									SetLedG(1, 1, 10);
								}
							}
							else
							{
								if (ChangeAntiClockGap_MT(CLR_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;						   
									SetLedG(1, 1, 10);
								}
							}
						}
					}
					else
					{
						COM_T_DATA[5] = 0xFF;
						SetLedG(1, 1, 10);
					}
				}
				else
				{
					SetLedG(1, 1, 10);					
					return;
				}
				
				COM_T_NUM = COM_R_NUM;
			break;
	
			// 手动设置/取消关闭边界
			case 0x08:
				if ((COM_R_DATA[5] == 0x00) || (COM_R_DATA[5] == 0x01))
				{
					if (Pos.f_DetectDistance == 0)
					{						 
						if (COM_R_DATA[5] == 0x01)	  // 设置
						{
							if (TrackDirection == POSITIVE_DIR)
							{
								if (ChangeAntiClockGap_MT(SET_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;						   
									SetLedG(1, 1, 10);
								}
							}
							else
							{
								if (ChangeClockWiseGap_MT(SET_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;						   
									SetLedG(1, 1, 10);
								}
							}
						}
						else											// 取消
						{
							if (TrackDirection == POSITIVE_DIR)
							{
								if (ChangeAntiClockGap_MT(CLR_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;						   
									SetLedG(1, 1, 10);
								}
							}
							else
							{
								if (ChangeClockWiseGap_MT(CLR_CHANGE))
								{
									SetLedG(3, 3, 5);
								}
								else
								{
									COM_T_DATA[5] = 0xFF;						   
									SetLedG(1, 1, 10);
								}
							}
						}
					}
					else
					{
						COM_T_DATA[5] = 0xFF;						   
						SetLedG(1, 1, 10);
					}
				}
				else
				{
					SetLedG(1, 1, 10);
					return;
				}
				
				COM_T_NUM = COM_R_NUM;
			break;

			//设置行程点,	只支持第三行程点设置
			case 0x09:
				if(COM_R_DATA[5] == 0x03)
				{
					if (ChangeThirdPos(SET_CHANGE))
					{
						SetLedG(3, 3, 5);
					}
					else
					{
						COM_T_DATA[5] = 0xff;
						SetLedG(1, 1, 10);
					}
				}

				COM_T_NUM = COM_R_NUM;
			break;

			// 运行至第三行程点
			case 0x0A:
				if(COM_R_DATA[5] == 0x03)
				{
					if ((Setting.f_Setting==0) && (Pos.f_DetectDistance==0) && (Third.Setted_ThirdPosition))	// 不在设置状态 找得到行程 且 已经设置第三行程点
					{
						Mid.NextMidPosition = Third.ThirdPosition;
						Mid.MidPosTask = MID_POS_TASK_CHECK_NEG_DIR;
	
					}
					else
					{
						COM_T_DATA[5] = 0xff;
					}
				}

				COM_T_NUM = COM_R_NUM;
			break;
	
			// 删除行程点
			case 0x0B:
				if ((COM_R_DATA[5]==0x01) || (COM_R_DATA[5]==0x02) || (COM_R_DATA[5]==0x10))
				{
					ResetLimit();					
					SetLedG(3, 3, 5);
					CurtainStatus.Next = STOP_Curtain;
				}
				// 第三行程点
				else if(COM_R_DATA[5] == 0x03)
				{
					ChangeThirdPos(CLR_CHANGE); 				
					SetLedG(3, 3, 5);
				}

				COM_T_NUM = COM_R_NUM;
			break;

			// 校准行程
			case 0x13:
				ResetLimit();					
				ReservedPercent.Value = MAX_PERCENT;
				ReservedPercent.Task = Start_RES_PERC;

				COM_T_NUM = COM_R_NUM;
			break;
			case 0xC0:
		//		if(BLERemote.Work == 1)
		//		{
		//			if(!ModelTestBLERemote_Deal(&COM_R_DATA[5]))
		//			{
		//				SendModelTestCMD();
		//				BLERemote.Work = 0;
		//				BLERemote.Delay = 0;
		//			}
		//		}
		
#if RFBLE_DOOYA_FUNC
				BLERemote_Deal(&COM_R_DATA[5]);
				COM_T_NUM = 7;
#endif
				return;
			// 模块发送网络状态
			case 0xE0:
				WifiStatus.Now = (Wifi_StatusEnum_TypeDef)(COM_R_DATA[5]);
				COM_T_DATA[2] = 0x04;
				COM_T_NUM = COM_R_NUM - 1;
			break;
	
			case 0xE1:
				if(COM_R_DATA[5] == 0x55 && COM_R_DATA[6] == 0xc2 && COM_R_DATA[7] == 0xda && COM_R_DATA[8] == 0x68)
				{
		#if defined (PROJ_HC32L17x_BOOT_v2) || defined (PROJ_FT32F072_BOOT)
				XYmodem_Start();
				UartUpdate.Mode = UARTUPDATE_UPDATE; 	//进入升级状态
				UartUpdate.Time = 0;

		#endif
					COM_T_NUM = COM_R_NUM;
				}
				break;
			// 恢复出厂设置
			case 0xF0:
				MotorReset();
				SetLedG(3, 3, 5);
				CurtainStatus.Next = STOP_Curtain;
				
				COM_T_NUM = COM_R_NUM;
			break;
	
			default:
				return;
			//break;
		}
	
		TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], COM_T_NUM-2);
		COM_T_DATA[COM_T_NUM-2] = (uint8_t)(TempCrc16>>8);
		COM_T_DATA[COM_T_NUM-1] = (uint8_t)TempCrc16;		
		RequestTransmit(&COM_T, COM_T_NUM);
	}
	// 模块读命令
	else if (COM_R_DATA[3] == 0x02)
	{
		GetAllMotorInformation_UNIFIED();

		switch(COM_R_DATA[4])
		{
			case 0x00:	//读所有信息
			COM_T_DATA[2] = COM_R_DATA[2]+ 0x08;
			memcpy(&COM_T_DATA[5], &VirtualAddr[0], 8);
			TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 13);
			COM_T_DATA[0x0D] = (uint8_t)(TempCrc16>>8);
			COM_T_DATA[0x0E] = (uint8_t)TempCrc16;
			RequestTransmit(&COM_T, 15);
				break;
			case 0x01:	//读电机状态
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = VirtualAddr[0];
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x02:	//读电机百分比位置
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = VirtualAddr[1];
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x03:	//读电机角度
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = VirtualAddr[2];
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0x04:	//读电机是否拥有总行程
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = VirtualAddr[3];
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x05:	//读电机手拉启动是否使能
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = VirtualAddr[4];
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x06:	//读电机方向
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = VirtualAddr[5];
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0x07:	//读弱电开关类型
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = SwitchMod.Weak;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x08:	//读强电开关类型
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = SwitchMod.Power;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0x09:	//打开边界设置状态
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				if(TrackDirection == POSITIVE_DIR)
				{
					COM_T_DATA[5] = Gap.f_ClockWiseGap;
				}
				else
				{
					COM_T_DATA[5] = Gap.f_AntiClockGap;
				}
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x0A:	//关闭边界设置状态
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				if(TrackDirection == POSITIVE_DIR)
				{
				COM_T_DATA[5] = Gap.f_AntiClockGap;
				}
				else
				{
					COM_T_DATA[5] = Gap.f_ClockWiseGap;
				}
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x0B:	//第三行程点是否设置
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];	//功能码
				COM_T_DATA[4] = COM_R_DATA[4];	//数据地址
				COM_T_DATA[5] = Third.Setted_ThirdPosition;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0x0C:  //读角度系数
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];  //功能码
				COM_T_DATA[4] = COM_R_DATA[4];  //数据地址
 				COM_T_DATA[5] = 0xff;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			
			case 0x0D:  //角度方向
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];  //功能码
				COM_T_DATA[4] = COM_R_DATA[4];  //数据地址
 				COM_T_DATA[5] = 0xFF;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0xE0:  //产品标识
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];  //功能码
				COM_T_DATA[4] = COM_R_DATA[4];  //数据地址
 				COM_T_DATA[5] = 0xFF;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0xE1:  //产品密匙
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];  //功能码
				COM_T_DATA[4] = COM_R_DATA[4];  //数据地址
 				COM_T_DATA[5] = 0xFF;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0xF0:  //电机类型
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 0x05;  //长度
				COM_T_DATA[3] = COM_R_DATA[3];  //功能码
				COM_T_DATA[4] = COM_R_DATA[4];  //数据地址
 				COM_T_DATA[5] = 0x10;
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 6);
				COM_T_DATA[0x06] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[0x07] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 8);
				break;
			case 0xF1:  //电机型号
				
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 4 + typelen;
				COM_T_DATA[3] = COM_R_DATA[3];  //功能码
				COM_T_DATA[4] = COM_R_DATA[4];  //数据地址
				memcpy(&COM_T_DATA[5],typestr,typelen);
				
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 5+typelen);
				COM_T_DATA[5+typelen] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[6+typelen] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 7+typelen);
				break;
			case 0xF2:  //软件版本号
				COM_T_DATA[0] = 0x55;
				COM_T_DATA[1] = 0xAA;
				COM_T_DATA[2] = 4 + len;
				COM_T_DATA[3] = COM_R_DATA[3];  //功能码
				COM_T_DATA[4] = COM_R_DATA[4];  //数据地址
				memcpy(&COM_T_DATA[5],softstr,len);
				
				TempCrc16 = CRC16_MODBUS(&COM_T_DATA[0], 5+len);
				COM_T_DATA[5+len] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[6+len] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 7+len);
				break;
			default:
				break;
		}
	}  
	else if (COM_R_DATA[3] == 0x03)
	{
		switch(COM_R_DATA[4])
		{	
			case 0x02:
				if(COM_R_DATA[5] == 1)  //进入配网成功
				{
					WifiStatus.Now = WIFI_ON_LINE_ing;
				}
				else   //失败
				{
//					WifiStatus.Now = WIFI_OFF_LINE;
				}
				break;
			// 模块反馈的网络状态信息
			case 0x04:
				WifiStatus.Now = (Wifi_StatusEnum_TypeDef)(COM_R_DATA[5]);
			break;

			// 模块发出产测结果
			case 0x05:
				if(TestModuleTxRxStatus_2G3G == TEST_MODULE_STATUS_TESTING)
				{
					#if (PCBA_TEST_2G3G == 2)
						// 第二代自动测试判断模块测试结果
					if(COM_R_DATA[5] == 0x00 || ((int8_t)COM_R_DATA[6]) < -80)	//失败
					{
							TestModuleTxRxStatus_2G3G = TEST_MODULE_STATUS_NG;
					}
					else 
					{
							TestModuleTxRxStatus_2G3G = TEST_MODULE_STATUS_PASS;
					}
					#elif (PCBA_TEST_2G3G == 3)
						// 第三代自动测试发送模块测试结果

						// 注意！！！模块的信号强度是以 有符号数 的形式发给测试治具的。所以测试项目中信号强度的上下范围也要是有符号数值。
						SendTestModuleResult_3G(COM_R_DATA[5], COM_R_DATA[6]);
					#endif
				}
			break;
			
			default:
			break;
		}
	}
	
}

/****************************************************************
** 功能描述： 杜亚 RS485 协议中，请求分配地址
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
void RequestDistribute485ID(void)
{
	uint16_t TempCrc16;

	COM_T_DATA[0] = 0x55;
	COM_T_DATA[1] = Motor_ID_L;
	COM_T_DATA[2] = Motor_ID_H;
	COM_T_DATA[3] = 0x04;
	COM_T_DATA[4] = 0x01;
	TempCrc16 = Calc_crc16(0xFFFF, &COM_T_DATA[0], 5);
	COM_T_DATA[5] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[6] = (uint8_t)TempCrc16;
	RequestTransmit(&COM_T, 7);

	f_EnWrite485 = 1;
}

/****************************************************************
** 功能描述： 杜亚 RS485 协议
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
#if RS485_FUNC
static void Dooya_RS485_Protocol(void)
{
	uint16_t TempCrc16;

	// 头码判断
	if (COM_R_DATA[0] != 0x55)
	{
		return;
	}

	// 接收最少数据个数限制
	if (COM_R_NUM <= 5)
	{
		return;
	}

	// RS485 ID 号比较
	if (f_EnWrite485 == 0)		// 正常情况 (不修改电机ID时)，需要对比电机 ID
	{				
		if (COM_R_DATA[1] == 0x00)			 // IDL 为零
		{
			if (COM_R_DATA[2] == 0x00)				 // IDH 为零，可以群控
			{
				if (COM_R_DATA[FUNC_POS] != 0x03)
				{
					return;
				}
			}
			else									// IDH 不为零，可以部分群控
			{
				if (COM_R_DATA[2] != Motor_ID_H)
				{
					return;
				}
				else
				{
					if (COM_R_DATA[FUNC_POS]!=0x03)
					{
						return;
					}
				}
			}
		}
		else								// IDL 不为零
		{
			if (COM_R_DATA[2] == 0x00)				 // IDH 为零
			{
				return;
			}
			else if (COM_R_DATA[2] != 0x00) 		 // IDH 不为零	  可以单控、读、写
			{
				if ((COM_R_DATA[1] != Motor_ID_L) || (COM_R_DATA[2] != Motor_ID_H))
				{
					return;
				}
			}
		}	
	}
	// 如果是修改电机地址，则无需比较电机 ID，但是 命令的地址需要是 0000
	else						
	{
		if ((COM_R_DATA[1] != 0x00) || (COM_R_DATA[2] != 0x00))
		{
			return;
		}
	}
	
	// 校验crc
	if (Calc_crc16(0xFFFF, &(COM_R_DATA[0]), COM_R_NUM-2) != (((uint16_t)COM_R_DATA[COM_R_NUM-2]<<8)+COM_R_DATA[COM_R_NUM-1]))
	{
		return;
	}

	memcpy(&COM_T_DATA[0], &COM_R_DATA[0], COM_R_NUM);

	switch(COM_R_DATA[3])
	{
		// 读
		case 0x01:
			GetAllMotorInformation_RS485();

			// 一次性 最多读写 16 个数据 
			if (COM_R_DATA[5] <= 0x10)
			{
				COM_T_DATA[4] = COM_R_DATA[5];

				memcpy(&COM_T_DATA[5], &VirtualAddr[COM_R_DATA[4]], COM_R_DATA[5]);

				TempCrc16 = Calc_crc16(0xFFFF, &COM_T_DATA[0], 5+COM_R_DATA[5]);
				COM_T_DATA[5+COM_R_DATA[5]] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[6+COM_R_DATA[5]] = (uint8_t)TempCrc16;
				RequestTransmit(&COM_T, 7+COM_R_DATA[5]);
			}
		break;

		// 写
		case 0x02:
			// 一次性 最多读写 16 个数据 
			if (COM_R_DATA[5] <= 0x10)
			{
				// 写ID
				if (f_EnWrite485 == 1)
				{
					if ((V_RS485_ID_L>=COM_R_DATA[4]) && (V_RS485_ID_L<(COM_R_DATA[4]+COM_R_DATA[5])))
					{
						if ((COM_R_DATA[V_RS485_ID_L-COM_R_DATA[4]+6] != 0x00) && (COM_R_DATA[V_RS485_ID_L-COM_R_DATA[4]+6] != 0xFF) &&
							(COM_R_DATA[V_RS485_ID_H-COM_R_DATA[4]+6] != 0x00) && (COM_R_DATA[V_RS485_ID_H-COM_R_DATA[4]+6] != 0xFF))
						{
							Motor_ID_L = COM_R_DATA[V_RS485_ID_L-COM_R_DATA[4]+6];
							Motor_ID_H = COM_R_DATA[V_RS485_ID_H-COM_R_DATA[4]+6];

							Write32BitToEE(ADDR_ID_485_L, ((uint32_t)Motor_ID_H<<8)+Motor_ID_L);
							
							Setting.Task_Setting = MENU_SUCCESS;
						}
					}
				}

			
				// 方向
				if ((V_DIR>=COM_R_DATA[4]) && (V_DIR<(COM_R_DATA[4]+COM_R_DATA[5])))
				{
					if (COM_R_DATA[V_DIR-COM_R_DATA[4]+6] == 0x00)
					{
						ChangeMotorDirection(CLR_CHANGE, CLR_LIMIT);
					}
					else if (COM_R_DATA[V_DIR-COM_R_DATA[4]+6] == 0x01)
					{
						ChangeMotorDirection(SET_CHANGE, CLR_LIMIT);
					}
					else if (COM_R_DATA[V_DIR-COM_R_DATA[4]+6] == 0x02)
					{
						ChangeMotorDirection(TOGGLE_CHANGE, CLR_LIMIT);
					}
				}
				// 手拉
				if ((V_PULLSTART>=COM_R_DATA[4]) && (V_PULLSTART<(COM_R_DATA[4]+COM_R_DATA[5])))
				{
					if (COM_R_DATA[V_PULLSTART-COM_R_DATA[4]+6] == 0x00)
					{
						ChangePullStart(SET_CHANGE);
					}
					else if (COM_R_DATA[V_PULLSTART-COM_R_DATA[4]+6] == 0x01)
					{
						ChangePullStart(CLR_CHANGE);
					}
				}
				// 弱电开关类型
				if ((V_WEAKKEY_MOD>=COM_R_DATA[4]) && (V_WEAKKEY_MOD<(COM_R_DATA[4]+COM_R_DATA[5])))
				{
					if (COM_R_DATA[V_WEAKKEY_MOD-COM_R_DATA[4]+6]<=4)
					{		   
						SwitchMod.Weak = (WeakKeyMod_TypeDef)(COM_R_DATA[V_WEAKKEY_MOD-COM_R_DATA[4]+6]);
						Write32BitToEE(ADDR_WEAK_SW_MOD, (uint32_t)(SwitchMod.Weak));
					}
				}
				// 强电开关类型
				if ((V_POWERKEY_MOD>=COM_R_DATA[4]) && (V_POWERKEY_MOD<(COM_R_DATA[4]+COM_R_DATA[5])))
				{
					if	(COM_R_DATA[V_POWERKEY_MOD-COM_R_DATA[4]+6]<=2)
					{
						SwitchMod.Power = (PowerKeyMod_TypeDef)((COM_R_DATA[V_POWERKEY_MOD-COM_R_DATA[4]+6])+1);
						Write32BitToEE(ADDR_POWER_SW_MOD, (uint32_t)(SwitchMod.Power));
					}
				}

				COM_T_DATA[0x01] = Motor_ID_L;
				COM_T_DATA[0x02] = Motor_ID_H;
				TempCrc16 = Calc_crc16(0xFFFF, &COM_T_DATA[0], 6);
				COM_T_DATA[6] = (uint8_t)(TempCrc16>>8);
				COM_T_DATA[7] = (uint8_t)TempCrc16;

				SetLedG(3, 3, 5);							
				RequestTransmit(&COM_T, 8);
			}
		break;

		// 控制
		case 0x03:
			switch (COM_R_DATA[4])
			{
				// 开
				case 0x01:
					if (Setting.f_Setting)
					{
						break;
					}
					CurtainNextControl(OPENING_Curtain, TRUE);					
				break;

				// 关
				case 0x02:
					if (Setting.f_Setting)
					{
						break;
					}
					CurtainNextControl(CLOSING_Curtain, TRUE);					
				break;

				// 停
				case 0x03:
					CurtainStatus.Next = STOP_Curtain;	 
					ClearMidStop();
				break;

				// 百分比
				case 0x04:
					if (Setting.f_Setting)
					{
						break;
					}
					if (COM_R_DATA[5] >= MAX_PERCENT)
					{
						CurtainNextControl(OPENING_Curtain, TRUE);					
						COM_T_DATA[5] = MAX_PERCENT;
					}
					else if (COM_R_DATA[5] == 0)
					{
						CurtainNextControl(CLOSING_Curtain, TRUE);					
						COM_T_DATA[5] = 0x00;
					}
					else
					{
						if (Pos.f_DetectDistance == 0)				// 找得到行程
						{
							Mid.NextMidPosition = TranslatePercentToPosition(COM_R_DATA[5], MAX_PERCENT);
							Mid.MidPosTask = MID_POS_TASK_CHECK_NEG_DIR;
							
							COM_T_DATA[5] = COM_R_DATA[5];
						}
						else										// 找不到行程
						{
							//ReservedPercent.Value = COM_R_DATA[5];
							//ReservedPercent.Task = Start_RES_PERC;
							
							COM_T_DATA[5] = 0xFF;
						}
					}
				break;

				// 打开边界
				case 0x05:
					if ((COM_R_DATA[5] == 0x00) || (COM_R_DATA[5] == 0x01))
					{
						if (Pos.f_DetectDistance == 0)
						{
							if (COM_R_DATA[5] == 0x01)		// 设置
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeClockWiseGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeAntiClockGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;							 
										SetLedG(1, 1, 10);
									}
								}
							}
							else											// 取消
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeClockWiseGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;							 
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeAntiClockGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;							 
										SetLedG(1, 1, 10);
									}
								}
							}
						}
						else
						{
							COM_T_DATA[5] = 0xFF;
							SetLedG(1, 1, 10);
						}
					}
					else
					{
						SetLedG(1, 1, 10);					
						return;
					}
				break;

				// 关闭边界
				case 0x06:
					if ((COM_R_DATA[5] == 0x00) || (COM_R_DATA[5] == 0x01))
					{
						if (Pos.f_DetectDistance == 0)
						{
							if (COM_R_DATA[5] == 0x01)		// 设置
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeAntiClockGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;							 
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeClockWiseGap_MT(SET_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;							 
										SetLedG(1, 1, 10);
									}
								}
							}
							else											// 取消
							{
								if (TrackDirection == POSITIVE_DIR)
								{
									if (ChangeAntiClockGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;							 
										SetLedG(1, 1, 10);
									}
								}
								else
								{
									if (ChangeClockWiseGap_MT(CLR_CHANGE))
									{
										SetLedG(3, 3, 5);
									}
									else
									{
										COM_T_DATA[5] = 0xFF;							 
										SetLedG(1, 1, 10);
									}
								}
							}
						}
						else
						{
							COM_T_DATA[5] = 0xFF;							 
							SetLedG(1, 1, 10);
						}
					}
					else
					{
						SetLedG(1, 1, 10);						
						return;
					}
				break;

				// 删除行程
				case 0x07:
					ResetLimit();
					SetLedG(3, 3, 5);
					CurtainStatus.Next = STOP_Curtain;
				break;

				// 恢复出厂
				case 0x08:
					MotorReset();
					SetLedG(3, 3, 5);
					CurtainStatus.Next = STOP_Curtain;
				break;
				
				default:
				break;
			}

			// 非群控 才反馈
			if (COM_R_DATA[1] != 0x00)
			{
				if ((COM_R_DATA[4]==RS485_CONTROL_UP) || (COM_R_DATA[4]==RS485_CONTROL_DOWN) || (COM_R_DATA[4]==RS485_CONTROL_STOP) ||
					(COM_R_DATA[4]==RS485_CONTROL_CLR_LIMIT) || (COM_R_DATA[4]==RS485_CONTROL_RESET_FACTORY))
				{
					TempCrc16 = Calc_crc16(0xFFFF, COM_T_DATA, 5);
					COM_T_DATA[5] = (uint8_t)(TempCrc16>>8);
					COM_T_DATA[6] = (uint8_t)TempCrc16;
					RequestTransmit(&COM_T, 7);
				}
				else if ((COM_R_DATA[4]==RS485_CONTROL_PERCENT) || (COM_R_DATA[4]==RS485_CONTROL_SET_CLR_OPEN_GAP) ||
						(COM_R_DATA[4]==RS485_CONTROL_SET_CLR_CLOSE_GAP))
				{
					TempCrc16 = Calc_crc16(0xFFFF, COM_T_DATA, 6);
					COM_T_DATA[6] = (uint8_t)(TempCrc16>>8);
					COM_T_DATA[7] = (uint8_t)TempCrc16;
					RequestTransmit(&COM_T, 8);
				}
			}
		break;

		default:
		break;
	}
}
#endif

unsigned short CRC16(unsigned char const *buffer, unsigned short len)
{
	unsigned short crc = 0x0000;
    
	while (len--)
	{
		crc = (crc >> 8) ^ crc16_table[(crc ^ (*buffer++)) & 0xff];
	}
    
	return crc;
}

// static uint8_t Format_Check(void)
// {

// }


/****************************************************************
** 功能描述： 将字符串转换位8位数据
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
void string_to_bytes(char *str, unsigned char *bytes, int size) 
{
    int i;
    for (i = 0; i < size; i++) {
        bytes[i] = (unsigned char)str[i]; // 转换为8位数据
    }
}	
	
static void YingShiCommonUartProtocol(void)
{
	unsigned short TempCrc16;
	memcpy(&COM_T_DATA[0], &COM_R_DATA[0], 5);//包头、版本号、包序号不变
	memcpy(&COM_T_DATA[8], &COM_R_DATA[8], 3);//数据属性ID和数据属性地址不变
	COM_T_DATA[5] = COM_R_DATA[5]+1;//命令字等于接收的+1
	COM_T_DATA[11] = Success;//先默认结果成功，节省代码行数

	// 头码判断
	if ((COM_R_DATA[0]!=HEAD) || (COM_R_DATA[1]!=HEAD) )
	{
		return;
	}

	// 协议版本号判断
	if (COM_R_DATA[2] != PROTOCOL_VERSION)
	{
		return;
	}
	
	// 包序号判断
	if (PACK_NUM > MAX_PACK_NUM)
	{
		return;
	}

	// 数据载荷长度判断
	if (DATA_LOAD_LONG != COM_R_NUM-10 || DATA_LOAD_LONG % 7 != 0) 
	{
		return;
	}

	// 校验CRC
	if (CRC16(&COM_R_DATA[0],COM_R_NUM-2) != (((uint16_t)COM_R_DATA[COM_R_NUM-2]<<8)+COM_R_DATA[COM_R_NUM-1]))
	{
		return;
	}

	//解析命令
	switch (COMMAND)
	{
	//模组下发配置数据（无属性内容）
	case Config_Data_From_Module:
		;
		uint16_t nature_id;    
		uint8_t data_attribute_type;
		uint16_t data_attribute_long;
		uint8_t Num = (COM_R_NUM-10)/7;
		uint8_t nature_value;
		// COM_T_DATA[6] = 0x00;//赋值数据载荷长度
		// COM_T_DATA[7] = 0x04;//赋值数据载荷长度
		// COM_T_NUM = 14;// 赋值数据字节数

		for (uint8_t num = 0; num < Num; num++)
		{
			nature_id = (((uint16_t)(COM_R_DATA[8+num*7]<<8)) + COM_R_DATA[9+num*7]);
			nature_value = COM_R_DATA[14+num*7];
			COM_T_DATA[6] = (uint8_t)((4*Num)>>8);//赋值数据载荷长度
			COM_T_DATA[7] = (uint8_t)(4*Num);//赋值数据载荷长度
			COM_T_DATA[8+num*4] = (uint8_t)(nature_id>>8);
			COM_T_DATA[9+num*4] = (uint8_t)nature_id;	
			COM_T_DATA[10+num*4] = COM_R_DATA[10+num*7];
			COM_T_DATA[11+num*4] = Success;
			data_attribute_type = COM_R_DATA[11+num*7];
			data_attribute_long = (((uint16_t)(COM_R_DATA[12+num*7]<<8)) + COM_R_DATA[13+num*7]);
			// 窗帘动作
			if (nature_id == Curtain_Action)
			{
				if (data_attribute_type == T_ENUM && data_attribute_long == 0X0001)
				{
					//属性值
					switch (nature_value)
					{
					case Open:
						if (Prot_MotorSetting())
						{
							COM_T_DATA[11+num*4] = Fail;
							break;
						}
						Prot_MotorOpen();
						break;
					
					case Stop:
						Prot_MotorStop();
						Prot_ClearMid();
						break;
					
					case Close:
						if (Prot_MotorSetting())
						{
							COM_T_DATA[11+num*4] = Fail;
							break;
						}
						Prot_MotorClose();
						break;

					default:
						COM_T_DATA[11+num*4] = Para_Error;
						break;
					}
				}
				else
				{
					COM_T_DATA[11+num*4] = Para_Error;
				}	
			}
			// 百分比控制
			else if (nature_id == Percent)
			{
				if (data_attribute_type == T_INT && data_attribute_long == 0X0001)
				{
					if (Prot_MotorSetting())
					{
						COM_T_DATA[11+num*4] = Fail;
					}
					else 
					{
						Prot_PercentControl(nature_value);
					}		
				}
				else
				{
					COM_T_DATA[11+num*4] = Para_Error;
				}		
			}
			// 方向控制
			else if (nature_id == Direction)
			{
				if (data_attribute_type == T_ENUM && data_attribute_long == 0X0001)
				{
					Prot_MotorStop();
					if (nature_value == Forward)//正转
					{
						Prot_Forward();
						Prot_SetLedG(3, 3, 5);
					}
					else if (nature_value == Reversal)//反转
					{
						Prot_Reversal();
						Prot_SetLedG(3, 3, 5);
					}
					else
					{
						COM_T_DATA[11+num*4] = Para_Error;
					} 
				}
				else
				{
					COM_T_DATA[11+num*4] = Para_Error;
				}	
					
				
			}
			// 行程校准
			else if (nature_id == Calibration)
			{
				if (data_attribute_type == T_ENUM && data_attribute_long == 0X0001)
				{
					if (NATURE_VALUE == Calibrate)
					{
						Prot_ResetLimit();					
						Prot_ReservedMaxPercent();
					}
					else
					{
						COM_T_DATA[11+num*4] = Para_Error;
					}
				}
				else
				{
					COM_T_DATA[11+num*4] = Para_Error;
				}
			}
			//遥控器配对
			else if (nature_id == Remote_Pair)
			{
				if (data_attribute_type == T_ENUM && data_attribute_long == 0X0001)
				{
					if (nature_value == 0x01)//遥控器配对
					{
						if (Prot_MotorSetting() == 1)
						{
							COM_T_DATA[11+num*4] = Fail;
						}
						else
						{
							Prot_UartRemoteInPair();
						}
					}
					else if (nature_value == 0x02)//清空遥控器配对配置信息
					{
						#if RF433_FUNC
							Prot_ClearMem();
						#endif	
					}
					else
					{
						COM_T_DATA[11+num*4] = Para_Error;
					}		
				}
				else
				{
					COM_T_DATA[11+num*4] = Para_Error;
				}
			}

		}
		COM_T_NUM = 10+Num*4;
	 	
		
		break;

	//模组读取MCU状态数据
	case Read_Mcu_State:
		// 百分比
		if (NATURE_ID == Percent)
		{
			COM_T_DATA[6] = 0x00;//赋值数据载荷长度
			COM_T_DATA[7] = 0x08;//赋值数据载荷长度
			COM_T_DATA[12] = 0x02;
			COM_T_DATA[13] = 0x00;
			COM_T_DATA[14] = 0x01;
			COM_T_NUM = 18;// 赋值数据字节数
			if (DATA_ATTRIBUTE_TYPE != 0X04 || DATA_ATTRIBUTE_LONG != 0X0001)
			{
				COM_T_DATA[11] = Para_Error;
			}
			else
			{
				if (NATURE_VALUE == 0x01)
				{
					COM_T_DATA[15] = PROT_PERCENT;
				}
				else
				{
					COM_T_DATA[11] = Para_Error;
				}
			}	
		}
		// MCU所有状态
		else if (NATURE_ID == Git_Curtain_All_Status)
		{
			if (NATURE_VALUE != 0x01 || DATA_ATTRIBUTE_TYPE != 0X04 || DATA_ATTRIBUTE_LONG != 0X0001)
			{
				COM_T_DATA[11] = Para_Error;
				COM_T_DATA[6] = 0x00;//赋值数据载荷长度
				COM_T_DATA[7] = 0x04;//赋值数据载荷长度
				COM_T_NUM = 14;// 赋值数据字节数
			}
			else
			{
				COM_T_DATA[6] = 0x00;//赋值数据载荷长度
				COM_T_DATA[7] = 0x24;//赋值数据载荷长度	

				//数据载荷1(窗帘运行状态)
				COM_T_DATA[12] = (uint8_t)(Curtain_Action>>8);
				COM_T_DATA[13] = (uint8_t)Curtain_Action;
				COM_T_DATA[14] = COM_R_DATA[10];
				COM_T_DATA[15] = Success;
				COM_T_DATA[16] = 0x04;
				COM_T_DATA[17] = 0x00;
				COM_T_DATA[18] = 0x01;
				COM_T_DATA[19] = PROT_MOTORSTATUS;

				//数据载荷2(百分比)
				COM_T_DATA[20] = (uint8_t)(Percent>>8);
				COM_T_DATA[21] = (uint8_t)Percent;
				COM_T_DATA[22] = COM_R_DATA[10];
				COM_T_DATA[23] = Success;
				COM_T_DATA[24] = 0x02;
				COM_T_DATA[25] = 0x00;
				COM_T_DATA[26] = 0x01;
				COM_T_DATA[27] = PROT_PERCENT;

				//数据载荷3(方向)
				COM_T_DATA[28] = (uint8_t)(Direction>>8);
				COM_T_DATA[29] = (uint8_t)Direction;
				COM_T_DATA[30] = COM_R_DATA[10];
				COM_T_DATA[31] = Success;
				COM_T_DATA[32] = 0x04;
				COM_T_DATA[33] = 0x00;
				COM_T_DATA[34] = 0x01;
				COM_T_DATA[35] = PROT_DERECTION;

				//数据载荷4(工作状态)
				COM_T_DATA[36] = (uint8_t)(Work_Status>>8);
				COM_T_DATA[37] = (uint8_t)Work_Status;
				COM_T_DATA[38] = COM_R_DATA[10];
				COM_T_DATA[39] = Success;
				COM_T_DATA[40] = 0x04;
				COM_T_DATA[41] = 0x00;
				COM_T_DATA[42] = 0x01;
				if (PROT_MOTORSTATUS == 2)
				{
					COM_T_DATA[43] = 0x00;
				}
				else if (PROT_MOTORSTATUS == 1 || PROT_MOTORSTATUS == 3)
				{
					COM_T_DATA[43] = 0x01;
				}
	
				//数据载荷5(校准状态)
				COM_T_DATA[44] = (uint8_t)(Calibration>>8);
				COM_T_DATA[45] = (uint8_t)Calibration;
				COM_T_DATA[46] = COM_R_DATA[10];
				COM_T_DATA[47] = Success;
				COM_T_DATA[48] = 0x04;
				COM_T_DATA[49] = 0x00;
				COM_T_DATA[50] = 0x01;
				COM_T_DATA[51] = PROT_HASDISTANCE;

				COM_T_NUM = 52;

			}
		}
		// MCU版本号
		else if (NATURE_ID == Git_MCU_Version)
		{
			if (NATURE_VALUE != 0x00 || DATA_ATTRIBUTE_TYPE != 0X03 || DATA_ATTRIBUTE_LONG != 0X0001)
			{
				COM_T_DATA[11] = Para_Error;
				COM_T_DATA[6] = 0x00;//赋值数据载荷长度
				COM_T_DATA[7] = 0x04;//赋值数据载荷长度
				COM_T_NUM = 14;// 赋值数据字节数
			}
			else
			{
				COM_T_DATA[6] = 0x00;//赋值数据载荷长度
				COM_T_DATA[7] = 0x13;//赋值数据载荷长度
				COM_T_DATA[12] = 0x03;
				COM_T_DATA[13] = 0x00;
				COM_T_DATA[14] = 0x0C;
				string_to_bytes(MCU_VERSION, &COM_T_DATA[15], 12);
				COM_T_NUM = 29;
			}
		}

		else
		{
			COM_T_DATA[11] = Para_Error;
			COM_T_DATA[6] = 0x00;//赋值数据载荷长度
			COM_T_DATA[7] = 0x04;//赋值数据载荷长度
			COM_T_NUM = 14;// 赋值数据字节数
		}
	
		break;

	// 模组报送状态数据给MCU
	case Report_Data_To_Mcu:	
		// 模组心跳包
		if (NATURE_ID == Module_Heart_Pack)
		{
			COM_T_DATA[6] = 0x00;//赋值数据载荷长度
			COM_T_DATA[7] = 0x04;//赋值数据载荷长度
			COM_T_NUM = 14;// 赋值数据字节数
			if ((NATURE_VALUE != 0x01 && NATURE_VALUE != 0x02) || DATA_ATTRIBUTE_TYPE != 0X04 || DATA_ATTRIBUTE_LONG != 0X0001)
			{
				COM_T_DATA[11] = Para_Error;
			}
		}
		// 模组联网状态
		else if (NATURE_ID == Module_Network_Status)
		{
			COM_T_DATA[6] = 0x00;//赋值数据载荷长度	
			COM_T_DATA[7] = 0x04;//赋值数据载荷长度
			COM_T_NUM = 14;// 赋值数据字节数
			if ((NATURE_VALUE != 0x01 && NATURE_VALUE != 0x02 && NATURE_VALUE != 0x03) || DATA_ATTRIBUTE_TYPE != 0X04 || DATA_ATTRIBUTE_LONG != 0X0001)
			{
				COM_T_DATA[11] = Para_Error;
			}
			else
			{
				YingShi_WifiStatus.Now = (YingShi_Wifi_StatusEnum_TypeDef)(NATURE_VALUE);
			}
		}

		break;
	//  	

	default:
		break;
	}
	
		
		
	
	






	TempCrc16 = CRC16(&COM_T_DATA[0], COM_T_NUM-2);
	COM_T_DATA[COM_T_NUM-2] = (uint8_t)(TempCrc16>>8);
	COM_T_DATA[COM_T_NUM-1] = (uint8_t)TempCrc16;	
	RequestTransmit(&COM_T, COM_T_NUM);
}

void ProcessReceivePacket0(void)
{
	if (Receive0.f_HasDataToProcesse == 0)
	{
		return;
	}  
	Receive0.f_HasDataToProcesse = 0;


#if RS485_FUNC
		// 杜亚 RS485 协议
		// Dooya_RS485_Protocol();
		
#elif (TUYAWIFIPROTOCOL_FUNC || TUYAZIGBEEPROTOCOL_FUNC)

		if(ProtocolType == TUYA_UART)
		{
			TuyaCommandProcess((void *)(&COM_R_DATA[0]));
		}
#else
		// 杜亚通用串口协议
		DooyaCommonUartProtocol();

		// 杜亚统一串口协议
		DooyaUnifiedProtocol();
		
//		process(Receive0.Data, Receive0.AllNum);

		YingShiCommonUartProtocol();
#endif

#if defined (PROJ_HC32D391_BOOT) || defined (PROJ_N32L40X_BOOT)
	Update_UartDeal(COM_R_DATA);
#endif

	// 测试 占空比与转速的关系
	//DebugCmd();
	
#if RS485_FUNC
	#if (PCBA_TEST_2G3G == 3)
		PCBA_PR_Cmd_3G(&DBG_R, &DBG_T);
	#elif (PCBA_TEST_2G3G == 2)
		PCBA_PR_Cmd_2G(&DBG_R, &DBG_T);
	#endif	

	if (PCBA.Mode == 0)
	{
		PCBA_TestCmd();
	}
#endif

	// 放在最后
	memset(Receive0.Data, 0, sizeof(Receive0.Data));	
	Receive0.IsProcessing = 0;
}

void ProcessReceivePacket1(void)
{
	if (Receive1.f_HasDataToProcesse == 0)
	{
		return;
	}
	Receive1.f_HasDataToProcesse = 0;
	
	#if defined (PROJ_HC32L17x_BOOT_v2) || defined (PROJ_FT32F072_BOOT)
		UartUpdate_Deal(DBG_R_DATA);
#endif

	#if (PCBA_TEST_2G3G == 3)
		PCBA_PR_Cmd_3G(&DBG_R, &DBG_T);
	#elif (PCBA_TEST_2G3G == 2)
		PCBA_PR_Cmd_2G(&DBG_R, &DBG_T);
	#endif
	
	if (PCBA.Mode == 0)
	{
		PCBA_TestCmd();
	}


	// 放在最后
	memset(Receive1.Data, 0, sizeof(Receive1.Data));
	Receive1.IsProcessing = 0;
}




/*********************************END OF FILE**********************************/

