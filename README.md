# NUCLEO-F401RE: FreeRTOS 기반 폐루프 PID 제어기

STM32 NUCLEO-F401RE 보드와 FreeRTOS(CMSIS-RTOS V2)를 활용하여 정밀 주기를 보장하는 폐루프(Closed-Loop) PID 제어 시스템을 구축하는 프로젝트입니다.  
물리적 모터가 없는 환경에서도 소프트웨어 1차 지연 플랜트 모델을 통해 제어기 응답 곡선을 PC 시각화 도구로 검증할 수 있도록 설계되었습니다.

---

## 🛠 개발 환경 (Development Environment)

* **Target Board**: STMicroelectronics NUCLEO-F401RE (ARM Cortex-M4 @ 84MHz)
* **IDE / Toolchain**: STM32CubeIDE (v2.1.1) / GNU Arm Embedded Toolchain
* **Configuration Tool**: STM32CubeMX
* **OS / Middleware**: FreeRTOS (CMSIS-RTOS V2)
* **HAL Driver**: STM32F4xx HAL Driver (Timebase: `TIM10`)
* **Visualization Tool**: Serial Studio / Arduino Serial Plotter (Baudrate: 115200)

---

## 📚 핵심 개념 (Core Concepts)

### 1. 왜 FreeRTOS를 사용하는가? (Why FreeRTOS?)
기존의 메인 루프(`while(1)`) 기반 제어는 통신이나 복잡한 계산이 끼어들 경우 루프 주기(Cycle)가 불규칙하게 늘어나는 **지터(Jitter)**가 발생합니다.
* **주기 결정론(Determinism)**: `vTaskDelayUntil()`을 사용해 다른 작업의 지연과 무관하게 10ms(100Hz) 정밀 제어 주기를 사수합니다.
* **우선순위 기반 선점(Preemption)**: 제어 루프(`osPriorityHigh`)가 텔레메트리 출력 루프(`osPriorityNormal`)보다 우선권을 가져 제어 안정성을 보장합니다.

### 2. 폐루프(Closed-Loop) PID 제어란?
출력 결과(현재 속도)를 센서로 측정해 목표값과의 오차(Error)를 구한 뒤, 실시간으로 출력을 보정하는 피드백 제어입니다.

$$u(t) = K_p \, e(t) + K_i \int e(t)\,dt + K_d \frac{de(t)}{dt}$$

* **P (비례항)**: 오차 크기에 비례하여 즉각적인 복원력 생성
* **I (적분항)**: 누적된 오차를 반영해 미세한 정상상태 오차(Steady-State Error) 제거 (*Anti-Windup 적용*)
* **D (미분항)**: 오차의 변화 속도를 감지하여 오버슈트 억제 및 급격한 외란에 댐핑 효과 부여

---

## 🏗 시스템 아키텍처 및 데이터 흐름도 (Data Flow)

```mermaid
flowchart TD
    subgraph STM32 [STM32 NUCLEO-F401RE]
        subgraph ControlTask [ControlTask: High Priority, 10ms 주기]
            SP[목표 속도: Setpoint] --> ERR[오차 계산: Error = SP - PV]
            ERR --> PID[이산 PID 알고리즘 연산]
            PID --> OUT[제어 출력: Duty Saturation]
            OUT --> PLANT[가상 모터 모델: 1차 지연 수식]
            PLANT --> PV[가상 센서 측정값: Process Value]
            PV -.->|피드백 루프| ERR
        end

        subgraph CommTask [CommTask: Normal Priority, 50ms 주기]
            LOG[데이터 포맷팅: CSV]
            OUT -.-> LOG
            PV -.-> LOG
            SP -.-> LOG
        end
    end

    subgraph PC [PC Monitoring]
        UART[USART2 Virtual COM Port]
        PLOTTER[Serial Studio / Serial Plotter]
        LOG -->|115200 bps| UART
        UART --> PLOTTER
    end
