#include "rfid_app.h"

//以下是空闲中断所需要定义的变量
uint8_t rfid_rx_dma_buffer[512] = {0};
uint8_t rfid_dma_buffer[512] = {0};
struct rt_ringbuffer rfid_ringbuffer;//定义一个结构体
uint8_t rfid_ringbuffer_pool[512];//定义一个循环缓冲区的数组


// 定义已知卡片的ID（根据您提供的Hex数据提取）
const unsigned char CARD_1_ID[] = {0xB3, 0x85, 0x7A, 0x5C};
const unsigned char CARD_2_ID[] = {0xC3, 0xCD, 0x72, 0x28};

/**
 * @brief RFID数据解析函数
 * @param buffer 串口接收到的原始字节数组
 * @param len 接收到的数据长度
 */
void parse_rfid_data(unsigned char *buffer, int len) {
    // 1. 基础校验：主动模式上传卡号固定为12字节 
    if (len < 12) return;

    // 2. 协议头与模式校验
    // buffer[0] 为命令类型 0x04, buffer[2] 为工作模式 0x02 
    if (buffer[0] == 0x04 && buffer[2] == 0x02) {
        
        // 3. 状态位校验：buffer[4] == 0x00 表示读取成功 
        if (buffer[4] != 0x00) return;

        // 4. 提取卡号：根据文档，ID号位于包的后部（偏移量7-10） 
        // 注意：第5-6字节通常为卡类型（如 04 00），第7-10字节为真正的4字节ID [cite: 133]
        unsigned char *current_card_id = &buffer[7];

        // 5. 匹配卡片
        if (memcmp(current_card_id, CARD_1_ID, 4) == 0) {
            my_printf(&huart1,"卡一刷卡\n");
        } 
        else if (memcmp(current_card_id, CARD_2_ID, 4) == 0) {
            my_printf(&huart1,"卡二刷卡\n");
        }
    }
}

/**
 * @brief RFID任务处理函数
 * 
 * 该函数负责处理RFID数据的接收、解析和处理。从环形缓冲区中读取RFID数据，
 * 查找有效的数据帧（以0x04开头，长度为12字节），并调用解析函数处理数据。
 * 
 * @param void 无参数
 * @return void 无返回值
 */
void rfid_task(void)
{
    uint16_t length;
    length = rt_ringbuffer_data_len(&rfid_ringbuffer);
    
    // 主动模式下，一帧完整数据固定为 12 字节 
    if (length < 12) return; 

    // 从环形缓冲区提取数据到处理缓冲区
    rt_ringbuffer_get(&rfid_ringbuffer, rfid_dma_buffer, length);

    // 遍历数据，寻找包头 0x04 并调用解析函数
    for (int i = 0; i <= (length - 12); i++) 
    {
        // 发现主动上传包头 0x04 且长度位匹配 0x0C 
        if (rfid_dma_buffer[i] == 0x04 && rfid_dma_buffer[i+1] == 0x0C) 
        {
            // 将当前帧的起始地址传给解析函数
            parse_rfid_data(&rfid_dma_buffer[i], 12);
            
            // 处理完一帧，跳过这 12 字节
            i += 11; 
        }
    }

    // 清空缓冲区，准备下次接收
    memset(rfid_dma_buffer, 0, sizeof(rfid_dma_buffer));
}





