
#include "bsp_uart.h"

/* ==============================================================================
 * Private Variables
 * ============================================================================== */
static UART_HandleTypeDef *s_huart = NULL;

/* ==============================================================================
 * Public Functions
 * ============================================================================== */

/**
 * @brief  Binds a HAL UART handle for printf retargeting
 * @param  huart: Pointer to the active UART handle
 */
void BSP_UART_Init(UART_HandleTypeDef *huart)
{
    s_huart = huart;
}

/**
 * @brief  Low-level I/O putchar retarget implementation
 * @param  ch: Character to transmit
 * @retval Character transmitted
 */
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    if (s_huart != NULL)
    {
        HAL_UART_Transmit(s_huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    }
    return ch;
}
