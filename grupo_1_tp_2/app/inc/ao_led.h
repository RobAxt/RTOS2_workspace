#ifndef INC_AO_LED_H_
#define INC_AO_LED_H_

#include "ao.h"

#define AO_LED_MAX_INSTANCES 3

typedef enum
{
  AO_LED_MESSAGE_RED_ON,
  AO_LED_MESSAGE_RED_OFF,
  AO_LED_MESSAGE_RED_BLINK,
  AO_LED_MESSAGE_GREEN_ON,
  AO_LED_MESSAGE_GREEN_OFF,
  AO_LED_MESSAGE_GREEN_BLINK,
  AO_LED_MESSAGE_BLUE_ON,
  AO_LED_MESSAGE_BLUE_OFF,
  AO_LED_MESSAGE_BLUE_BLINK,
  AO_LED_MESSAGE__N
} ao_led_event_t;


ao_t ao_led_init();
bool ao_led_send(ao_t ao, ao_led_event_t event);


#endif /* INC_AO_LED_H_ */
