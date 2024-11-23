#ifndef INC_PQUEUE_H_
#define INC_PQUEUE_H_

#include <stdbool.h>

typedef enum
{
  LOW,
  MEDIUM,
  HIGH
} priority_t;

typedef void * data_t;
typedef struct priority_queue_s * priority_queue_t;

priority_queue_t init_queue();
bool enqueue(priority_queue_t queue, priority_t priority, data_t data);
bool dequeue(priority_queue_t queue, data_t * data);

#endif /* INC_PQUEUE_H_ */
