
#ifndef BSP_UART_H_
#define BSP_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdio.h>

void BSP_UART_Init(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* BSP_UART_H_ */
