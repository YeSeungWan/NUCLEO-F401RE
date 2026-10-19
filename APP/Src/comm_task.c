
#include "comm_task.h"
#include "telemetry_def.h"
#include <stdio.h>


/* ==============================================================================
 * Private Constants & Definitions
 * ============================================================================== */
#define COMM_STACK_SIZE_BYTES    (256 * 4)


/* ==============================================================================
 * Private Variables
 * ============================================================================== */
static osThreadId_t s_commTaskHandle = NULL;


/* ==============================================================================
 * Private Functions
 * ============================================================================== */
static void CommTask_Entry(void *argument)
{
    (void)argument;

    TelemetryData_t packet;
    osStatus_t status;

    for (;;)
    {
        status = osMessageQueueGet(g_telemetryQueue, &packet, NULL, osWaitForever);

        if (status == osOK)
        {
            printf("%lu,%.2f,%.2f,%.2f\r\n", packet.timestamp_ms, packet.setpoint, packet.process_value, packet.duty_cycle);
        }
    }
}


/* ==============================================================================
 * Public Functions
 * ============================================================================== */
void CommTask_Init(void)
{
    const osThreadAttr_t attr =
    {
        .name       = "CommTask",
        .stack_size = COMM_STACK_SIZE_BYTES,
        .priority   = (osPriority_t)osPriorityNormal,
    };

    s_commTaskHandle = osThreadNew(CommTask_Entry, NULL, &attr);
}
