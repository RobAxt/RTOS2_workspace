
#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "ao.h"

static const uint8_t QUEUE_LENGTH_= 3;
static const uint8_t QUEUE_ITEM_SIZE_ = sizeof(ao_event_t);

struct ao_s
{
  QueueHandle_t queue_h;
  ao_event_handler_t event_h;
};

static void task_(void *argument);

ao_t ao_init(ao_event_handler_t event_handler)
{
  ao_t ao = (ao_t)pvPortMalloc(sizeof(struct ao_s));
  while (NULL == ao)
  {
    // error
  }

  ao->queue_h = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
  while(NULL == ao->queue_h)
  {
    // error
  }

  vQueueAddToRegistry(ao->queue_h, "Queue Handle");

  ao->event_h = event_handler;
  while(NULL == ao->event_h)
  {
    // error
  }

  BaseType_t status;
  status = xTaskCreate(task_, "task_ao_", 128, (void* const)ao, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status)
  {
    // error
  }

  return ao;
}

bool ao_send(ao_t ao, ao_event_t event)
{
  return (pdPASS == xQueueSend(ao->queue_h, (void*)&event, (TickType_t)0));
}

static void task_(void *argument)
{
  ao_t ao = (ao_t)argument;
  while (true)
  {
    ao_event_t msg;
    if (pdPASS == xQueueReceive(ao->queue_h, &msg, portMAX_DELAY))
    {
	ao->event_h(msg);
    }
  }
}
