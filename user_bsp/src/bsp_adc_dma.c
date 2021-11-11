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
 * \brief ADC多通道通过DMA传输，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. 对应ADC通道打印电压值。
 *
 * \par 源代码
 * \snippet demo_zlg_adc_dma.c src_zlg_adc_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 19-02-27  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg_adc_dma
 * \copydoc demo_zlg_adc_dma.c
 */

#include "ametal.h"
#include "am_board.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zlg_dma.h"
#include "hw/amhw_zlg_dma.h"
#include "hw/amhw_zlg_adc.h"

#include "am_zlg217.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_zlg_entries.h"

#include "bsp.h"

#define __ADC1_DMA_CHAN1 DMA_CHAN_ADC1  /* ADC1对应DMA通道1 */
#define __ADC2_DMA_CHAN2 DMA_CHAN_ADC2  /* ADC2对应DMA通道2 */

#define BUF_SIZE 12

uint16_t g_adc1_buf_dst[BUF_SIZE];      /* 目标端数据缓冲区 */
uint16_t g_adc2_buf_dst[BUF_SIZE];      /* 目标端数据缓冲区 */

static int adc1_chan[] = { 0 };         /* 选择ADC1通道 */
static int adc2_chan[] = { 4, 5 };      /* 选择ADC2通道 */

/**
 * \brief DMA 中断服务程序
 */
static void dma_isr(void* p_arg, uint32_t flag)
{
    if (flag == AM_ZLG_DMA_INT_NORMAL)
    {
        if (ZLG217_ADC1 == p_arg)
        {

        }
        else if (ZLG217_ADC2 == p_arg)
        {

        }
    }
}

static void __zlg_adc_init(amhw_zlg_adc_t* p_hw_adc, int* p_adc_chan, int adc_chan_num)
{
    int i;

    amhw_zlg_adc_data_resolution(p_hw_adc, AMHW_ZLG_ADC_DATA_VALID_12BIT);
    amhw_zlg_adc_prescaler_val(p_hw_adc, 4);
    amhw_zlg_adc_chan_sample_time(p_hw_adc, AMHW_ZLG_ADC_CHAN_ST55_5);

    for (i = 0; i < adc_chan_num; i++)
    {
        amhw_zlg_adc_input_chan_enable(p_hw_adc, p_adc_chan[i]);
    }
    amhw_zlg_adc_cgf_reg_set(p_hw_adc, AMHW_ZLG_ADC_MODULE_EN_MASK);

    /* 使能ADC DMA请求 */
    amhw_zlg_adc_ctrl_reg_set(p_hw_adc, AMHW_ZLG_ADC_DMA_EN_MASK);

    amhw_zlg_adc_data_scan_set(p_hw_adc, AMHW_ZLG_ADC_CONTINUE_SCAN);

    amhw_zlg_adc_data_transfer_set(p_hw_adc, AMHW_ZLG_ADC_DATA_TRANSFER_START);
}

/**
 * \brief 例程入口
 */
void bsp_init_adc_dma(void)
{
    amhw_zlg_dma_xfer_desc_t desc;     /* DMA描述符 */
    uint32_t flags;
    /* ADC1 Start */
    /* ADC1 配置引脚，使能时钟 */
    am_gpio_pin_cfg(PIOA_0, PIOA_0_ADC1_IN0 | PIOA_0_AIN);  /* ADC1通道0 */
    am_clk_enable(CLK_ADC1);

    /* ADC1 初始化寄存器 */
    __zlg_adc_init(ZLG217_ADC1, adc1_chan, sizeof(adc1_chan) / sizeof(adc1_chan[0]));
    /* 连接 DMA 中断服务函数 */
    am_zlg_dma_isr_connect(__ADC1_DMA_CHAN1, dma_isr, (void*)ZLG217_ADC1);
    /* 设置参数 */
    flags = AMHW_ZLG_DMA_CHAN_PRIORITY_HIGH |       /* 中断优先级 高 */
        AMHW_ZLG_DMA_CHAN_MEM_SIZE_16BIT |          /* 内存数据宽度 2 字节 */
        AMHW_ZLG_DMA_CHAN_PER_SIZE_16BIT |          /* 外设数据宽度 2 字节 */
        AMHW_ZLG_DMA_CHAN_MEM_ADD_INC_ENABLE |      /* 内存地址自增 */
        AMHW_ZLG_DMA_CHAN_PER_ADD_INC_DISABLE |     /* 外设地址不自增 */
        AMHW_ZLG_DMA_CHAN_CIRCULAR_MODE_ENABLE |    /* 打开循环模式 */
        AMHW_ZLG_DMA_CHAN_INT_TX_CMP_ENABLE;        /* 使能DMA传输完成中断 */
    /* 建立通道描述符 */
    am_zlg_dma_xfer_desc_build(&desc,               /* 通道描述符 */
        (uint32_t)(&ZLG217_ADC1->addata),           /* 源端数据缓冲区 */
        (uint32_t)(g_adc1_buf_dst),                 /* 目标端数据缓冲区 */
        sizeof(g_adc1_buf_dst),                     /* 传输字节数 */
        flags);                                     /* 传输配置 */
    /* 配置DMA传输 */
    am_zlg_dma_xfer_desc_chan_cfg(&desc,            /* 通道描述符 */
        AMHW_ZLG_DMA_PER_TO_MER,                    /* 外设到内存 */
        __ADC1_DMA_CHAN1);                          /* DMA通道1 DMA_CHAN_ADC1 */
    /* 启动 DMA 传输，马上开始传输 */
    am_zlg_dma_chan_start(__ADC1_DMA_CHAN1);
    /* ADC1 End */

    /* ADC2 Start */
    /* ADC2 配置引脚，使能时钟 */
    am_gpio_pin_cfg(PIOC_2, PIOC_2_ADC2_IN4 | PIOC_2_AIN);  /* ADC2通道4 */
    am_gpio_pin_cfg(PIOC_3, PIOC_3_ADC2_IN5 | PIOC_3_AIN);  /* ADC2通道5 */
    am_clk_enable(CLK_ADC2);
    /* ADC2 初始化寄存器 */
    __zlg_adc_init(ZLG217_ADC2, adc2_chan, sizeof(adc2_chan) / sizeof(adc2_chan[0]));
    /* 连接 DMA 中断服务函数 */
    am_zlg_dma_isr_connect(__ADC2_DMA_CHAN2, dma_isr, (void*)ZLG217_ADC2);
    /* 设置参数 */
    flags = AMHW_ZLG_DMA_CHAN_PRIORITY_HIGH |       /* 中断优先级 高 */
        AMHW_ZLG_DMA_CHAN_MEM_SIZE_16BIT |          /* 内存数据宽度 2 字节 */
        AMHW_ZLG_DMA_CHAN_PER_SIZE_16BIT |          /* 外设数据宽度 2 字节 */
        AMHW_ZLG_DMA_CHAN_MEM_ADD_INC_ENABLE |      /* 内存地址自增 */
        AMHW_ZLG_DMA_CHAN_PER_ADD_INC_DISABLE |     /* 外设地址不自增 */
        AMHW_ZLG_DMA_CHAN_CIRCULAR_MODE_ENABLE |    /* 打开循环模式 */
        AMHW_ZLG_DMA_CHAN_INT_TX_CMP_ENABLE;        /* 使能DMA传输完成中断 */
    /* 建立通道描述符 */
    am_zlg_dma_xfer_desc_build(&desc,               /* 通道描述符 */
        (uint32_t)(&ZLG217_ADC2->addata),           /* 源端数据缓冲区 */
        (uint32_t)(g_adc2_buf_dst),                 /* 目标端数据缓冲区 */
        sizeof(g_adc2_buf_dst),                     /* 传输字节数 */
        flags);                                     /* 传输配置 */
    /* 配置DMA传输 */
    am_zlg_dma_xfer_desc_chan_cfg(&desc,            /* 通道描述符 */
        AMHW_ZLG_DMA_PER_TO_MER,                    /* 外设到内存 */
        __ADC2_DMA_CHAN2);                          /* DMA通道1 DMA_CHAN_ADC1 */
    /* 启动 DMA 传输，马上开始传输 */
    am_zlg_dma_chan_start(__ADC2_DMA_CHAN2);
    /* ADC2 End */
}

/*
*********************************************************************************************************
*    函 数 名: bsp_adc_dma_test
*    功能说明: ADC测试程序 阻塞
*    形    参: 无
*    返 回 值: 无
*    作    者： cctv02
*********************************************************************************************************
*/
void bsp_adc_dma_test(void)
{
    AM_DBG_INFO("demo am217_core hw adc dma!\r\n");

    while (1)
    {
        am_kprintf("\r\nADC1\r\n");
        for (int i = 0; i < sizeof(g_adc1_buf_dst) / sizeof(g_adc1_buf_dst[0]); i++)
        {
            uint32_t adc_mv = 0;
            /* 转换为电压值对应的整数值 */
            adc_mv = (g_adc1_buf_dst[i] & 0x0fff) * 3355 / ((1UL << 12) - 1);
            am_kprintf("CH: %d, Sample : %4d, Vol: %d mv\r\n",
                adc1_chan[0],
                g_adc1_buf_dst[i],
                adc_mv);
        }
        am_kprintf("\r\nADC2\r\n");
        for (int i = 0; i < sizeof(g_adc2_buf_dst) / sizeof(g_adc2_buf_dst[0]); i++)
        {
            uint32_t adc_mv = 0;
            /* 转换为电压值对应的整数值 */
            adc_mv = (g_adc2_buf_dst[i] & 0x0fff) * 3355 / ((1UL << 12) - 1);
            am_kprintf("CH: %d, Sample : %4d, Vol: %d mv\r\n",
                adc2_chan[i % (sizeof(adc2_chan) / sizeof(adc2_chan[0]))],
                g_adc2_buf_dst[i],
                adc_mv);
        }
        am_kprintf("\r\n");
        am_led_toggle(LED0);
        am_mdelay(1000);
    }
}
/* end of file */
