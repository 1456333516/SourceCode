#include "Driver_CAN.h"
#include "string.h"

static void Driver_CAN_ConfigFilter(void);

void Driver_CAN_Init(void)
{
    MX_CAN_Init();

    Driver_CAN_ConfigFilter();

    HAL_CAN_Start(&hcan);
}

static void Driver_CAN_ConfigFilter(void)
{
    CAN_FilterTypeDef filter;
    filter.FilterBank = 0;  /* 过滤器0 */
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;  /* 把过滤器0与fifo0绑定 */
    filter.FilterMode = CAN_FILTERMODE_IDMASK; /* 掩码模式 */
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh =0x0;
    filter.FilterIdLow =0x0;

    filter.FilterMaskIdHigh = 0x0;
    filter.FilterMaskIdLow = 0x0;

    filter.FilterActivation = CAN_FILTER_ENABLE;
    HAL_CAN_ConfigFilter(&hcan, &filter);
}

/**
 * @description: 发送信息
 * @param {uint16_t} id 信息的id
 * @param {uint8_t} *data 数据
 * @param {uint8_t} len 数据的长度(字节个数)
 */
void Driver_CAN_SendMsg(uint16_t id, uint8_t *data, uint8_t len)
{
    if(len > 8) return;
    uint32_t mailBox = 0;
    CAN_TxHeaderTypeDef header;
    header.DLC = len;
    header.StdId = id;
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;

    HAL_CAN_AddTxMessage(&hcan, &header, data, &mailBox);
}

/* 一次性把接收到的所有报文全部读取 */
void Driver_CAN_RecevieMsgs(RxMsgStruct msgs[], uint8_t *count)
{
    /* 获取目前报文数 */
    *count = HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0);
    CAN_RxHeaderTypeDef header;
    for (uint8_t i = 0; i < *count; i++)
    {
        memset(msgs[i].data, 0, 8);


        HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &header, msgs[i].data);
        msgs[i].id = header.StdId;
        msgs[i].len = header.DLC;
    }
}
