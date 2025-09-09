/*
 * @Author: wushengran
 * @Date: 2024-12-31 11:27:17
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "lora.h"

// ����ȫ�ֽṹ����󣬾��
static llcc68_handle_t gs_handle;

// ��ʼ��
uint8_t LoRa_Init(void)
{
    printf("LoRa��ʼ��ʼ��...\n");

    uint8_t res;
    uint32_t reg;
    uint8_t modulation;
    uint8_t config;

    /* link interface function */
    DRIVER_LLCC68_LINK_INIT(&gs_handle, llcc68_handle_t);
    DRIVER_LLCC68_LINK_SPI_INIT(&gs_handle, llcc68_interface_spi_init);
    DRIVER_LLCC68_LINK_SPI_DEINIT(&gs_handle, llcc68_interface_spi_deinit);
    DRIVER_LLCC68_LINK_SPI_WRITE_READ(&gs_handle, llcc68_interface_spi_write_read);
    DRIVER_LLCC68_LINK_RESET_GPIO_INIT(&gs_handle, llcc68_interface_reset_gpio_init);
    DRIVER_LLCC68_LINK_RESET_GPIO_DEINIT(&gs_handle, llcc68_interface_reset_gpio_deinit);
    DRIVER_LLCC68_LINK_RESET_GPIO_WRITE(&gs_handle, llcc68_interface_reset_gpio_write);
    DRIVER_LLCC68_LINK_BUSY_GPIO_INIT(&gs_handle, llcc68_interface_busy_gpio_init);
    DRIVER_LLCC68_LINK_BUSY_GPIO_DEINIT(&gs_handle, llcc68_interface_busy_gpio_deinit);
    DRIVER_LLCC68_LINK_BUSY_GPIO_READ(&gs_handle, llcc68_interface_busy_gpio_read);
    DRIVER_LLCC68_LINK_DELAY_MS(&gs_handle, llcc68_interface_delay_ms);
    DRIVER_LLCC68_LINK_DEBUG_PRINT(&gs_handle, llcc68_interface_debug_print);
    DRIVER_LLCC68_LINK_RECEIVE_CALLBACK(&gs_handle, llcc68_interface_receive_callback);

    /* init the llcc68 */
    res = llcc68_init(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: init failed.\n");

        return 1;
    }

    /* enter standby */
    res = llcc68_set_standby(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set standby failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set stop timer on preamble */
    res = llcc68_set_stop_timer_on_preamble(&gs_handle, LLCC68_LORA_DEFAULT_STOP_TIMER_ON_PREAMBLE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: stop timer on preamble failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set regulator mode */
    res = llcc68_set_regulator_mode(&gs_handle, LLCC68_LORA_DEFAULT_REGULATOR_MODE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set regulator mode failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set pa config */
    res = llcc68_set_pa_config(&gs_handle, LLCC68_LORA_DEFAULT_PA_CONFIG_DUTY_CYCLE, LLCC68_LORA_DEFAULT_PA_CONFIG_HP_MAX);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set pa config failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* enter to stdby xosc mode */
    res = llcc68_set_rx_tx_fallback_mode(&gs_handle, LLCC68_RX_TX_FALLBACK_MODE_STDBY_XOSC);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx tx fallback mode failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set dio irq */
    res = llcc68_set_dio_irq_params(&gs_handle, 0x03FF, 0x03FF, 0x0000, 0x0000);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set dio irq params failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* clear irq status */
    res = llcc68_clear_irq_status(&gs_handle, 0x03FF);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: clear irq status failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set lora mode */
    res = llcc68_set_packet_type(&gs_handle, LLCC68_PACKET_TYPE_LORA);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set tx params */
    res = llcc68_set_tx_params(&gs_handle, LLCC68_LORA_DEFAULT_TX_DBM, LLCC68_LORA_DEFAULT_RAMP_TIME);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx params failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set lora modulation params */
    res = llcc68_set_lora_modulation_params(&gs_handle, LLCC68_LORA_DEFAULT_SF, LLCC68_LORA_DEFAULT_BANDWIDTH,
                                            LLCC68_LORA_DEFAULT_CR, LLCC68_LORA_DEFAULT_LOW_DATA_RATE_OPTIMIZE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora modulation params failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* convert the frequency */
    res = llcc68_frequency_convert_to_register(&gs_handle, LLCC68_LORA_DEFAULT_RF_FREQUENCY, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set the frequency */
    res = llcc68_set_rf_frequency(&gs_handle, reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rf frequency failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set base address */
    res = llcc68_set_buffer_base_address(&gs_handle, 0x00, 0x00);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set buffer base address failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set lora symb num */
    res = llcc68_set_lora_symb_num_timeout(&gs_handle, LLCC68_LORA_DEFAULT_SYMB_NUM_TIMEOUT);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora symb num timeout failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* reset stats */
    res = llcc68_reset_stats(&gs_handle, 0x0000, 0x0000, 0x0000);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: reset stats failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* clear device errors */
    res = llcc68_clear_device_errors(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: clear device errors failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set the lora sync word */
    res = llcc68_set_lora_sync_word(&gs_handle, LLCC68_LORA_DEFAULT_SYNC_WORD);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* get tx modulation */
    res = llcc68_get_tx_modulation(&gs_handle, (uint8_t *)&modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }
    modulation |= 0x04;

    /* set the tx modulation */
    res = llcc68_set_tx_modulation(&gs_handle, modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set the rx gain */
    res = llcc68_set_rx_gain(&gs_handle, LLCC68_LORA_DEFAULT_RX_GAIN);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx gain failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* set the ocp */
    res = llcc68_set_ocp(&gs_handle, LLCC68_LORA_DEFAULT_OCP);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set ocp failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    /* get the tx clamp config */
    res = llcc68_get_tx_clamp_config(&gs_handle, (uint8_t *)&config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }
    config |= 0x1E;

    /* set the tx clamp config */
    res = llcc68_set_tx_clamp_config(&gs_handle, config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return 1;
    }

    printf("LoRa��ʼ����ɣ�\n");

    return 0;
}

// ��������
uint8_t LoRa_SendData(uint8_t data[], uint16_t len)
{
    // 0. ʹ��TxEN
    TXEN_HIGH;
    RXEN_LOW;

    // 1. ����Txģʽ
    /* set dio irq */
    if (llcc68_set_dio_irq_params(&gs_handle, LLCC68_IRQ_TX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  LLCC68_IRQ_TX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  0x0000, 0x0000) != 0)
    {
        return 1;
    }

    /* clear irq status */
    if (llcc68_clear_irq_status(&gs_handle, 0x03FFU) != 0)
    {
        return 1;
    }

    // 2. �������ݷ���
    if (llcc68_lora_transmit(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ,
                             LLCC68_LORA_DEFAULT_PREAMBLE_LENGTH, LLCC68_LORA_DEFAULT_HEADER,
                             LLCC68_LORA_DEFAULT_CRC_TYPE, LLCC68_LORA_DEFAULT_INVERT_IQ,
                             (uint8_t *)data, len, 0) != 0)
    {
        return 1;
    }

    return 0;
}

// ���루����������ģʽ
uint8_t LoRa_EnterRxMode(void)
{
    // 0. ʹ��RxEN
    TXEN_LOW;
    RXEN_HIGH;

    uint8_t setup;

    /* set dio irq */
    if (llcc68_set_dio_irq_params(&gs_handle, LLCC68_IRQ_RX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CRC_ERR | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  LLCC68_IRQ_RX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CRC_ERR | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  0x0000, 0x0000) != 0)
    {
        return 1;
    }

    /* clear irq status */
    if (llcc68_clear_irq_status(&gs_handle, 0x03FFU) != 0)
    {
        return 1;
    }

    /* set lora packet params */
    if (llcc68_set_lora_packet_params(&gs_handle, LLCC68_LORA_DEFAULT_PREAMBLE_LENGTH,
                                      LLCC68_LORA_DEFAULT_HEADER, LLCC68_LORA_DEFAULT_BUFFER_SIZE,
                                      LLCC68_LORA_DEFAULT_CRC_TYPE, LLCC68_LORA_DEFAULT_INVERT_IQ) != 0)
    {
        return 1;
    }

    /* get iq polarity */
    if (llcc68_get_iq_polarity(&gs_handle, (uint8_t *)&setup) != 0)
    {
        return 1;
    }

#if LLCC68_LORA_DEFAULT_INVERT_IQ == LLCC68_BOOL_FALSE
    setup |= 1 << 2;
#else
    setup &= ~(1 << 2);
#endif

    /* set the iq polarity */
    if (llcc68_set_iq_polarity(&gs_handle, setup) != 0)
    {
        return 1;
    }

    /* start receive */
    if (llcc68_continuous_receive(&gs_handle) != 0)
    {
        return 1;
    }

    return 0;
}

// ��������
void LoRa_RecvData(uint8_t rxBuff[], uint16_t *rxLen)
{
    // ����irq_handler�������ݴӽ��ջ���������
    llcc68_irq_handler(&gs_handle);

    // �ж����ݳ��ȣ�����������ݣ����ٸ��Ƶ�������Ӧ�Ļ�������
    if (gs_handle.receive_len > 0)
    {
        *rxLen = gs_handle.receive_len;

        // �������ݵ����ջ�������������
        memcpy(rxBuff, gs_handle.receive_buf, *rxLen);

        // ��������
        gs_handle.receive_len = 0;
    }
}
