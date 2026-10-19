
#include "control_task.h"
#include "telemetry_def.h"
#include "main.h"


/* ==============================================================================
 * Private Constants & Definitions
 * ============================================================================== */
#define CONTROL_TASK_PERIOD_MS    (10U)
#define DOWNSAMPLE_RATIO          (5U)
#define CONTROL_STACK_SIZE_BYTES  (256 * 4)


/* ==============================================================================
 * Private Variables
 * ============================================================================== */
static osThreadId_t s_controlTaskHandle = NULL;


/* ==============================================================================
 * Private Functions
 * ============================================================================== */
static void ControlTask_Entry(void *argument)
{
    (void)argument;

    uint32_t tick = osKernelGetTickCount();
    uint32_t loop_counter = 0U;

    float dummy_pv   = 0.0f;
    float dummy_sp   = 100.0f;
    float dummy_duty = 25.0f;

    for (;;)
    {
        loop_counter++;

        if ((loop_counter % 50U) == 0U)
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        }

        dummy_pv += 0.5f;
        if (dummy_pv > 120.0f)
        {
            dummy_pv = 0.0f;
        }

        /* Downsampled queue insertion (50ms interval) */
        if ((loop_counter % DOWNSAMPLE_RATIO) == 0U)
        {
            TelemetryData_t packet =
            {
                .timestamp_ms  = osKernelGetTickCount(),
                .setpoint      = dummy_sp,
                .process_value = dummy_pv,
                .duty_cycle    = dummy_duty
            };

            if (g_telemetryQueue != NULL)
            {
                (void)osMessageQueuePut(g_telemetryQueue, &packet, 0U, 0U);
            }
        }

        tick += CONTROL_TASK_PERIOD_MS;
        osDelayUntil(tick);
    }
}


/* ==============================================================================
 * Public Functions
 * ============================================================================== */
void ControlTask_Init(void)
{
    const osThreadAttr_t attr =
    {
        .name       = "ControlTask",
        .stack_size = CONTROL_STACK_SIZE_BYTES,
        .priority   = (osPriority_t)osPriorityHigh,
    };
    
    s_controlTaskHandle = osThreadNew(ControlTask_Entry, NULL, &attr);
}
