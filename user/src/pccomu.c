
#include "main.h"
//打包电机发送给pc的信息

void sendtopc(float yawangle, float pitchangle, float dist)
{   
    uint8_t mes[10];
    mes[0] = 0xff;
    mes[1] = (uint16_t)yawangle;
    mes[2] = (uint16_t)yawangle >> 8;
    mes[3] = (uint16_t)pitchangle;
    mes[4] = (uint16_t)pitchangle >> 8;
    mes[5] = (uint16_t)dist;
    mes[6] = (uint16_t)dist >> 8;
    mes[7] = (uint16_t)(yawangle + pitchangle + dist);
    mes[8] = (uint16_t)(yawangle + pitchangle + dist) >> 8;
    mes[9] = (uint16_t)(0x0a);

    HAL_UART_Transmit(&huart6, mes, sizeof(mes),100);
}
