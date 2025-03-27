/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* UART handle */
UART_HandleTypeDef huart2;

/* DHT11 Pin and data structure */
#define DHT11_PIN GPIO_PIN_1
#define DHT11_PORT GPIOA
typedef struct {
    float temperature;
    float humidity;
} DHT11_Data;

/* UART transmit buffer */
char uart_buffer[100];

/* Function prototypes */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void DHT11_Init(void);
void DHT11_Start(void);
uint8_t DHT11_CheckResponse(void);
uint8_t DHT11_ReadData(void);
void DHT11_Read(DHT11_Data *data);
void send_sensor_data_via_uart(DHT11_Data *data);

int main(void) {
    /* Initialize the hardware */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    /* Initialize DHT11 sensor */
    DHT11_Init();

    /* DHT11 sensor data structure */
    DHT11_Data dht11_data;

    while (1) {
        /* Read temperature and humidity data from DHT11 */
        DHT11_Read(&dht11_data);

        /* Send the sensor data to ESP32 over UART */
        send_sensor_data_via_uart(&dht11_data);

        /* Delay for demonstration purposes */
        HAL_Delay(2000);  // 2 seconds delay
    }
}

/* Function to initialize DHT11 pin */
void DHT11_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/* Start communication with DHT11 sensor */
void DHT11_Start(void) {
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);  // Pull the pin low
    HAL_Delay(18);                                             // Wait for 18ms
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);    // Pull the pin high
    HAL_Delay(1);                                              // Wait for 20-40us
}

/* Check for DHT11 response */
uint8_t DHT11_CheckResponse(void) {
    uint8_t response = 0;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);  // Set pin as input

    HAL_Delay(40);
    if (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
        HAL_Delay(80);
        if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
            response = 1;  // Response received
        }
    }
    HAL_Delay(40);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);  // Set pin back to output

    return response;
}

/* Read data bit from DHT11 */
uint8_t DHT11_ReadData(void) {
    uint8_t i, data = 0;

    for (i = 0; i < 8; i++) {
        while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));  // Wait for pin to go high
        HAL_Delay(40);                                    // Wait for 40 us

        if (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
            data &= ~(1 << (7 - i));  // Write 0 if pin is low
        } else {
            data |= (1 << (7 - i));  // Write 1 if pin is high
        }

        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));  // Wait for pin to go low
    }

    return data;
}

/* Function to read temperature and humidity from DHT11 */
void DHT11_Read(DHT11_Data *data) {
    uint8_t rh_integral, rh_decimal, temp_integral, temp_decimal, checksum;

    DHT11_Start();
    if (DHT11_CheckResponse()) {
        rh_integral = DHT11_ReadData();
        rh_decimal = DHT11_ReadData();
        temp_integral = DHT11_ReadData();
        temp_decimal = DHT11_ReadData();
        checksum = DHT11_ReadData();

        if (checksum == (rh_integral + rh_decimal + temp_integral + temp_decimal)) {
            data->humidity = rh_integral + (rh_decimal / 10.0);
            data->temperature = temp_integral + (temp_decimal / 10.0);
        }
    }
}

/* Send the collected sensor data via UART to ESP32 */
void send_sensor_data_via_uart(DHT11_Data *data) {
    /* Prepare data to send over UART */
    sprintf(uart_buffer, "Temperature: %.2f, Humidity: %.2f\r\n",
            data->temperature, data->humidity);

    /* Transmit data over UART */
    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buffer, strlen(uart_buffer), HAL_MAX_DELAY);
}

/* UART2 Initialization Function */
void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        /* Initialization Error */
        Error_Handler();
    }
}

/* GPIO Initialization */
void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/* System Clock Configuration */
void SystemClock_Config(void) {
    /* Implementation generated by STM32CubeMX */
}

/* Error Handler */
void Error_Handler(void) {
    while (1) {
        /* Stay in loop if an error occurs */
    }
}
