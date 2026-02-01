#include "Driver_CAN.h"
#include "string.h"

static void Driver_CAN_ConfigFilter(void);

void Driver_CAN_Init(void)
{
    /* 1. 开启时钟 */
    RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN);
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    /* 2. 配置和TX和RX的工作模式  重映射 */
    AFIO->MAPR &= ~AFIO_MAPR_CAN_REMAP;
    AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_REMAP2;

    /* pb8->Rx  浮空输入  mode=00 cnf=01 */
    /* pb9->Tx  复用推挽  mode=11 cnf=10 */
    GPIOB->CRH |= (GPIO_CRH_CNF8_0 | GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1);
    GPIOB->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_0);

    /* 3. CAN的配置 */
    /* 3.1 软重启CAN,  会进入睡眠模式 */
    CAN1->MCR |= CAN_MCR_RESET;

    /* 等待进入睡眠 */
    while((CAN1->MSR & CAN_MSR_SLAK) == 0);

    /* 3.2 进入初始化模式, 然后才可以对CAN进行配置  */
    CAN1->MCR &= ~CAN_MCR_SLEEP;            /* 退出睡眠模式 */
    while((CAN1->MSR & CAN_MSR_SLAK) != 0); /* 等待退出睡眠 */

    CAN1->MCR |= CAN_MCR_INRQ;              /* 进入初始化 */
    while((CAN1->MSR & CAN_MSR_INAK) == 0); /* 等待进入初始化 */

    /* 3.3 自动离线管理 */
    CAN1->MCR |= CAN_MCR_ABOM;

    /* 3.4 自动唤醒 */
    CAN1->MCR |= CAN_MCR_AWUM;

    /* 3.5 禁止报文自动重传 */
    CAN1->MCR |= CAN_MCR_NART;

    /* 3.6 fifo锁定模式: 0 覆盖 1 丢失 */
    CAN1->MCR &= ~CAN_MCR_RFLM;

    /* 3.7 测试模式: 回环静默 */
    CAN1->BTR |= (CAN_BTR_LBKM | CAN_BTR_SILM);

    /* 3.8 配置can的波特率, 决定一个tq的大小:  1us=  (35 + 1) * 1/36us */
    CAN1->BTR &= ~CAN_BTR_BRP;
    CAN1->BTR |= (35 << 0);

    /* 3.9 配置ts1=3tq 和ts2=6tq 大小 */
    CAN1->BTR &= ~CAN_BTR_TS1;
    CAN1->BTR |= (2 << 16);

    CAN1->BTR &= ~CAN_BTR_TS2;
    CAN1->BTR |= (5 << 20);

    /* 3.10 sjw: 重新同步跳跃宽度 延长或缩短多少个时间单元的上限  =4tq*/
    CAN1->BTR &= ~CAN_BTR_SJW;
    CAN1->BTR |= (3 << 24);

    /* 3.  */
    CAN1->MCR &= ~CAN_MCR_INRQ;             /* 退出初始化 */
    while((CAN1->MSR & CAN_MSR_INAK) != 0); /* 等待退出初始化 */

    Driver_CAN_ConfigFilter();
}

static void Driver_CAN_ConfigFilter(void)
{
    /* 1. 进入过滤器的初始化模式 */
    CAN1->FMR |= CAN_FMR_FINIT;

    /* 2. 配置过滤器0的位宽: 32位 */
    CAN1->FS1R |= CAN_FS1R_FSC0;

    /* 3. 配置过滤器0的模式: 屏蔽模式 */
    CAN1->FM1R &= ~CAN_FM1R_FBM0;

    /* 4. 配置 过滤器的id和屏蔽位 */
    CAN1->sFilterRegister[0].FR1 = 0x00000000;
    CAN1->sFilterRegister[0].FR2 = 0x00000000; /* 所有位都不关心 */

    /* 5. 给过滤器0关联到fifo 0*/
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

    /* 6. 激活过滤器0 */
    CAN1->FA1R |= CAN_FA1R_FACT0;

    /* . 退出过滤器的初始化模式 */
    CAN1->FMR &= ~CAN_FMR_FINIT;
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

    /* 等待邮箱0为空 */
    while((CAN1->TSR & CAN_TSR_TME0) == 0);

    /* 设置id相关 */
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_RTR; /* 数据帧 */
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE; /* 标准帧 */
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_STID;
    CAN1->sTxMailBox[0].TIR |= (id << 21);

    /* 设置要发送的数据长度  0-8 */
    CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT0R_DLC;
    CAN1->sTxMailBox[0].TDTR |= (len << 0);

    CAN1->sTxMailBox[0].TDLR = 0x0;
    CAN1->sTxMailBox[0].TDHR = 0x0;
    /* 设置具体数据 */
    for(uint8_t i = 0; i < len; i++)
    {
        if(i < 4)
        {
            CAN1->sTxMailBox[0].TDLR |= data[i] << (i * 8);
        }
        else
        {
            CAN1->sTxMailBox[0].TDHR |= data[i] << ((i - 4) * 8);
        }
    }

    /* 请求发送数据 */
    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
}

/* 一次性把接收到的所有报文全部读取 */
void Driver_CAN_RecevieMsgs(RxMsgStruct msgs[], uint8_t *count)
{
    /* 获取目前报文数 */
    *count = (CAN1->RF0R & CAN_RF0R_FMP0) >> 0;

    for(uint8_t i = 0; i < *count; i++)
    {
        RxMsgStruct *msg = &msgs[i]; /* 结构体指针变量指向第i个结构体 */

        msg->id  = (CAN1->sFIFOMailBox[0].RIR & CAN_RI0R_STID) >> 21;
        msg->len = (CAN1->sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC) >> 0;
        memset(msg->data, 0, 8);
        for(uint8_t j = 0; j < msg->len; j++)
        {

            if(j < 4)
            {
                msg->data[j] = (CAN1->sFIFOMailBox[0].RDLR >> j * 8) & 0xff;
            }
            else
            {
                msg->data[j] = (CAN1->sFIFOMailBox[0].RDHR >> (j - 4) * 8) & 0xff;
            }
        }
        /* 完成了一个报文数据读取. 释放邮箱 */
        CAN1->RF0R |= CAN_RF0R_RFOM0;
    }
}
