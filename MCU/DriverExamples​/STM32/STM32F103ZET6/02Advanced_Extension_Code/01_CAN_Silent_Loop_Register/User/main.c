#include "Common_Debug.h"

#include "string.h"

#include "Driver_CAN.h"
#include "Delay.h"

RxMsgStruct msgs[3];
uint8_t     msgCount;

int         main()
{

    debug_init();
    debug_printfln("CAN通讯实验");
    Driver_CAN_Init();
    /* 先发送 */
    Driver_CAN_SendMsg(0x1, "abcdef", 6);
    Driver_CAN_SendMsg(0x2, "abc", 3);
    Driver_CAN_SendMsg(0x3, "1234", 4);
    Driver_CAN_SendMsg(0x4, "12345", 5);
    debug_printfln("发送完成...");

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
    }
}
