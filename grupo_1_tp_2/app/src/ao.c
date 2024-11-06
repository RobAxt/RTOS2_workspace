
#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "memory_pool.h"
#include "ao.h"


static void task_(void *argument);

struct ao_event_s
{
  ao_t ao_h;
  ao_msj_t msj_h;
};

struct ao_s
{
  ao_event_handler_t event_h;
};

static const uint8_t QUEUE_LENGTH_= 4;
static const uint8_t QUEUE_ITEM_SIZE_ = sizeof(ao_event_t);

static QueueHandle_t queue_h;

static const uint8_t MEMORY_POOL_NBLOCKS = 4;
static const uint8_t MEMORY_POOL_BLOCK_SIZE = sizeof(struct ao_s);

static memory_pool_t memory_pool_;
static uint8_t memory_pool_memory_[MEMORY_POOL_SIZE(MEMORY_POOL_NBLOCKS, MEMORY_POOL_BLOCK_SIZE)];

memory_pool_t* const hmp = &memory_pool_;

ao_t ao_init(ao_event_handler_t event_handler)
{
  static bool task_running = pdFALSE;

  if(pdFALSE != task_running)
  {
     memory_pool_init(hmp, memory_pool_memory_, MEMORY_POOL_NBLOCKS, MEMORY_POOL_BLOCK_SIZE);

     queue_h = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
     while(NULL == queue_h)
     {
       // error
     }
     vQueueAddToRegistry(queue_h, "Queue Handle");

     BaseType_t status = xTaskCreate(task_, "task_ao_", 128, NULL, tskIDLE_PRIORITY, NULL);
     while (pdPASS != status)
     {
        // error
     }

     task_running = pdTRUE;
  }

    ao_t ao = (ao_t)memory_pool_block_get(hmp);
    while (NULL == ao)
    {
      // error
    }

    ao->event_h = event_handler;
    while(NULL == ao->event_h)
    {
      // error
    }

  return ao;
}

bool ao_send(ao_t ao, ao_msj_t eventMsj) // TODO: unificar los returns.
{
  ao_event_t event = (ao_event_t)pvPortMalloc(sizeof(struct ao_event_s));
  if(NULL != event)
  {
    event->ao_h = ao;
    event->msj_h = eventMsj;
    if(pdPASS != xQueueSend(queue_h, (void*)&event, (TickType_t)0))
    {
	vPortFree((void*)event);
	return pdFALSE;
    }
    return pdTRUE;
  }
  return pdFALSE;
}

static void task_(void *argument)
{
  while (true)
  {
    ao_event_t msg;
    if (pdPASS == xQueueReceive(queue_h, &msg, portMAX_DELAY))
    {
	msg->ao_h->event_h(msg->msj_h);
	vPortFree((void*)msg);
    }
  }
}