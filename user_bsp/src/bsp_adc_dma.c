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
 * \brief ADC��ͨ��ͨ��DMA���䣬ͨ��������ӿ�ʵ��
 *
 * - ʵ������
 *   1. ��ӦADCͨ����ӡ��ѹֵ��
 *
 * \par Դ����
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

#define __ADC1_DMA_CHAN1 DMA_CHAN_ADC1  /* ADC1��ӦDMAͨ��1 */
#define __ADC2_DMA_CHAN2 DMA_CHAN_ADC2  /* ADC2��ӦDMAͨ��2 */

#define BUF_SIZE 12

uint16_t g_adc1_buf_dst[BUF_SIZE];      /* Ŀ������ݻ����� */
uint16_t g_adc2_buf_dst[BUF_SIZE];      /* Ŀ������ݻ����� */

static int adc1_chan[] = { 0 };         /* ѡ��ADC1ͨ�� */
static int adc2_chan[] = { 4, 5 };      /* ѡ��ADC2ͨ�� */

/**
 * \brief DMA �жϷ������
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

    /* ʹ��ADC DMA���� */
    amhw_zlg_adc_ctrl_reg_set(p_hw_adc, AMHW_ZLG_ADC_DMA_EN_MASK);

    amhw_zlg_adc_data_scan_set(p_hw_adc, AMHW_ZLG_ADC_CONTINUE_SCAN);

    amhw_zlg_adc_data_transfer_set(p_hw_adc, AMHW_ZLG_ADC_DATA_TRANSFER_START);
}

/**
 * \brief �������
 */
void bsp_init_adc_dma(void)
{
    amhw_zlg_dma_xfer_desc_t desc;     /* DMA������ */
    uint32_t flags;
    /* ADC1 Start */
    /* ADC1 �������ţ�ʹ��ʱ�� */
    am_gpio_pin_cfg(PIOA_0, PIOA_0_ADC1_IN0 | PIOA_0_AIN);  /* ADC1ͨ��0 */
    am_clk_enable(CLK_ADC1);

    /* ADC1 ��ʼ���Ĵ��� */
    __zlg_adc_init(ZLG217_ADC1, adc1_chan, sizeof(adc1_chan) / sizeof(adc1_chan[0]));
    /* ���� DMA �жϷ����� */
    am_zlg_dma_isr_connect(__ADC1_DMA_CHAN1, dma_isr, (void*)ZLG217_ADC1);
    /* ���ò��� */
    flags = AMHW_ZLG_DMA_CHAN_PRIORITY_HIGH |       /* �ж����ȼ� �� */
        AMHW_ZLG_DMA_CHAN_MEM_SIZE_16BIT |          /* �ڴ����ݿ�� 2 �ֽ� */
        AMHW_ZLG_DMA_CHAN_PER_SIZE_16BIT |          /* �������ݿ�� 2 �ֽ� */
        AMHW_ZLG_DMA_CHAN_MEM_ADD_INC_ENABLE |      /* �ڴ��ַ���� */
        AMHW_ZLG_DMA_CHAN_PER_ADD_INC_DISABLE |     /* �����ַ������ */
        AMHW_ZLG_DMA_CHAN_CIRCULAR_MODE_ENABLE |    /* ��ѭ��ģʽ */
        AMHW_ZLG_DMA_CHAN_INT_TX_CMP_ENABLE;        /* ʹ��DMA��������ж� */
    /* ����ͨ�������� */
    am_zlg_dma_xfer_desc_build(&desc,               /* ͨ�������� */
        (uint32_t)(&ZLG217_ADC1->addata),           /* Դ�����ݻ����� */
        (uint32_t)(g_adc1_buf_dst),                 /* Ŀ������ݻ����� */
        sizeof(g_adc1_buf_dst),                     /* �����ֽ��� */
        flags);                                     /* �������� */
    /* ����DMA���� */
    am_zlg_dma_xfer_desc_chan_cfg(&desc,            /* ͨ�������� */
        AMHW_ZLG_DMA_PER_TO_MER,                    /* ���赽�ڴ� */
        __ADC1_DMA_CHAN1);                          /* DMAͨ��1 DMA_CHAN_ADC1 */
    /* ���� DMA ���䣬���Ͽ�ʼ���� */
    am_zlg_dma_chan_start(__ADC1_DMA_CHAN1);
    /* ADC1 End */

    /* ADC2 Start */
    /* ADC2 �������ţ�ʹ��ʱ�� */
    am_gpio_pin_cfg(PIOC_2, PIOC_2_ADC2_IN4 | PIOC_2_AIN);  /* ADC2ͨ��4 */
    am_gpio_pin_cfg(PIOC_3, PIOC_3_ADC2_IN5 | PIOC_3_AIN);  /* ADC2ͨ��5 */
    am_clk_enable(CLK_ADC2);
    /* ADC2 ��ʼ���Ĵ��� */
    __zlg_adc_init(ZLG217_ADC2, adc2_chan, sizeof(adc2_chan) / sizeof(adc2_chan[0]));
    /* ���� DMA �жϷ����� */
    am_zlg_dma_isr_connect(__ADC2_DMA_CHAN2, dma_isr, (void*)ZLG217_ADC2);
    /* ���ò��� */
    flags = AMHW_ZLG_DMA_CHAN_PRIORITY_HIGH |       /* �ж����ȼ� �� */
        AMHW_ZLG_DMA_CHAN_MEM_SIZE_16BIT |          /* �ڴ����ݿ�� 2 �ֽ� */
        AMHW_ZLG_DMA_CHAN_PER_SIZE_16BIT |          /* �������ݿ�� 2 �ֽ� */
        AMHW_ZLG_DMA_CHAN_MEM_ADD_INC_ENABLE |      /* �ڴ��ַ���� */
        AMHW_ZLG_DMA_CHAN_PER_ADD_INC_DISABLE |     /* �����ַ������ */
        AMHW_ZLG_DMA_CHAN_CIRCULAR_MODE_ENABLE |    /* ��ѭ��ģʽ */
        AMHW_ZLG_DMA_CHAN_INT_TX_CMP_ENABLE;        /* ʹ��DMA��������ж� */
    /* ����ͨ�������� */
    am_zlg_dma_xfer_desc_build(&desc,               /* ͨ�������� */
        (uint32_t)(&ZLG217_ADC2->addata),           /* Դ�����ݻ����� */
        (uint32_t)(g_adc2_buf_dst),                 /* Ŀ������ݻ����� */
        sizeof(g_adc2_buf_dst),                     /* �����ֽ��� */
        flags);                                     /* �������� */
    /* ����DMA���� */
    am_zlg_dma_xfer_desc_chan_cfg(&desc,            /* ͨ�������� */
        AMHW_ZLG_DMA_PER_TO_MER,                    /* ���赽�ڴ� */
        __ADC2_DMA_CHAN2);                          /* DMAͨ��1 DMA_CHAN_ADC1 */
    /* ���� DMA ���䣬���Ͽ�ʼ���� */
    am_zlg_dma_chan_start(__ADC2_DMA_CHAN2);
    /* ADC2 End */
}

/*
*********************************************************************************************************
*    �� �� ��: bsp_adc_dma_test
*    ����˵��: ADC���Գ��� ����
*    ��    ��: ��
*    �� �� ֵ: ��
*    ��    �ߣ� cctv02
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
            /* ת��Ϊ��ѹֵ��Ӧ������ֵ */
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
            /* ת��Ϊ��ѹֵ��Ӧ������ֵ */
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
