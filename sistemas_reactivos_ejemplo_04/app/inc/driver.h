/*
 * Copyright (c) 2024 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : driver.c
 * @date   : Feb 17, 2023
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include "main.h"
#include "cmsis_os.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

typedef enum
{
  DRIVER_GPIO_LEDR,
  DRIVER_GPIO_LEDG,
  DRIVER_GPIO_LEDB,
  DRIVER_GPIO_SW,
  DRIVER_GPIO__CNT,
} driver_gpio_idx_t;

typedef struct
{
  eboard_gpio_idx_t idx;
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin;
} driver_gpio_descriptor_t_;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static driver_gpio_descriptor_t_ driver_gpios_[] = { {idx: EBOARD_GPIO_LEDR, GPIOx: GPIOB, GPIO_Pin: GPIO_PIN_14}, // LED3
    {idx: EBOARD_GPIO_LEDG, GPIOx: GPIOB, GPIO_Pin: GPIO_PIN_0}, // LED1
    {idx: EBOARD_GPIO_LEDB, GPIOx: GPIOB, GPIO_Pin: GPIO_PIN_7}, // LED2
    {idx: EBOARD_GPIO_SW, GPIOx: GPIOC, GPIO_Pin: GPIO_PIN_13}, // USER BTN
    };

extern UART_HandleTypeDef huart3;
UART_HandleTypeDef *p_huart_selected_ = &huart3;

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void euart_hal_receive(void *phardware_handle, uint8_t *pbuffer, size_t size)
{
  HAL_UARTEx_ReceiveToIdle_IT((UART_HandleTypeDef*)phardware_handle, pbuffer, size);
}

void euart_hal_send(void *phardware_handle, uint8_t *pbuffer, size_t size)
{
  HAL_UART_Transmit_IT((UART_HandleTypeDef*)phardware_handle, pbuffer, size);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  eboard_hal_port_uart_error((void*)huart);
  // TODO: ¿?
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
  eboard_hal_port_uart_rx_irq((void*)huart, size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  eboard_hal_port_uart_tx_irq((void*)huart);
}

void eboard_hal_port_gpio_write(void *handle, bool value)
{
  driver_gpio_descriptor_t_ *hgpio = (driver_gpio_descriptor_t_*)handle;
  HAL_GPIO_WritePin(hgpio->GPIOx, hgpio->GPIO_Pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool eboard_hal_port_gpio_read(void *handle)
{
  driver_gpio_descriptor_t_ *hgpio = (driver_gpio_descriptor_t_*)handle;
  GPIO_PinState state = HAL_GPIO_ReadPin(hgpio->GPIOx, hgpio->GPIO_Pin);
  return (GPIO_PIN_SET == state);
}

uint32_t eboard_osal_port_get_time(void)
{
  return (uint32_t)xTaskGetTickCount();
}

void eboard_osal_port_delay(uint32_t time_ms)
{
  vTaskDelay((TickType_t)((time_ms) / portTICK_PERIOD_MS));
}

/********************** end of file ******************************************/
