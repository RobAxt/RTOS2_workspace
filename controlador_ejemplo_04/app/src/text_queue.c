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
 * @author : Sebastian Bedin <sebabedin@gmail.com>
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

#define RB_BUFFER_SIZE_         (16)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static ringbuffer_t rb_;
static uint8_t rb_buffer_[RB_BUFFER_SIZE_];

static SemaphoreHandle_t hsem_write_;
static SemaphoreHandle_t hsem_read_;

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

static void check_buffer_state_(void)
{
  if(false == ringbuffer_is_full(&rb_))
  {
    xSemaphoreGive(hsem_write_);
  }

  if(false == ringbuffer_is_empty(&rb_))
  {
    xSemaphoreGive(hsem_read_);
  }
}

/********************** external functions definition ************************/

size_t text_queue_init(void)
{
  ringbuffer_init(&rb_, rb_buffer_, RB_BUFFER_SIZE_);

  hsem_write_ = xSemaphoreCreateBinary();
  while(NULL == hsem_write_)
  {
    // Error
  }
  hsem_read_ = xSemaphoreCreateBinary();
  while(NULL == hsem_read_)
  {
    // Error
  }

  xSemaphoreTake(hsem_write_, 0);
  xSemaphoreTake(hsem_read_, 0);

  check_buffer_state_();
}

size_t text_queue_write(const uint8_t *buffer, size_t size, TickType_t xBlockTime)
{
  size_t ret = 0;
  if(pdTRUE == xSemaphoreTake(hsem_write_, xBlockTime))
  {
    ret = ringbuffer_write(&rb_, buffer, size);
    check_buffer_state_();
  }

  return ret;
}

size_t text_queue_read(uint8_t *buffer, size_t size, TickType_t xBlockTime)
{
  size_t ret = 0;
  if(pdTRUE == xSemaphoreTake(hsem_read_, xBlockTime))
  {
    ret = ringbuffer_read(&rb_, buffer, size);
    check_buffer_state_();
  }

  return ret;
}

/********************** end of file ******************************************/
