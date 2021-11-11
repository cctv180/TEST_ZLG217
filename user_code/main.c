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
 * \brief ZLG217 ģ�幤��
 *
 * - ʵ������:
 *   ģ������:LED0��1���Ƶ����˸; demo�������:���demo��ں����ļ�����
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-07  nwt, first implementation
 * \endinternal
 */

/**
 * \brief �������
 */
#include "bsp.h"
#include "amhw_arm_nvic.h"

#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"

int am_main (void)
{
    bsp_Init();
    /* demo������� */
    AM_DBG_INFO("\r\nint am_main (void)\r\n");
    AM_DBG_INFO("cpuid=0x%08X\r\n", AMHW_ARM_SCB->cpuid);
    AM_DBG_INFO("icsr =0x%08X\r\n", AMHW_ARM_SCB->icsr);

    while (1)
    {
        am_led_toggle(LED0);
        am_mdelay(1000);
    }
}

/* end of file */
