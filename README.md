# Industrial Sensor Network: STM32 & ESP32 with DHT11

## Overview
This project implements a sensor network where an STM32 microcontroller collects temperature and humidity data from a DHT11 sensor and sends it to an ESP32 via UART. The ESP32 uploads the data to a cloud server over Wi-Fi.

## System Overview
- STM32 reads temperature and humidity data from a DHT11 sensor.
- The STM32 transmits sensor data to ESP32 via UART.
- The ESP32 connects to a Wi-Fi network and sends the data to a cloud server for remote monitoring.
