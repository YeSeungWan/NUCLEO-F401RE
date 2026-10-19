
#ifndef TELEMETRY_DEF_H_
#define TELEMETRY_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "cmsis_os2.h"

/* ==============================================================================
 * Shared Data Types
 * ============================================================================== */

typedef struct
{
    uint32_t timestamp_ms;
    float    setpoint;
    float    process_value;
    float    duty_cycle;
} TelemetryData_t;

/* ==============================================================================
 * Shared IPC Handles
 * ============================================================================== */

/**
 * @brief Global queue handle for telemetry streaming
 */
extern osMessageQueueId_t g_telemetryQueue;

#ifdef __cplusplus
}
#endif

#endif /* TELEMETRY_DEF_H_ */
