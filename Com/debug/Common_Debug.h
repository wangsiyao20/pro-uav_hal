#ifndef __COMMON_DEBUG
#define __COMMON_DEBUG

#include "usart.h"
#include "string.h"
#include "stdio.h"
/* 
    �����Դ����ʱ��,����� DEBUG.
    ������Ҫ���Ե�ʱ��,ȥ���궨�� DEBUG
 */
#define DEBUG

#ifdef DEBUG

#define debug_init() Common_Debug_Init()
#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define debug_printf(format, ...) printf("[%s:%d]--" format, FILENAME, __LINE__, ##__VA_ARGS__)
#define debug_printfln(format, ...) printf("[%s:%d]--" format "\r\n", FILENAME, __LINE__, ##__VA_ARGS__)

#else

#define debug_init()  
#define debug_printf(format, ...)    
#define debug_printfln(format, ...)   

#endif

void Common_Debug_Init(void);

#endif
