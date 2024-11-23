/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
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
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "driver_uart.h"

/********************** macros and definitions *******************************/

#define TASK_READER_PERIOD_MS_  (5000)
#define MSG_TICK_               ("tick")
#define RB_BUFFER_SIZE_         (16)

/********************** internal data declaration ****************************/

typedef struct
{
  char *msg;
  uint32_t period_ms;
} task_write_args_t;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static char msg1_[] = "Real";
static char msg2_[] = "Time";
static char msg3_[] = "System";

static task_write_args_t args_[] = {
                            {.msg = msg1_, .period_ms = 1000},
                            {.msg = msg2_, .period_ms = 2000},
                            {.msg = msg3_, .period_ms = 3000},
};

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

static void task_writer_(void* argument)
{
  task_write_args_t *pargs = (task_write_args_t*)argument;
  while(true)
  {
    size_t msg_len = strlen(pargs->msg);
    size_t ret = driver_uart_tx((uint8_t*)pargs->msg, msg_len);
    LOGGER_INFO("writer send: %s, %d bytes", pargs->msg, ret);
    vTaskDelay((TickType_t)(pargs->period_ms / portTICK_PERIOD_MS));
  }
}

/********************** external functions definition ************************/

void app_init(void)
{
  LOGGER_LOG("tasks init");

  driver_uart_init();

  BaseType_t status;

  for(int i = 0; i < sizeof(args_)/sizeof(task_write_args_t); ++i)
  {
    status = xTaskCreate(task_writer_, "task_write", 128, (void*)(args_+i), tskIDLE_PRIORITY, NULL);
    while (pdPASS != status)
    {
      // error
    }
  }

  cycle_counter_init();
}

/********************** end of file ******************************************/
