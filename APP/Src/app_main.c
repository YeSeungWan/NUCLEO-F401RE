#include "app_main.h"
#include "telemetry_def.h"
#include "control_task.h"
#include "comm_task.h"


/* ==============================================================================
 * Private Constants & Definitions
 * ============================================================================== */
#define TELEMETRY_QUEUE_CAPACITY    (8U)


/* ==============================================================================
 * Public Variables
 * ============================================================================== */
osMessageQueueId_t g_telemetryQueue = NULL;


/* ==============================================================================
 * Public Functions
 * ============================================================================== */
void App_Init(void)
{
    /* 1. IPC Resource allocation */
    g_telemetryQueue = osMessageQueueNew(TELEMETRY_QUEUE_CAPACITY, sizeof(TelemetryData_t), NULL);

    /* 2. Sub-task initializations */
    ControlTask_Init();
    CommTask_Init();
}
