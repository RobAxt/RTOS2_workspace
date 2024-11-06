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
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 */

/********************** inclusions *******************************************/
#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

/********************** macros and definitions *******************************/

#define QUEUE_LENGTH_           (10)
#define QUEUE_ITEM_SIZE_        (sizeof(int))
#define TASK_PERIOD_MS_         (50)

/********************** internal data declaration ****************************/

static QueueHandle_t hqueue;
static bool msg_task_active;

/********************** internal functions declaration ***********************/

bool process_system_state(int* pmsg);

/********************** internal data definition *****************************/

static void task_msg_processing(void *parg)
{
  while (true)
  {
    int msg;
    while (pdPASS == xQueueReceive(hqueue, (void*)&msg, 0))
    {
      // process msg
    }
    msg_task_active = false;
    vTaskDelete(NULL);
  }
}

static void send_msg(int msg)
{
  xQueueSend(hqueue, (void*)&msg, 0);
  if(!msg_task_active)
  {
    xTaskCreate(task_msg_processing, "task_msg_processing", 128, NULL, tskIDLE_PRIORITY, NULL);
    msg_task_active = true;
  }
}

static void task_system(void *parg)
{
  int msg;
  while (true)
  {
    if(process_system_state(&msg))
    {
      send_msg(msg);
    }
    vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  }
}

/********************** external data declaration *****************************/


/********************** external functions definition ************************/
void app_init(void)
{
  hqueue = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
  xTaskCreate(task_system, "task_system", 128, NULL, tskIDLE_PRIORITY, NULL);
  cycle_counter_init();
}

/********************** end of file ******************************************/
