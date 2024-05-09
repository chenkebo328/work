
/******************************************************************************
作者： name
时间： 2020.02.26
功能： 
说明： 	
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MY_PROTOCOL_H
#define MY_PROTOCOL_H


/* Includes ------------------------------------------------------------------*/
#include "App_admin.h"

/* Exported Macro Define -----------------------------------------------------*/

// 调试、测试串口
//#if RS485_FUNC
//#define DBG_T				Transmit0
//#define DBG_T_DATA			Transmit0.Data
//#define DBG_T_NUM			Transmit0.AllNum
//#define DBG_T_PENDING		Transmit0.f_HasDataToSend

//#define DBG_R				Receive0
//#define DBG_R_DATA			Receive0.Data
//#define DBG_R_NUM			Receive0.AllNum
//#else
#define DBG_T				Transmit1
#define DBG_T_DATA			Transmit1.Data
#define DBG_T_NUM			Transmit1.AllNum
#define DBG_T_PENDING		Transmit1.f_HasDataToSend

#define DBG_R				Receive1
#define DBG_R_DATA			Receive1.Data
#define DBG_R_NUM			Receive1.AllNum
//#endif
// 通用串口
#define COM_T				Transmit0
#define COM_T_DATA			Transmit0.Data
#define COM_T_NUM			Transmit0.AllNum
#define COM_T_PENDING		Transmit0.f_HasDataToSend
#define COM_TX_SPACETIME   Tx0.SpaceTime

#define COM_R				Receive0
#define COM_R_DATA			Receive0.Data
#define COM_R_NUM			Receive0.AllNum


// 接收 和 发送 数组中的 位置
#define HEAD_POS          0
#define IDL_POS           1
#define IDH_POS           2
#define FUNC_POS          3
#define DATA_INFO_POS     4


// UART串口电机 部分的宏定义

// 功能码
#define RS485_HEAD                      0x55

#define RS485_READ                      0x01        // 读
#define RS485_WRITE                     0x02        // 写
#define RS485_CONTROL                   0x03        // 控制
#define RS485_REPORT                    0x04        // 电机主动上报

// 控制命令
#define RS485_CONTROL_UP                0x01
#define RS485_CONTROL_DOWN              0x02
#define RS485_CONTROL_STOP              0x03
#define RS485_CONTROL_PERCENT           0x04
#define RS485_CONTROL_SET_CLR_OPEN_GAP  0x05
#define RS485_CONTROL_SET_CLR_CLOSE_GAP 0x06
#define RS485_CONTROL_CLR_LIMIT         0x07        // 删除行程限位点，删除上下行程点时，会删除所有的行程点信息。
#define RS485_CONTROL_RESET_FACTORY     0x08

// 读写虚拟地址
// VirtualAddr 内存内部的虚拟地址
#define V_RS485_ID_L        0x00
#define V_RS485_ID_H        0x01
#define V_CUR_PERCENT       0x02
#define V_DIR               0x03
#define V_PULLSTART         0x04
#define V_MOTOR_STATE       0x05
#define V_HAVE_ALL_LIMIT    0x09
#define V_WEAKKEY_MOD       0x27
#define V_POWERKEY_MOD      0x28
#define V_MOTOR_BIG_TYPE    0xF0
#define V_FACTORY_CODE      0xFC
#define V_MOTOR_VERSION     0xFD
#define V_PROTOCOL_VERSION  0xFE



#define FIXED_REPORT_TIME      100          // 100*10ms 主动上报

/*萤石通用模组串口通信协议*/
#define HEAD              0X5A
#define PROTOCOL_VERSION  0X01   //协议版本
#define MAX_PACK_NUM      0XFF   //最大包序号（从0开始递增，超过255后继续从0开始）
#define PACK_NUM          (((uint16_t)(COM_R_DATA[3]<<8)) + COM_R_DATA[4])
#define COMMAND           COM_R_DATA[5]
#define NATURE_ID         (((uint16_t)(COM_R_DATA[8]<<8)) + COM_R_DATA[9])
#define DATA_ATTRIBUTE_TYPE         COM_R_DATA[11]
#define DATA_ATTRIBUTE_LONG         (((uint16_t)(COM_R_DATA[12]<<8)) + COM_R_DATA[13])
#define NATURE_VALUE      COM_R_DATA[14]
#define NATURE_ADDRESS    0X00
#define DATA_LOAD_LONG    (((uint16_t)COM_R_DATA[6]<<8)+COM_R_DATA[7])  //数据载荷长度

#define MCU_VERSION       "EZ_00_V1.0.0" //MCU版本
 


typedef enum
{
	Config_Data_From_Module = 0x01,   //模组下发配置数据给MCU
	Ack_Config_Data_From_Module,      //MCU对下发配置指令的应答
	Read_Mcu_State,                   //模组读取MCU状态数据
	Ack_Read_Mcu_State,				  //MCU对读取配置指令的应答
	Report_Data_To_Mcu,               //模组报送状态数据给MCU
	Ack_Report_Data_To_Mcu,           //MCU对报送状态指令的应答
	Config_Data_From_Mcu,             //MCU发送配置数据给WIFI模组
	Ack_Config_Data_From_Mcu,         //模组对配置指令的应答
	Read_Module_State,                //MCU读取WIFI模组状态数据
	Ack_Read_Module_State,            //模组对读取状态指令的应答
	Report_State_To_Module,           //MCU上报状态数据给WIFI模组
	Ack_Report_State_To_Module        //模组对上报状态指令的应答
}CommandIDEnum_TypeDef;//命令字ID

typedef enum
{
	// Module_Heart_Pack = 0x00,//模组心跳包
	// Version_Num,             //获取/上报版本号
	// Serial_Num,              //获取/上报模组序列号
	// Module_Verifi_Code,      //获取/上报模组验证码
	// 
	// Module_Mode,             //控制/获取/上报WiFi模式
	// Device_Time,             //控制/获取/上报设备时间
	// Reset_Module,			 //控制重置模组		
	// Device_Status_Infor,     //获取/上传设备所有状态信息

	Module_Heart_Pack = 0x0000,        //模组心跳包
	Module_Network_Status = 0x0004,    //获取/上报模组联网状态
	Reset_Module = 0x0007,             //控制重置模组
	Git_Curtain_All_Status = 0x0008,
	Git_MCU_Version = 0x000D,
	Remote_Pair = 0x000E,
	Power_Switch = 0x0065,
	Curtain_Action,
	Percent, 
	Direction,
	Work_Status,
	Calibration
}NatureIDEnum_TypeDef;//属性ID

typedef enum
{
	Open = 1,
	Stop,
	Close,
	Continue
}CurtainActionEnum_TypeDef;//窗帘动作值

typedef enum
{
	Reversal = 0, //反转
	Forward       //正转
}DerectionEnum_TypeDef;//窗帘方向

typedef enum
{
	Uncalibrated = 0, //未校准
	Calibrate,      //执行校准
	Calibration_Comple //校准完成
}CalibrationStatusEnum_TypeDef;//校准状态

typedef enum
{
	Success = 0X00,
	Overtime,
	Para_Error,
	Fail
}AckResult_TypeDef;//应答结果

// typedef enum
// {
//     T_RAW = 0,
//     T_BOOL,
//     T_INT,
//     T_STR,
//     T_ENUM,
//     T_BITMAP,
// }YingShi_Data_para_enum_t;//数据属性类型
/****************************************************************/


/* Exported Types Define -----------------------------------------------------*/
typedef enum
{
	WU_Protocol = 0,
	DOOYA_COMMON_UART = 1,
	DOOYA_UNIFIED_UART = 2,
	
}ProtocolType_TypeDef;

typedef enum
{
	TEST_MODULE_STATUS_IDLE = 0,		// 模块测试空闲
	TEST_MODULE_STATUS_TESTING = 1, 	// 模块开始测试，测试中
	TEST_MODULE_STATUS_PASS = 2,		// 模块测试成功
	TEST_MODULE_STATUS_NG = 3			// 模块测试失败

}TEST_MODULE_TXRX_STATUS_2G3G;

/* Exported Variable Declare -------------------------------------------------*/
extern uint8_t f_EnWrite485;
extern uint8_t Motor_ID_L;
extern uint8_t Motor_ID_H;

extern ProtocolType_TypeDef ProtocolType;
extern TEST_MODULE_TXRX_STATUS_2G3G TestModuleTxRxStatus_2G3G;

/* Exported functions Declare ------------------------------------------------*/
extern void PeiWang(void);

extern void ProcessReceivePacket1(void);
extern void ProcessReceivePacket0(void);
extern void RequestDistribute485ID(void);
extern void CheckAndShowWifiStatus(void);
extern void TestModuleTxRx_2G3G(void);
extern void ModelReset(void);

extern unsigned short CRC16(unsigned char const *buffer, unsigned short len);
extern void ShowWifiStatus_YingShi(void);
extern void PeiWang_YingShi(void);
extern void ModelReset_YingShi(void);
extern void string_to_bytes(char *str, unsigned char *bytes, int size); 

#endif
/*********************************END OF FILE**********************************/

