/*
*********************************************************************************************************
*
*    ģ������ : BSPģ��(For STM32H7)
*    �ļ����� : bsp.h
*    ��    �� : V1.0
*    ˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*               bsp = Borad surport packet �弶֧�ְ�
*    �޸ļ�¼ :
*        �汾��  ����         ����       ˵��
*        V1.0    2018-07-29  Eric2013   ��ʽ����
*
*    Copyright (C), 2018-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#ifndef _BSP_H_
#define _BSP_H_

/* ����ͷ�ļ� */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ���� BSP �汾�� */
#define HARD_BOARD      "ZLG217P64"
#define BSP_VERSION     "V0.01"
#define LCD_VERSION     (...)

#define FLASH_SIZE_BASE_STM32F0     (*(uint16_t*)0x1FFFF7CC)      //STM32F0 Flash��������ַ
#define FLASH_SIZE_BASE_STM32F1     (*(uint16_t*)0x1FFFF7E0)      //STM32F1 Flash��������ַ
#define FLASH_SIZE_BASE_STM32F2     (*(uint16_t*)0x1FFF7A22)      //STM32F2 Flash��������ַ
#define FLASH_SIZE_BASE_STM32F3     (*(uint16_t*)0x1FFFF7CC)      //STM32F3 Flash��������ַ
#define FLASH_SIZE_BASE_STM32F4     (*(uint16_t*)0x1FFF7A22)      //STM32F4 Flash��������ַ
#define FLASH_SIZE_BASE_STM32F7     (*(uint16_t*)0x1FF0F442)      //STM32F7 Flash��������ַ
#define FLASH_SIZE_BASE_STM32L0     (*(uint16_t*)0x1FF8007C)      //STM32L0 Flash��������ַ
#define FLASH_SIZE_BASE_STM32L1     (*(uint16_t*)0x1FF8004C)      //STM32L1 Flash��������ַ
#define FLASH_SIZE_BASE_STM32L4     (*(uint16_t*)0x1FFF75E0)      //STM32L4 Flash��������ַ
#define FLASH_SIZE_BASE_STM32H7     (*(uint16_t*)0x1FF0F442)      //STM32H7 Flash��������ַ
#define FLASH_SIZE_BASE_STM32G0     (*(uint16_t*)0x1FFF75E0)      //STM32G0 Flash��������ַ

/* ������ӡ���� */
#define BSP_INFO_EN

/* �������Դ�ӡ */
//#define DEBUG_MODE

/* ��������ģʽ  1���� 0�ر�  */
#define Enable_EventRecorder    0

/* RTOS_RTX����  1���� 0�ر� */
#define  USE_RTX                0

/* CPU����ʱִ�еĺ��� */
#define CPU_IDLE()          bsp_Idle()
#define ERROR_HANDLER()     Error_Handler(__FILE__, __LINE__)
/* ����ȫ���жϵĺ� */
#define ENABLE_INT()        __set_PRIMASK(0)    /* ʹ��ȫ���ж� */
#define DISABLE_INT()       __set_PRIMASK(1)    /* ��ֹȫ���ж� */

/*
*********************************************************************************************************
 * ���º��Զ���������ʾ
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

/* ����Ƿ����˿������ͺ� */
#if !defined (HARD_BOARD)
    #error "Please define the board model : HARD_BOARD"
#endif

/* ���������ڵ��Խ׶��Ŵ�printf */
#if Enable_EventRecorder == 1
    #include "EventRecorder.h"
#endif

/* printf �����Ƹ�ʽ��� �� */
#define BYTE_TO_BINARY_PATTERN "0b%c%c%c%c%c%c%c%c"
/* printf �����Ƹ�ʽ��� �� */
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

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
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

/* �ṩ������C�ļ����õı��� */

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);
void Error_Handler(char *file, uint32_t line);

void bsp_RunPer1ms(void);
void bsp_RunPer10ms(void);

#endif
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
