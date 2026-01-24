#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "bsp_system.h"

// 当不要debug信息时，注释掉下面这行即可
#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define FILENAME (strrchr(__FILE__,'\\') ? strrchr(__FILE__,'\\') + 1 : __FILE__)
#define debug_printf(format,...) printf("[%s:%d]--" format, FILENAME, __LINE__, ##__VA_ARGS__)
#define debug_printfln(format,...) printf("[%s:%d]--" format "\r\n", FILENAME, __LINE__, ##__VA_ARGS__)//这个是自动带\r\n

#else

#define debug_printf(format,...)
#define debug_printfln(format,...)

#endif/* DEBUG_ENABLE */














#endif/* __DEBUG_H__ */


