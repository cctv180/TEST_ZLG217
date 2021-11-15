/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief ZLG217 模板工程
 *
 * - 实验现象:
 *   模板例程:LED0以1秒的频率闪烁; demo例程入口:详见demo入口函数文件介绍
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-07  nwt, first implementation
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "bsp.h"
#include "amhw_arm_nvic.h"

#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"

#include "includes.h"

static TaskHandle_t HandleLed = NULL;
static TaskHandle_t HandleTask = NULL;

/* 串口输出 */
static void task_printf(void* p_arg)
{
    while (1)
    {
        AM_DBG_INFO("task running!\n");
        vTaskDelay(5000);
    }
}

/* LED 闪烁 */
static void task_led(void* p_arg)
{
    while (1)
    {
        AM_DBG_INFO("led running!\n");
        am_led_toggle(LED0);
        vTaskDelay(1000);
    }
}

int am_main (void)
{
    bsp_Init();
    /* demo例程入口 */
    AM_DBG_INFO("\r\nint am_main (void)\r\n");
    AM_DBG_INFO("cpuid=0x%08X\r\n", AMHW_ARM_SCB->cpuid);
    AM_DBG_INFO("icsr =0x%08X\r\n", AMHW_ARM_SCB->icsr);

    xTaskCreate(task_printf,    //任务函数
        "task_printf",          //任务名称
        512,                    //任务堆栈大小
        NULL,                   //传递给任务函数的参数
        4,                      //任务优先级
        &HandleTask);           //任务句柄

    xTaskCreate(task_led,       //任务函数
        "task_led",             //任务名称
        128,                    //任务堆栈大小
        NULL,                   //传递给任务函数的参数
        3,                      //任务优先级
        &HandleLed);            //任务句柄

    vTaskStartScheduler();      //开启任务调度
    while (1);
}

/* end of file */
