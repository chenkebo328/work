#ifndef APP_YINGSHI_H
#define APP_YINGSHI_H

#include "App_admin.h"


#define Head_L 0x5a
#define Head_H 0x5a
#define MAX_PACK_NUM 0XFF


typedef struct 
{
    uint8_t head[2];
    uint8_t version;
    uint8_t mid[2];
    uint8_t ctr;
    uint8_t length[2];
}YingShi_Data_FrameHead_t;//数据框架头

typedef struct
{
   uint8_t crc[2];
}YingShi_Data_FrameTail_t;//数据框架尾

typedef enum
{
    T_RAW = 0,
    T_BOOL,
    T_INT,
    T_STR,
    T_ENUM,
    T_BITMAP,
}YingShi_Data_para_enum_t;//数据属性类型

typedef enum
{
    ID_CURTAIN_HEART = 0X0000,
    ID_CURTAIN_MOD_STATUS = 0X0004,
    ID_CURTAIN_READ_ALL = 0X0008,
    ID_CURTAIN_READ_MOUDLUE_VERSION = 0X000D,
    ID_CURTAIN_ACTION = 0x0066,
    ID_CURTAIN_READ_PERCENT = 0x0067,
}YingShi_Data_id_enum_t;//数据属性ID

typedef enum
{
    MODULE_TO_CURTIAN_CONFIG = 1,
    MODULE_TO_CURTIAN_READ = 3,
    MODULE_TO_CURTIAN_HEART = 5,
}YingShi_Data_ctr_enum_t;//数据命令字ID

typedef struct 
{
    YingShi_Data_para_enum_t para_enum;
    uint8_t para_len[2];
    uint8_t para_dat[1];//这里先设置一个字节
}YingShi_para_data_t;//数据属性内容

typedef struct 
{
    uint8_t para_ID[2];
    uint8_t para_addr;
    YingShi_para_data_t data;
}YingShi_Data_receive_t;//发送指令的数据载荷
    
typedef struct 
{
    uint8_t para_ID[2];
    uint8_t para_addr;
    uint8_t res;
    YingShi_para_data_t data;
}YingShi_Data_ack_t;//应答指令的数据载荷

extern void process(uint8_t* dat, uint16_t len);

#endif


