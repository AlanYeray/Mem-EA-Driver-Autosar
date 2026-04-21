# AUTOSAR EA (EEPROM Abstraction) Driver

Implementation of the **EEPROM Abstraction (EA)** module following the AUTOSAR Classic Platform standard. This driver provides a uniform interface to access EEPROM data, abstracting the underlying memory hardware through the Memory Access (MemAcc) and Memory Interface (MemIf) layers.

### Problem
The AUTOSAR memory stack requires an abstraction layer between the application logic and the microcontroller's internal EEPROM memory. Implementing this layer correctly is critical to system reliability.

### Solution
Implementation of the AUTOSAR EA (EEPROM Abstraction) module from scratch in C on the NXP S32K144, acting as the lower layer of the MemIF interface and directly managing internal memory operations.

### Result
Enables the AUTOSAR stack to access memory in a standardized and portable manner, separating business logic from the physical details of the hardware.

## Overview

The EA module sits between the **NvM (NVRAM Manager)** and the low-level **MemAcc/Mem_43_EEP** drivers in the AUTOSAR memory stack. It manages logical block addressing, read/write operations, block invalidation, and immediate data block erasure for non-volatile storage on NXP S32K1xx microcontrollers.

```
┌─────────────────────────┐
│     NvM (NVRAM Manager) │
└────────────┬────────────┘
             │
┌────────────▼────────────┐
│    MemIf (Memory If)    │
└────────────┬────────────┘
             │
┌────────────▼────────────┐
│  ► EA (EEPROM Abstrac.) │  ◄── This module
└────────────┬────────────┘
             │
┌────────────▼────────────┐
│  MemAcc / Mem_43_EEP    │
└────────────┬────────────┘
             │
┌────────────▼────────────┐
│   EEPROM / Flash HW     │
└─────────────────────────┘
```

## Features

- **Ea_Init** — Module initialization with configurable parameters
- **Ea_Read** — Asynchronous read with block offset and length validation
- **Ea_Write** — Asynchronous write with logical-to-physical address mapping
- **Ea_Cancel** — Cancel ongoing operations via MemAcc
- **Ea_InvalidateBlock** — Mark logical blocks as invalid
- **Ea_EraseImmediateBlock** — Erase blocks flagged as immediate data
- **Ea_GetStatus / Ea_GetJobResult** — Query module and job state
- **Ea_GetVersionInfo** — Retrieve SW version information
- **Ea_MainFunction** — Cyclic processing of pending jobs and internal management (garbage collection)
- **Ea_JobEndNotification** — Callback from MemAcc with optional NvM notification

## AUTOSAR Compliance

Developed following the AUTOSAR SWS (Software Specification) for the EA module. Key SWS references implemented:

| Requirement | Description |
|---|---|
| SWS_Ea_00006 | Block number validation (reserved values 0x0000, 0xFFFF) |
| SWS_Ea_00075 | Virtual page size configuration |
| SWS_Ea_00079 | Number of write cycles per logical block |
| SWS_Ea_00104 | Immediate data block check in MainFunction |
| SWS_Ea_00128 | Module transitions to IDLE after init |
| SWS_Ea_00153 | Job end notification handling |
| SWS_Ea_00178 | Internal busy state management |
| SWS_Ea_00191 | NULL pointer check on init |
| SWS_Ea_00197 | Buffer alignment configuration |
| SWS_Ea_00198 | Minimum read page size |

## AUTOSAR BSW Modules Included

This repository includes a comprehensive set of AUTOSAR MCAL and BSW modules for NXP S32K1xx (S32K116, S32K118, S32K142, S32K144, S32K146, S32K148) and S32M24x:

| Layer | Modules |
|---|---|
| **MCAL** | ADC, CAN (FlexCAN), DIO, EEP (Mem_43_EEP), Fee, GPT, MCL (DMA, Cache, Trgmux), MCU (Clock, Power, RAM), Mem (INFLS), MemAcc, MemIf, Platform, Port, PWM, SPI |
| **ECU Abstraction** | CanIf |
| **Services** | Det (Development Error Tracer), EcuM (ECU State Manager) |
| **RTE** | SchM (BSW Scheduler) for all modules |

## Target Hardware

- **MCU Family:** NXP S32K1xx (ARM Cortex-M0+/M4)
- **Toolchains supported:** GCC, GHS (Green Hills), IAR
- **Linker scripts:** Provided for Flash and RAM configurations

## Project Structure

```
├── Ea.c                    # EA driver implementation
├── Ea.h                    # EA driver header & configuration
├── Autosar/
│   ├── CanIf/              # CAN Interface
│   ├── Det/                # Development Error Tracer
│   ├── EcuM/               # ECU State Manager
│   ├── Mcal/               # Microcontroller Abstraction Layer
│   │   ├── Base/           # Standard types, compiler abstractions, OS interface
│   │   ├── Adc/            # ADC driver
│   │   ├── Can/            # FlexCAN driver
│   │   ├── Dio/            # Digital I/O driver
│   │   ├── Eep/            # EEPROM emulation driver (Mem_43_EEP)
│   │   ├── Fee/            # Flash EEPROM Emulation
│   │   ├── Gpt/            # General Purpose Timer
│   │   ├── Mcl/            # MCL (DMA, Cache, Trgmux, Flexio)
│   │   ├── Mcu/            # MCU driver (Clock, Power, RAM)
│   │   ├── Mem/            # Internal Flash driver (Mem_43_INFLS)
│   │   ├── MemAcc/         # Memory Access module
│   │   ├── MemIf/          # Memory Abstraction Interface
│   │   ├── Platform/       # Platform (IntCtrl, MPU, startup)
│   │   ├── Port/           # Port driver
│   │   ├── Pwm/            # PWM driver (FTM, Flexio)
│   │   └── Spi/            # SPI driver (LPSPI, Flexio)
│   └── Rte/                # Runtime Environment (SchM)
```

## Configuration

Block configuration is defined in `Ea.h` through static tables:

- **Ea_BlockConfigType** — Defines logical blocks with size and immediate data flag
- **Ea_BlockAddressType** — Maps logical blocks to physical EEPROM addresses
- **Ea_BlockToAreaMapping** — Associates blocks with MemAcc address areas

Key configuration macros:

```c
#define EA_VIRTUAL_PAGE_SIZE        8     // Logical block granularity
#define EaMinimumReadPageSize       8     // Minimum read alignment
#define EaBufferAlignmentValue      2     // Buffer alignment in bytes
#define NUM_CONFIGURED_BLOCKS       3     // Number of EA blocks
#define EA_DEV_ERROR_DETECT     STD_OFF   // DET error reporting
```

## Status

> **Work in progress** — This is a learning/development project. Some functions are marked as "ready" while others (like `Ea_MainFunction`) are still being refined. DET error reporting integration is partially complete.

## Author

Developed as a personal project to learn AUTOSAR BSW development and embedded firmware architecture targeting NXP S32K1xx microcontrollers.

## License

This project is intended for educational and portfolio purposes.
