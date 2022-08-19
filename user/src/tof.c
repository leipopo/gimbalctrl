#include "main.h"
uint8_t tof_rx_buf[2][TOF_RX_BUF_NUM];
uint8_t tofready = 0;
Point_Data pointdata[12];
float distance;

void tofinit()
{
    while (tofready == 0)
    {
        DMA_RX_INIT(&huart1, &hdma_usart1_rx, tof_rx_buf[0], tof_rx_buf[1], TOF_RX_BUF_NUM);
        HAL_Delay(100);
    }
}

float tofdatadecode(Point_Data pd[12], uint8_t buf[TOF_RX_BUF_NUM])
{
    float tofdist      = 0;
    float sumintensity = 0;
    for (int8_t i = 0; i < 12; i++)
    {
        pd[i].distance  = (buf[3 * (i + 2) + 1] << 8) | buf[3 * (i + 2)];
        pd[i].intensity = buf[3 * (i + 2) + 2];
        tofdist += pd[i].intensity * pd[i].distance;
        sumintensity += pd[i].intensity;
    }

    return tofdist / sumintensity;
}

void USART1_IRQHandler()
{
    if (huart1.Instance->SR & UART_FLAG_RXNE)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    }
    else if (huart1.Instance->SR & UART_FLAG_IDLE)
    {
        static uint16_t this_time_rx_len = 0;
        __HAL_UART_CLEAR_PEFLAG(&huart1);
        this_time_rx_len              = TOF_RX_BUF_NUM - hdma_usart1_rx.Instance->NDTR;
        hdma_usart1_rx.Instance->NDTR = TOF_RX_BUF_NUM;
        if ((hdma_usart1_rx.Instance->CR & DMA_SxCR_CT) == RESET)
        {
            hdma_usart1_rx.Instance->CR |= DMA_SxCR_CT;
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if (this_time_rx_len == TOF_FRAME_LENGTH)
            {
                distance = tofdatadecode(pointdata, tof_rx_buf[0]);
                tofready = 1;
            }
        }
        else
        {
            DMA2_Stream2->CR &= ~(DMA_SxCR_CT);
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if (this_time_rx_len == TOF_FRAME_LENGTH)
            {
                distance = tofdatadecode(pointdata, tof_rx_buf[1]);
            }
        }
    }
}
