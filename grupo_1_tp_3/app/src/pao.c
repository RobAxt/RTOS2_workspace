#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "pao.h"

#define NON_BLOCKED_QUEUE_DELAY 10

struct pao_s
{
  priority_queue_t pqueue_h;
  pao_event_handler_t event_h;
};

static void task_(void *argument);

pao_t pao_init(pao_event_handler_t event_handler)
{
  pao_t pao = (pao_t)pvPortMalloc(sizeof(struct pao_s));
  while (NULL == pao)
  {
    // error
  }

  pao->pqueue_h =  init_queue();
  while(NULL == pao->pqueue_h)
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

bool pao_send(pao_t pao, pao_event_t event, priority_t priority)
{
  return (pdPASS == enqueue(pao->pqueue_h, priority, (data_t) event));
}

static void task_(void *argument)
{
  pao_t pao = (pao_t)argument;
  while (true)
  {
    pao_event_t msg;
    if (pdPASS == dequeue(pao->pqueue_h, (data_t *) &msg))
    {
	pao->event_h(msg);
    }
    vTaskDelay((TickType_t)((NON_BLOCKED_QUEUE_DELAY) / portTICK_PERIOD_MS));
  }
}
