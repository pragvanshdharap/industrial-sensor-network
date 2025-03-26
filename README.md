# Industrial Sensor Network with CAN Bus and Wi-Fi Monitoring

## Overview
This project implements an **Industrial Sensor Network** using STM32 microcontrollers, CAN Bus for communication, and ESP32 for Wi-Fi-based remote monitoring. It is designed for industrial environments to monitor sensor data in real-time and transmit the data to a cloud server or local server for remote access.

## Key Features
- STM32 reads industrial sensor data via I2C.
- CAN Bus enables communication between multiple STM32 nodes.
- ESP32 sends data to the cloud or a local server via Wi-Fi.
- Scalable design suitable for real-time monitoring in industrial setups.


## Setup

### 1. Hardware Setup
- Connect the STM32 to sensors via I2C.
- Connect the STM32 to a CAN transceiver (e.g., MCP2551) for CAN Bus communication.
- Connect the STM32 to the ESP32 using UART.

For detailed wiring instructions, refer to the [Setup Guide](docs/setup-guide.md).

### 2. Firmware
- **STM32 Firmware**: Code for reading sensor data, transmitting over CAN, and communicating with ESP32 via UART.
- **ESP32 Firmware**: Code for receiving data from STM32 and sending it to the cloud via Wi-Fi.
