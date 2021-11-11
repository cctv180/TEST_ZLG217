/*
*********************************************************************************************************
*
*    模块名称 : BSP模块(For STM32H7)
*    文件名称 : bsp.h
*    版    本 : V1.0
*    说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*               bsp = Borad surport packet 板级支持包
*    修改记录 :
*        版本号  日期         作者       说明
*        V1.0    2018-07-29  Eric2013   正式发布
*
*    Copyright (C), 2018-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#ifndef _BSP_H_
#define _BSP_H_

/* 公共头文件 */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 定义 BSP 版本号 */
#define HARD_BOARD      "ZLG217P64"
#define BSP_VERSION     "V0.01"
#define LCD_VERSION     (...)

#define FLASH_SIZE_BASE_STM32F0     (*(uint16_t*)0x1FFFF7CC)      //STM32F0 Flash容量基地址
#define FLASH_SIZE_BASE_STM32F1     (*(uint16_t*)0x1FFFF7E0)      //STM32F1 Flash容量基地址
#define FLASH_SIZE_BASE_STM32F2     (*(uint16_t*)0x1FFF7A22)      //STM32F2 Flash容量基地址
#define FLASH_SIZE_BASE_STM32F3     (*(uint16_t*)0x1FFFF7CC)      //STM32F3 Flash容量基地址
#define FLASH_SIZE_BASE_STM32F4     (*(uint16_t*)0x1FFF7A22)      //STM32F4 Flash容量基地址
#define FLASH_SIZE_BASE_STM32F7     (*(uint16_t*)0x1FF0F442)      //STM32F7 Flash容量基地址
#define FLASH_SIZE_BASE_STM32L0     (*(uint16_t*)0x1FF8007C)      //STM32L0 Flash容量基地址
#define FLASH_SIZE_BASE_STM32L1     (*(uint16_t*)0x1FF8004C)      //STM32L1 Flash容量基地址
#define FLASH_SIZE_BASE_STM32L4     (*(uint16_t*)0x1FFF75E0)      //STM32L4 Flash容量基地址
#define FLASH_SIZE_BASE_STM32H7     (*(uint16_t*)0x1FF0F442)      //STM32H7 Flash容量基地址
#define FLASH_SIZE_BASE_STM32G0     (*(uint16_t*)0x1FFF75E0)      //STM32G0 Flash容量基地址

/* 开启打印数据 */
#define BSP_INFO_EN

/* 开启调试打印 */
//#define DEBUG_MODE

/* 开启调试模式  1开启 0关闭  */
#define Enable_EventRecorder    0

/* RTOS_RTX开启  1开启 0关闭 */
#define  USE_RTX                0

/* CPU空闲时执行的函数 */
#define CPU_IDLE()          bsp_Idle()
#define ERROR_HANDLER()     Error_Handler(__FILE__, __LINE__)
/* 开关全局中断的宏 */
#define ENABLE_INT()        __set_PRIMASK(0)    /* 使能全局中断 */
#define DISABLE_INT()       __set_PRIMASK(1)    /* 禁止全局中断 */

/*
*********************************************************************************************************
 * 以下宏自动处理与提示
*********************************************************************************************************
 */
#ifdef DEBUG_MODE
//#define BSP_Printf(...) bsp_log_debug(__FILE__, __LINE__, __VA_ARGS__)
#define BSP_Printf(...)                                 \
    do                                                  \
    {                                                   \
        printf("[D/SYS] (%s:%d) ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                            \
        printf("\r\n");                                 \
    } while (0)
#else
    #define BSP_Printf(...)
#endif /* DEBUG_MODE END */

#ifdef BSP_INFO_EN
    //#define BSP_INFO(...)  bsp_log_info(__VA_ARGS__)
    #define BSP_INFO(...)                               \
    do                                                  \
    {                                                   \
        printf("[I/SYS] ");                             \
        printf(__VA_ARGS__);                            \
        printf("\r\n");                                 \
    } while (0)

#else
    #define BSP_INFO(...)
#endif

#if USE_RTX == 1
    #ifndef RTE_CMSIS_RTOS2
        #define RTE_CMSIS_RTOS2
    #endif
#endif

/* 检查是否定义了开发板型号 */
#if !defined (HARD_BOARD)
    #error "Please define the board model : HARD_BOARD"
#endif

/* 这个宏仅用于调试阶段排错printf */
#if Enable_EventRecorder == 1
    #include "EventRecorder.h"
#endif

/* printf 二进制格式输出 宏 */
#define BYTE_TO_BINARY_PATTERN "0b%c%c%c%c%c%c%c%c"
/* printf 二进制格式输出 宏 */
#define BYTE_TO_BINARY(byte) \
        (byte & 0x80 ? '1' : '0'), \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')
#ifndef TRUE
    #define TRUE  1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifndef __IO
    #define __IO volatile
#endif

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include <am_zlg217.h>

#include "bsp_adc_dma.h"
//#include "..\User\bsp_msg.h"
//#include "..\User\bsp\bsp_user_lib.h"
//#include "bsp_key.h"
//#include "bsp_hc595.h"
//#include "bsp_led.h"
//#include "bsp_lcd.h"
//#include "bsp_pic.h"
//#include "bsp_uart.h"
//#include "bsp_crc16.h"
//#include "bsp_timer.h"
//#include "bsp_adc.h"
//#include "bsp_beep.h"

//#include "app.h"
//#include "gui_dev.h"
//#include "gui_run_1.h"
//#include "gui_run_2.h"
//#include "gui_stop.h"
//#include "gui_factory.h"
//#include "ringbuff.h"

/* 提供给其他C文件调用的变量 */

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);
void Error_Handler(char *file, uint32_t line);

void bsp_RunPer1ms(void);
void bsp_RunPer10ms(void);

#endif
/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
