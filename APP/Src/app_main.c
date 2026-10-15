
#include "app_main.h"
#include "bsp_uart.h"
#include "cmsis_os2.h"
#include "main.h"
#include <stdio.h>


/* ==============================================================================
 * Private Constants & Definitions
 * ============================================================================== */
#define CONTROL_TASK_PERIOD_MS    (1000U)
#define COMM_TASK_PERIOD_MS       (2000U)
#define DEFAULT_STACK_SIZE_BYTES  (256 * 4)



/* ==============================================================================
 * Private Variables
 * ============================================================================== */
static osThreadId_t s_controlTaskHandle = NULL;
static osThreadId_t s_commTaskHandle    = NULL;



/* ==============================================================================
 * Private Task Prototypes
 * ============================================================================== */
static void StartControlTask(void *argument);
static void StartCommTask(void *argument);



/* ==============================================================================
 * Private Task Implementations
 * ============================================================================== */
/**
 * @brief  High-priority real-time control task
 * @details Toggles onboard LD2 (PA5) periodically with jitter compensation
 * @param  argument: Unused
 */
static void StartControlTask(void *argument)
{
    (void)argument;
    uint32_t tick = osKernelGetTickCount();

    for (;;)
    {
        /* 1. Toggle hardware indicator */
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        /* 2. Absolute delay to eliminate periodic drift and minimize jitter */
        tick += CONTROL_TASK_PERIOD_MS;
        osDelayUntil(tick);
    }
}


/**
 * @brief  Normal-priority communication and telemetry task
 * @details Transmits uptime and alive heartbeat over UART VCP console
 * @param  argument: Unused
 */
static void StartCommTask(void *argument)
{
    (void)argument;
    uint32_t tick = osKernelGetTickCount();
    uint32_t loop_count = 0;

    for (;;)
    {
        /* 1. Output diagnostic telemetry */
        loop_count++;
        printf("[CommTask] Uptime: %lu ms | Count: %lu\r\n", osKernelGetTickCount(), loop_count);

        /* 2. Absolute delay to prevent UART latency cumulative drift */
        tick += COMM_TASK_PERIOD_MS;
        osDelayUntil(tick);
    }
}



/* ==============================================================================
 * Public Functions
 * ============================================================================== */
/**
 * @brief  Initializes application tasks and RTOS kernel objects
 */
void App_Init(void)
{
    /* 1. Instantiate ControlTask (High Priority, 1KB Stack) */
    const osThreadAttr_t control_attr =
    {
        .name       = "ControlTask",
        .stack_size = DEFAULT_STACK_SIZE_BYTES,
        .priority   = (osPriority_t)osPriorityHigh,
    };
    s_controlTaskHandle = osThreadNew(StartControlTask, NULL, &control_attr);

    /* 2. Instantiate CommTask (Normal Priority, 1KB Stack) */
    const osThreadAttr_t comm_attr =
    {
        .name       = "CommTask",
        .stack_size = DEFAULT_STACK_SIZE_BYTES,
        .priority   = (osPriority_t)osPriorityNormal,
    };
    s_commTaskHandle = osThreadNew(StartCommTask, NULL, &comm_attr);
}

