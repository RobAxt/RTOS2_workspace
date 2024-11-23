/*
 * Copyright (c) YEAR NOMBRE <MAIL>.
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
 * @file   : driver_uart.c
 * @date   : Apr 1, 2023
 * @author : NOMBRE <MAIL>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"

#include "ringbuffer.h"

/********************** macros and definitions *******************************/

#define TX_BUFFER_SIZE_         (16)
#define RX_BUFFER_SIZE_         (16)
#define RB_TX_BUFFER_SIZE_      (128)
#define RB_RX_BUFFER_SIZE_      (128)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static bool init_ = false;

static uint8_t tx_buffer_[TX_BUFFER_SIZE_];
static uint8_t rx_buffer_[RX_BUFFER_SIZE_];

static bool tx_free_;

static ringbuffer_t rb_tx_;
static uint8_t rb_tx_buffer_[RB_TX_BUFFER_SIZE_];

static ringbuffer_t rb_rx_;
static uint8_t rb_rx_buffer_[RB_RX_BUFFER_SIZE_];

/********************** external data definition *****************************/

extern UART_HandleTypeDef huart3;

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  // TODO:
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t size)
{
  if((&huart3) != huart)
  {
    return;
  }

  if(0 < size)
  {
    ringbuffer_write(&rb_rx_, rx_buffer_, size);
  }
  HAL_UARTEx_ReceiveToIdle_IT(&huart3, rx_buffer_, RX_BUFFER_SIZE_);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
  if((&huart3) != huart)
  {
    return;
  }

  if(ringbuffer_is_empty(&rb_tx_))
  {
    tx_free_ = true;
  }
  else
  {
    size_t tx_len = ringbuffer_read(&rb_tx_, tx_buffer_, TX_BUFFER_SIZE_);
    HAL_UART_Transmit_IT(&huart3, tx_buffer_, tx_len);
  }
}

void driver_uart_init(void)
{
  tx_free_ = true;

  ringbuffer_init(&rb_tx_, rb_tx_buffer_, RB_TX_BUFFER_SIZE_);
  ringbuffer_init(&rb_rx_, rb_rx_buffer_, RB_RX_BUFFER_SIZE_);

  HAL_UARTEx_ReceiveToIdle_IT(&huart3, rx_buffer_, RX_BUFFER_SIZE_);

  init_ = true;
}

size_t driver_uart_rx(uint8_t* buffer, size_t size)
{
  while(false == init_);
  return ringbuffer_read(&rb_rx_, buffer, size);
}

size_t driver_uart_tx(uint8_t* buffer, size_t size)
{
  while(false == init_);
  taskENTER_CRITICAL();
  size_t ret = ringbuffer_write(&rb_tx_, buffer, size);
  if(tx_free_)
  {
    size_t tx_len = ringbuffer_read(&rb_tx_, tx_buffer_, TX_BUFFER_SIZE_);
    HAL_UART_Transmit_IT(&huart3, tx_buffer_, tx_len);
    tx_free_ = false;
  }
  taskEXIT_CRITICAL();
  return ret;
}

/********************** end of file ******************************************/
