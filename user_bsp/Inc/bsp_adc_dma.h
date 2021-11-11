#ifndef __BSP_ADC_DMA_H
#define __BSP_ADC_DMA_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

    #include "am_common.h"  /* 标准定义头文件 */
    #include "am_version.h" /* 版本信息接口头文件 */

    extern uint16_t g_adc1_buf_dst[];      /* 目标端数据缓冲区 */
    extern uint16_t g_adc2_buf_dst[];      /* 目标端数据缓冲区 */

    void bsp_init_adc_dma(void);
    void bsp_adc_dma_test(void);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* __AMETAL_H */

/* end of file */
