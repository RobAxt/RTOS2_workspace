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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "app.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (5000)
#define MAX_CONNECTION_           (3)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

static int task_cnt_;

/********************** external data definition *****************************/

extern QueueHandle_t hqueue;

/********************** internal functions definition ************************/

static void delete_task_(void)
{
  LOGGER_INFO("Elimino tarea");
  task_cnt_--;
  LOGGER_INFO("Cantidad de procesos: %d", task_cnt_);
  vTaskDelete(NULL);
}

static void task_connection_(void *argument)
{
  while (true)
  {
    int id;
    while (pdPASS == xQueueReceive(hqueue, (void*)&id, 0))
    {
      LOGGER_INFO("Procesando conexión: %d", id);
      vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
      LOGGER_INFO("Fin de la conexión: %d", id);
    }
    delete_task_();
  }
}

static bool create_new_task_connection_(void)
{
  LOGGER_INFO("Crear nueva tarea");
  if(task_cnt_ < MAX_CONNECTION_)
  {
    BaseType_t status;
    status = xTaskCreate(task_connection_, "task_connection", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    if(pdPASS != status)
    {
      LOGGER_INFO("No es posible crear mas tareas");
      return false;
    }
    LOGGER_INFO("Nueva tarea creata");
    task_cnt_++;
    LOGGER_INFO("Cantidad de procesos: %d", task_cnt_);
    return true;
  }
  else
  {
    LOGGER_INFO("Maxima cantidad de tareas creadas");
    return false;
  }
}

/********************** external functions definition ************************/

bool connection_new_connection(int id)
{
  LOGGER_INFO("Ingresa nueva conexión: %d", id);
  if(pdPASS == xQueueSend(hqueue, (void* )&id, 0))
  {
    if(0 == task_cnt_)
    {
      create_new_task_connection_();
    }
    LOGGER_INFO("Nueva tarea para la conexión %d", id);
    return true;
  }
  else if (create_new_task_connection_())
  {
    if (pdPASS == xQueueSend(hqueue, (void* )&id, 0))
    {
      LOGGER_INFO("Nueva tarea para la conexión %d", id);
      return true;
    }
    else
    {
      LOGGER_INFO("Conexión %d, Error: ¿Cuándo se podría dar este error?", id);
    }
  }
  else
  {
    LOGGER_INFO("Conexión %d, Error: No se puede crear más recursos", id);
  }
  LOGGER_INFO("Conexión %d, Error: No hay más lugar en la cola", id);

  return false;
}

/********************** end of file ******************************************/
