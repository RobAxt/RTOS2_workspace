
#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "pao.h"

static const uint8_t QUEUE_LENGTH_= 3;
static const uint8_t QUEUE_ITEM_SIZE_ = sizeof(pao_event_t);

struct pao_s
{
  QueueHandle_t queue_h;   // TODO: declarar queue
  pao_event_handler_t event_h;
  bool used;
};

static void task_(void *argument);

pao_t pao_init(pao_event_handler_t event_handler)
{
  pao_t pao = (pao_t)pvPortMalloc(sizeof(struct pao_s));
  while (NULL == pao)
  {
    // error
  }

  pao->queue_h = NULL;   // TODO: Crear la fucking cola
  while(NULL == pao->queue_h)
  {
    // error
  }

  pao->event_h = event_handler;
  while(NULL == pao->event_h)
  {
    // error
  }

  BaseType_t status;
  status = xTaskCreate(task_, "task_ao_", 128, (void* const)pao, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status)
  {
    // error
  }

  return pao;
}

bool pao_send(pao_t pao, pao_event_t event)
{
  return (pdPASS == xQueueSend(pao->queue_h, (void*)&event, (TickType_t)0)); // TODO: enqueue
}

static void task_(void *argument)
{
  pao_t pao = (pao_t)argument;
  while (true)
  {
    pao_event_t msg;
    if (pdPASS == xQueueReceive(pao->queue_h, &msg, portMAX_DELAY)) // TODO: dequeue
    {
	pao->event_h(msg);
    }
  }
}
