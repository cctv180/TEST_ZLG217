#ifndef __BSP_ADC_DMA_H
#define __BSP_ADC_DMA_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

    #include "am_common.h"  /* ��׼����ͷ�ļ� */
    #include "am_version.h" /* �汾��Ϣ�ӿ�ͷ�ļ� */

    extern uint16_t g_adc1_buf_dst[];      /* Ŀ������ݻ����� */
    extern uint16_t g_adc2_buf_dst[];      /* Ŀ������ݻ����� */

    void bsp_init_adc_dma(void);
    void bsp_adc_dma_test(void);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* __AMETAL_H */

/* end of file */
