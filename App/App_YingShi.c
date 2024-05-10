#include "App_admin.h"
//萤石协议用CRC校验

uint16_t down_config_pro(uint8_t len, uint8_t* dat, uint8_t* tx_buf);

/****************************************************************
** 功能描述： 萤石组包函数
** 输入参数： 
** 返 回 值： 数据载荷长度
** 调用方式： 
** 函数说明： 
*****************************************************************/
uint16_t ack_data_zubao(YingShi_Data_ack_t* res_dat, uint8_t* tx_buf)
{
    YingShi_Data_ack_t* ack = (YingShi_Data_ack_t*)tx_buf;
    ack->para_ID[0] = res_dat->para_ID[0];
    ack->para_ID[1] = res_dat->para_ID[1];
    ack->para_addr = res_dat->para_addr;
    ack->res = res_dat->res;
    ack->data.para_len[0] = res_dat->data.para_len[0];
    ack->data.para_len[1] = res_dat->data.para_len[1];
    uint16_t len = (uint16_t)(res_dat->data.para_len[0] << 8) + res_dat->data.para_len[1];
    if (len == 0)
    {
        return 4;
    }
    else
    {
        ack->data.para_enum = res_dat->data.para_enum;
        memcpy(ack->data.para_dat, res_dat->data.para_dat, len);
        return (4 + 2 + len);
    }
}

/****************************************************************
** 功能描述： 解码
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
uint16_t decode_data(uint8_t ctr, uint8_t* dat, uint16_t len, uint8_t* tx_buf)
{
    uint16_t tx_len;
    switch (ctr)
    {
    case MODULE_TO_CURTIAN_CONFIG:
        tx_len = down_config_pro(len, dat, tx_buf);
        break;
    case MODULE_TO_CURTIAN_READ:
        tx_len = down_config_pro(len, dat, tx_buf);
        break;    
    case MODULE_TO_CURTIAN_HEART:
        tx_len = down_config_pro(len, dat, tx_buf);
        break; 
    default:
        break;
    }
    return tx_len;
}

/****************************************************************
** 功能描述： 协议解析函数
** 输入参数： 
** 返 回 值： 
** 调用方式： 
** 函数说明： 
*****************************************************************/
void process(uint8_t* dat, uint16_t len)
{
    uint16_t tx_len;
    uint16_t pack_num;//包序号
    uint16_t data_load_long;//数据载荷长度
    uint8_t temp_buf[100];
    uint8_t* buffer = &temp_buf[8];
    static uint8_t save_mid[2];
    // uint8_t protocol_version;
    uint8_t ctr;
    uint8_t* data;

    // 指向COM首地址

    YingShi_Data_FrameHead_t* head = (YingShi_Data_FrameHead_t*)dat;
    pack_num = (uint16_t)(head->mid[0]<<8) + head->mid[1];
    data_load_long = (uint16_t)(head->length[0]<<8) + head->length[1];//记录数据载荷长度
		YingShi_Data_FrameTail_t* tail = (YingShi_Data_FrameTail_t*)(dat+len-2);
    // 头码判断
    if (head->head[1] != Head_L || head->head[1] != Head_H)
			
        return;
    // 协议版本号判断
    // if (head->version != PROTOCOL_VERSION)
    //     return;
    // 包序号判断
    if (pack_num > MAX_PACK_NUM)
        return;
    // 数据载荷长度判断
    if (data_load_long != len-10)
        return;
    // 校验CRC
    tail = (YingShi_Data_FrameTail_t*)((uint8_t*)head + sizeof(YingShi_Data_FrameHead_t) + data_load_long);
    if (CRC16((unsigned char*)head, len-2) != ((uint16_t)(tail->crc[0] << 8) + tail->crc[1]))
        return;
    
    //记录收到的协议版本号
    // protocol_version = head->version;

    //记录收到的包序号
    save_mid[0] = head->mid[0];
    save_mid[1] = head->mid[1];
    
    ctr = head->ctr;//记录命令字
    data = (uint8_t*)((uint8_t*)head + sizeof(YingShi_Data_FrameHead_t)); //记录数据载荷首地址

    tx_len = decode_data(ctr, data, data_load_long, buffer); //解码得到要发送的数据载荷长度 

    // 组包
    YingShi_Data_FrameHead_t* tx_buf_head = (YingShi_Data_FrameHead_t*)temp_buf;
    YingShi_Data_FrameTail_t* tx_buf_tail = (YingShi_Data_FrameTail_t*)&temp_buf[7+tx_len];
    tx_buf_head->head[0] = Head_L;
    tx_buf_head->head[1] = Head_L;
    tx_buf_head->version = head->version;
    tx_buf_head->mid[0] = save_mid[0];
    tx_buf_head->mid[1] = save_mid[1];
    tx_buf_head->ctr = ctr+1;
    tx_buf_head->length[0] = (uint8_t)(tx_len>>8);
    tx_buf_head->length[1] = (uint8_t)tx_len;

    tx_buf_tail->crc[0] = (uint8_t)(CRC16(temp_buf, 8+tx_len)>>8);
    tx_buf_tail->crc[1] = (uint8_t)CRC16(temp_buf, 8+tx_len);
    tx_len += 10;
//    Transmit_TypeDef Transmit;
//    Transmit.AllNum = tx_len;
    memcpy(Transmit0.Data, temp_buf, tx_len);
		
    //调用发送
    RequestTransmit(&Transmit0, tx_len);
}

/****************************************************************
** 功能描述： 下发控制指令的处理
** 输入参数： 
** 返 回 值：要回复的数据载荷长度 
** 调用方式： 
** 函数说明： 
*****************************************************************/
uint16_t down_config_pro(uint8_t len, uint8_t* dat, uint8_t* tx_buf)
{
    uint16_t tx_len = 0;
    uint16_t rx_len = 0;//记录的数据载荷长度
    uint16_t data_para_len = 0;//数据属性长度
    uint8_t res = Success;//应答结果（先默认成功，节省代码）
    uint8_t res_data[50] = {0};
    YingShi_Data_ack_t* res_dat;
    res_dat = (YingShi_Data_ack_t*) &res_data[0];//应答数据载荷首地址
    do{
        YingShi_Data_receive_t* receive = (YingShi_Data_receive_t*)(dat + rx_len);
        uint16_t ID = (uint16_t)(receive->para_ID[0] << 8) + receive->para_ID[1];//得到属性ID号
         data_para_len += (uint16_t)(receive->data.para_len[0] << 8) + receive->data.para_len[1];//得到数据属性长度
        //解析属性ID号
        switch (ID)
        {
        case ID_CURTAIN_ACTION:
            if (receive->data.para_enum == T_ENUM)
            {
                if (data_para_len == 1)
                {
                    if (receive->data.para_dat[0] == 1)
                    {
                        if (Prot_MotorSetting())
                        {
                            res = Fail;
                        }
                        else
                        {
                            // 开电机
                            Prot_MotorOpen();    
                        }
                        
                    }
                    else if (receive->data.para_dat[0] == 2)
                    {
                        // 停电机
                        Prot_MotorStop();
                        Prot_ClearMid();
                    }
                    else if (receive->data.para_dat[0] == 3)
                    {
                        if (Prot_MotorSetting())
                        {
                            res = Fail;
                        }
                        else
                        {
                            // 关电机
                            Prot_MotorClose();
                        } 
                    } 
                    else
                    {
                        res = Para_Error;
                    }
                }
                else
                {
                    res = Para_Error;
                }
            }
            else
            {
                res = Para_Error;
            }
            break;

        case ID_CURTAIN_READ_PERCENT:
            if (receive->data.para_enum == T_INT)
            {
                if (data_para_len == 1)
                {
                    if (Prot_MotorSetting())
                    {
                        res = Fail;
                    }
                    else
                    {
                        Prot_PercentControl(receive->data.para_dat[0]);
                    }
                }
                else
                {
                    res = Para_Error;
                }
            }
            else
            {
                res = Para_Error;
            }
            break;

        default:
            break;

        }  
        memset(res_data, 0, sizeof(res_data));
        res_dat->para_ID[0] = receive->para_ID[0];
        res_dat->para_ID[1] = receive->para_ID[1];
        res_dat->para_addr = receive->para_addr;   
        res_dat->res = res; 
        res_dat->data.para_len[0] = 0;  
        tx_len += ack_data_zubao(res_dat, tx_buf + tx_len);//得到要回复的数据载荷长度
        if (data_para_len != 0)
            rx_len += data_para_len + 7;//得到接收到的数据载荷长度
        else
            rx_len += 4; 
    }while(rx_len <= len);
    return tx_len; 
}

uint16_t down_read_pro(uint8_t len, uint8_t* dat, uint8_t* tx_buf)
{
    uint16_t tx_len = 0;
	        uint8_t percent;
    uint8_t res = 1;
    uint8_t res_data[20] = {0};
    YingShi_Data_ack_t* res_dat;
    YingShi_Data_receive_t* send = (YingShi_Data_receive_t*)dat;
    res_dat = (YingShi_Data_ack_t*) &res_data[0];
    uint16_t ID = (uint16_t)(send->para_ID[0] << 8) + send->para_ID[1];
    switch (ID)
    {
    case ID_CURTAIN_READ_PERCENT:
        if (send->data.para_enum == T_ENUM)
        {
            if ((uint16_t)(send->data.para_len[0] << 8) + send->data.para_len[1] == 1)
            {
                if (send->data.para_dat[0] == 1)
                {
                    res = 0;
                    //读取percent;
                }
            }
        }
        memset(res_data, 0, sizeof(res_data));
        res_dat->para_ID[0] = send->para_ID[0];
        res_dat->para_ID[1] = send->para_ID[1];
        res_dat->para_addr = send->para_addr;   
        res_dat->res = res; 
        res_dat->data.para_enum = T_INT;
        res_dat->data.para_len[0] = 0;
        res_dat->data.para_len[1] = 1;
        res_dat->data.para_dat[0] = percent;
        tx_len += ack_data_zubao(res_dat, tx_buf + tx_len);
        break;
    case ID_CURTAIN_READ_ALL:
        if (send->data.para_enum == T_ENUM)
        {
            if ((uint16_t)(send->data.para_len[0] << 8) + send->data.para_len[1] == 1)
            {
                if (send->data.para_dat[0] == 1)
                {
                    memset(res_data, 0, sizeof(res_data));
                    res_dat->para_ID[0] = send->para_ID[0];
                    res_dat->para_ID[1] = send->para_ID[1];
                    res_dat->para_addr = send->para_addr;   
                    res_dat->res = 0; 
                    tx_len += ack_data_zubao(res_dat, tx_buf + tx_len);
                    // 电机百分比
                    memset(res_data, 0, sizeof(res_data));
                    res_dat->para_ID[0] = (uint8_t)(ID_CURTAIN_READ_PERCENT >> 8);
                    res_dat->para_ID[1] = (uint8_t)(ID_CURTAIN_READ_PERCENT);
                    res_dat->para_addr = send->para_addr; 
                    if (percent == 0xff)
                    {
                        res_dat->res = 1; 
                    }
                    else
                    {
                        res_dat->res = 0;
                        res_dat->data.para_enum = T_INT;
                        res_dat->data.para_len[0] = 0;
                        res_dat->data.para_len[1] = 1;
                        res_dat->data.para_dat[0] = percent;
                    }  
                    tx_len += ack_data_zubao(res_dat, tx_buf + tx_len);  
                }
            }
        }
    case ID_CURTAIN_READ_MOUDLUE_VERSION:
        if (send->data.para_enum == T_STR)
        {
            if ((uint16_t)(send->data.para_len[0] << 8) + send->data.para_len[1] == 1)
            {
                if (send->data.para_dat[0] == 0)
                {
                    res = 0;
                    //读取percent;
                }
            }
        }
        memset(res_data, 0, sizeof(res_data));
        res_dat->para_ID[0] = send->para_ID[0];
        res_dat->para_ID[1] = send->para_ID[1];
        res_dat->para_addr = send->para_addr;   
        res_dat->res = res; 
        res_dat->data.para_enum = T_STR;
        res_dat->data.para_len[0] = 0;
        res_dat->data.para_len[1] = 0x0C;
				string_to_bytes(MCU_VERSION, &res_dat->data.para_dat[0], 12);
//        memcpy(&res_dat->data.para_dat[0],version,sizeof(version));
        tx_len += ack_data_zubao(res_dat, tx_buf + tx_len);
        break;    
    default:
        break;
    }   
    return tx_len; 
}

uint16_t down_heart_pro(uint8_t len, uint8_t* dat, uint8_t* tx_buf)
{
    uint16_t tx_len = 0;
    uint8_t res = 1;
    uint8_t res_data[50] = {0};
    YingShi_Data_ack_t* res_dat;
    YingShi_Data_receive_t* send = (YingShi_Data_receive_t*)dat;
    res_dat = (YingShi_Data_ack_t*) &res_data[0];
    uint16_t ID = (uint16_t)(send->para_ID[0] << 8) + send->para_ID[1];
    switch (ID)
    {
    case ID_CURTAIN_HEART:
        if (send->data.para_enum == T_ENUM)
        {
            if ((uint16_t)(send->data.para_len[0] << 8) + send->data.para_len[1] == 1)
            {
                if (send->data.para_dat[0] == 1)
                {
                    res = 0;
                    // 开电机
                }
            }
        }
        memset(res_data, 0, sizeof(res_data));
        res_dat->para_ID[0] = send->para_ID[0];
        res_dat->para_ID[1] = send->para_ID[1];
        res_dat->para_addr = send->para_addr;   
        res_dat->res = res; 
        res_dat->data.para_len[0] = 0;  
        tx_len += ack_data_zubao(res_dat, tx_buf + tx_len);
        break;
    case ID_CURTAIN_MOD_STATUS:
        if (send->data.para_enum == T_ENUM)
        {
            if ((uint16_t)(send->data.para_len[0] << 8) + send->data.para_len[1] == 1)
            {
                if (send->data.para_dat[0] == 1)
                {
                    res = 0;
                    // 更新联网状态
                }
            }
        }
        memset(res_data, 0, sizeof(res_data));
        res_dat->para_ID[0] = send->para_ID[0];
        res_dat->para_ID[1] = send->para_ID[1];
        res_dat->para_addr = send->para_addr;   
        res_dat->res = res; 
        res_dat->data.para_len[0] = 0;  
        tx_len += ack_data_zubao(res_dat, tx_buf + tx_len);
        break;
    default:
        break;
    }   
    return tx_len; 
}

