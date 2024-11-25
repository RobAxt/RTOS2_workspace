#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "pqueue.h"

#define MAX_SIZE 10

typedef struct {
    priority_t priority;
    data_t data;
} queue_data_t;

struct priority_queue_s {
    queue_data_t elements[MAX_SIZE];
    int size;
    SemaphoreHandle_t mtx;
};


priority_queue_t init_queue()
{
  priority_queue_t pqueue = (priority_queue_t)pvPortMalloc(sizeof(struct priority_queue_s));
  if(NULL != pqueue)
  {
    pqueue->size = 0;
    pqueue->mtx = xSemaphoreCreateMutex();
    while(NULL == pqueue->mtx)
    {
      //...error
    }
    vQueueAddToRegistry(pqueue->mtx, "Priority Queue Mutex Handle");
  }
  return pqueue;
}

bool enqueue(priority_queue_t queue, priority_t priority, data_t data)
{
  if(NULL == queue)
    return pdFALSE;

  xSemaphoreTake(queue->mtx, portMAX_DELAY);

  if(10 <= queue->size)
  {
    xSemaphoreGive(queue->mtx);
    return pdFALSE;
  }

   int8_t i;
   for(i = queue->size - 1; (i >= 0 && queue->elements[i].priority < priority); i--)
   {
     queue->elements[i + 1].priority = queue->elements[i].priority;
     queue->elements[i + 1].data = queue->elements[i].data;
   }

   queue->elements[i + 1].priority = priority;
   queue->elements[i + 1].data = data;
   queue->size++;

   xSemaphoreGive(queue->mtx);
   return pdTRUE;
}

bool dequeue(priority_queue_t queue, data_t * data)
{
  if(NULL == queue)
    return pdFALSE;

  xSemaphoreTake(queue->mtx, portMAX_DELAY);

  if(0 == queue->size)
  {
    xSemaphoreGive(queue->mtx);
    return pdFALSE;
  }

  *data = queue->elements[0].data;

  for(int8_t i = 1; i < queue->size; i++)
  {
    queue->elements[i - 1].priority = queue->elements[i].priority;
    queue->elements[i - 1].data = queue->elements[i].data;
  }
  queue->size--;

  xSemaphoreGive(queue->mtx);
  return pdTRUE;
}
