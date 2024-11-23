#ifndef INC_PAO_LED_H_
#define INC_PAO_LED_H_

#include <stdbool.h>
#include "pao.h"

typedef enum
{
  AO_LED_MESSAGE_RED_BLINK,
  AO_LED_MESSAGE_GREEN_BLINK,
  AO_LED_MESSAGE_BLUE_BLINK,
} pao_led_event_t;

pao_t pao_led_init();
bool pao_led_send(pao_t pao, pao_led_event_t event, priority_t priority);

#endif /* INC_PAO_LED_H_ */
