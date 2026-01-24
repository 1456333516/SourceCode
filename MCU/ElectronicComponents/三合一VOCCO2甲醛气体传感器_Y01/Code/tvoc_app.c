/*
 * @Descripttion: 
 * @version: 
 * @Author: tlm
 * @Date: 2026-01-24 20:42:39
 * @LastEditors: tlm
 * @LastEditTime: 2026-01-24 20:56:26
 */
 #include "tvoc_app.h"


//以下是空闲中断所需要定义的变量
uint8_t tvoc_rx_dma_buffer[512] = {0};
uint8_t tvoc_dma_buffer[512] = {0};


//第一步，先定义以下变量
//以下是空闲中断+环形缓冲区所需要的定义的变量
struct rt_ringbuffer tvoc_ringbuffer;//定义一个结构体
uint8_t tvoc_ringbuffer_pool[512];//定义一个循环缓冲区的数组
//还需要定义一个从环形缓冲区拿出来存放的数组，用来解析使用，不过这里为了方便，就直接使用uart_dma_buffer[128]

//第二步，在main.c里面使用rt_ringbuffer_init(&uart_ringbuffer,ringbuffer_pool,sizeof(ringbuffer_pool));//初始化环形缓冲区 
//第一个参数是定义环形缓冲区的结构体的地址，第二个传入环形缓冲区的数组,第三个是传入环形缓冲区的大小。



AirQuality_Data_t current_air_quality;
/**
 * @brief  解析 MK245 模组数据
 * @param  raw_buffer: 接收到的原始数据数组 (至少9字节)
 * @param  result:     用于输出解析结果的结构体指针
 * @return 0: 解析成功, -1: 帧头错误, -2: 校验和错误
 */
int8_t MK245_Parse_Data(uint8_t *raw_buffer, AirQuality_Data_t *result)
{
    // 1. 校验帧头 (B1, B2)
    // 文档规定固定地址为 0x2C, 0xE4 [cite: 24]
    if (raw_buffer[0] != 0x2C || raw_buffer[1] != 0xE4) {
        return -1; // 帧头不匹配
    }

    // 2. 计算校验和 (Checksum)
    // 公式: B9 = unit_8(B1+B2+B3+B4+B5+B6+B7+B8) 
    uint8_t cal_checksum = 0;
    for (int i = 0; i < 8; i++) {
        cal_checksum += raw_buffer[i];
    }

    // 比较计算出的校验和与接收到的第9个字节 (B9)
    if (cal_checksum != raw_buffer[8]) {
        return -2; // 校验和错误
    }

    // 3. 数据转换
    
    // TVOC 计算: (高字节*256 + 低字节) * 0.001 [cite: 31]
    uint16_t tvoc_raw = (raw_buffer[2] << 8) | raw_buffer[3];
    result->tvoc_mg_m3 = (float)tvoc_raw * 0.001f;

    // 甲醛 (HCHO) 计算: (高字节*256 + 低字节) * 0.001 [cite: 32]
    // 文档中表格标记为 CH20，实际指 HCHO
    uint16_t hcho_raw = (raw_buffer[4] << 8) | raw_buffer[5];
    result->hcho_mg_m3 = (float)hcho_raw * 0.001f;

    // CO2 计算: 高字节*256 + 低字节 
    // 注意：CO2 单位是 ppm，不是 mg/m3，且不需要乘以 0.001
    result->co2_ppm = (raw_buffer[6] << 8) | raw_buffer[7];

    return 0; // 解析成功
}
void tvoc_task(void)
{
	uint16_t length;
	length = rt_ringbuffer_data_len(&tvoc_ringbuffer);
	if(length == 0) return;
	rt_ringbuffer_get(&tvoc_ringbuffer,tvoc_dma_buffer,length);
	//my_printf(&huart1, "tvoc data: %s\r\n", tvoc_dma_buffer);
	// 调用解析函数，传入缓冲区地址
	int8_t status = MK245_Parse_Data(tvoc_dma_buffer, &current_air_quality);

	if (status == 0) {
			// 解析成功，可以使用 current_air_quality 中的数据了
			// 例如打印出来：
			my_printf(&huart1,"TVOC: %.3f mg/m3, HCHO: %.3f mg/m3, CO2: %d ppm\r\n", 
			        current_air_quality.tvoc_mg_m3, 
			        current_air_quality.hcho_mg_m3, 
			        current_air_quality.co2_ppm);
	} else {
			// 处理错误（如清空缓冲区或请求重发）
	}
	// 清空接收缓冲区，将接收索引置零
	memset(tvoc_dma_buffer, 0, sizeof(tvoc_dma_buffer));
}









