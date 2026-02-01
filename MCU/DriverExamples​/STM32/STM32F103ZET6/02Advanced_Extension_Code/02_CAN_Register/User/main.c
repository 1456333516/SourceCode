#include "Common_Debug.h"

#include "string.h"

#include "Driver_CAN.h"
#include "Delay.h"
#include "Driver_GPIO.h"

RxMsgStruct msgs[3];
uint8_t     msgCount;

uint8_t buff[10];
uint8_t cnt = 0;

int main()
{
    debug_init();
    debug_printfln("CAN通讯实验 2");
    Driver_GPIO_Init();
    Driver_CAN_Init();

    /* 接收 */
    while(1)
    {
        msgCount = 0;
        Driver_CAN_RecevieMsgs(msgs, &msgCount);

        for(uint8_t i = 0; i < msgCount; i++)
        {
            debug_printfln("id = %#x, len = %d, msg = %.*s",
                           msgs[i].id,
                           msgs[i].len,
                           msgs[i].len,
                           msgs[i].data);
        }

        /* 检测按键 */
        if(detect_key() == KEY1)
        {
            sprintf(buff, "can2 %d", cnt++);
            Driver_CAN_SendMsg(0x1, buff, strlen(buff));
        }
    }
}
