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
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "app.h"
#include "memory_pool.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (50)

#define BUTTON_PERIOD_MS_         (TASK_PERIOD_MS_)
#define BUTTON_PULSE_TIMEOUT_     (100)

/********************** internal data declaration ****************************/

typedef enum
{
  BUTTON_TYPE_NONE,
  BUTTON_TYPE_PULSE,
  BUTTON_TYPE__N,
} button_type_t;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static struct
{
    uint32_t counter;
} button_;

int msg_wip_ = 0;

/********************** external data definition *****************************/

extern QueueHandle_t hqueue;
extern memory_pool_t* const hmp;

/********************** internal functions definition ************************/

static void button_init_(void)
{
  button_.counter = 0;
}

static button_type_t button_process_state_(bool value)
{
  button_type_t ret = BUTTON_TYPE_NONE;
  if(value)
  {
    button_.counter += BUTTON_PERIOD_MS_;
  }
  else
  {
    if(BUTTON_PULSE_TIMEOUT_ <= button_.counter)
    {
      ret = BUTTON_TYPE_PULSE;
    }
    button_.counter = 0;
  }
  return ret;
}

static void callback_process_completed_(msg_t* pmsg)
{
  memory_pool_block_put(hmp, (void*)pmsg);
  LOGGER_INFO("Memoria liberada desde button");
  msg_wip_--;
  LOGGER_INFO("Mensajes en proceso: %d", msg_wip_);
}

/********************** external functions definition ************************/

void task_button(void* argument)
{
  button_init_();

  while(true)
  {
    GPIO_PinState button_state;
    button_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);

    button_type_t button_type;
    button_type = button_process_state_(button_state);

    {
      if(BUTTON_TYPE_PULSE == button_type)
      {
        msg_t* pmsg = (msg_t*)memory_pool_block_get(hmp);

        if(NULL != pmsg)
        {
          LOGGER_INFO("Memoria alocada: %d", sizeof(msg_t));
          pmsg->size = sizeof(msg_t);
          pmsg->callback_process_completed = callback_process_completed_;
          if(pdPASS == xQueueSend(hqueue, (void*)&pmsg, 0))
          {
            LOGGER_INFO("Mensaje enviado");
            msg_wip_++;
            LOGGER_INFO("Mensajes en proceso: %d", msg_wip_);
          }
          else
          {
            LOGGER_INFO("Mensaje no enviado");
            memory_pool_block_put(hmp, (void*)pmsg);
            LOGGER_INFO("Memoria liberada desde button");
          }
        }
        else
        {
          LOGGER_INFO("Memoria insuficiente");
        }
      }
    }

    vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  }
}

/********************** end of file ******************************************/
