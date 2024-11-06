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
 * @file   : ao_led.c
 * @date   : Feb 17, 2023
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 * @version	v1.0.0
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

#include <ao_led.h>

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (1000)

#define QUEUE_LENGTH_            (1)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_event_t))

/********************** internal data declaration ****************************/

typedef struct
{
    ao_led_handle_t* hao;
    ao_led_message_t msg;
} ao_event_t;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static GPIO_TypeDef* led_port_[] = {LED_RED_PORT, LED_GREEN_PORT,  LED_BLUE_PORT};
static uint16_t led_pin_[] = {LED_RED_PIN,  LED_GREEN_PIN, LED_BLUE_PIN };
static QueueHandle_t hqueue_;

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

static void task_(void *argument)
{
  while (true)
  {
    ao_event_t event;
    if (pdPASS == xQueueReceive(hqueue_, &event, portMAX_DELAY))
    {
      ao_led_handle_t* hao = event.hao;
      ao_led_message_t msg = event.msg;
      switch (msg.action) {
        case AO_LED_MESSAGE_ON:
          HAL_GPIO_WritePin(led_port_[hao->color], led_pin_[hao->color], GPIO_PIN_SET);
          msg.callback(msg.id);
          break;

        case AO_LED_MESSAGE_OFF:
          HAL_GPIO_WritePin(led_port_[hao->color], led_pin_[hao->color], GPIO_PIN_RESET);
          msg.callback(msg.id);
          break;

        case AO_LED_MESSAGE_BLINK:
          HAL_GPIO_WritePin(led_port_[hao->color], led_pin_[hao->color], GPIO_PIN_SET);
          vTaskDelay((TickType_t)((msg.value) / portTICK_PERIOD_MS));
          HAL_GPIO_WritePin(led_port_[hao->color], led_pin_[hao->color], GPIO_PIN_RESET);
          msg.callback(msg.id);
          break;

        default:
          break;
      }
    }
  }
}

/********************** external functions definition ************************/

bool ao_led_send(ao_led_handle_t* hao, ao_led_message_t* msg)
{
  ao_event_t event;
  event.hao = hao;
  event.msg = *msg;
  return (pdPASS == xQueueSend(hqueue_, (void*)&event, 0));
}

void ao_led_init(ao_led_handle_t* hao, ao_led_color color)
{
  hao->color = color;

  static bool init = false;
  if(init)
  {
    return;
  }

  hqueue_ = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
  while(NULL == hqueue_)
  {
    // error
  }

  BaseType_t status;
  status = xTaskCreate(task_, "task_ao", 128, NULL, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status)
  {
    // error
  }

  init = true;
}

/********************** end of file ******************************************/
