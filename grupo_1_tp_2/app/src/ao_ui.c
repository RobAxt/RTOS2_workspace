#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include <ao_led.h>
#include <ao_ui.h>

/********************** internal functions declaration ************************/
static void event_handler(ao_event_t event);

/*************************** internal data definition *****************************/
static ao_t led_red, led_green, led_blue;

/********************** external functions definition *****************************/

ao_t ao_ui_init()
{
  ao_t ao_ui_h = ao_init(event_handler);

  led_red = ao_led_init();
  led_green = ao_led_init();
  led_blue = ao_led_init();

  return ao_ui_h;
}

bool ao_ui_send(ao_t ao, ao_ui_event_t event)
{
  return ao_send(ao, (ao_event_t)event);
}

/********************** internal functions definition ************************/

static void event_handler(ao_event_t event)
{
  ao_ui_event_t event_ = (ao_ui_event_t) event;
  switch (event_)
  {
    case MSG_EVENT_BUTTON_PULSE:
      ao_led_send(led_red, AO_LED_MESSAGE_RED_ON);
      break;
    case MSG_EVENT_BUTTON_SHORT:
      ao_led_send(led_green, AO_LED_MESSAGE_GREEN_ON);
      break;
    case MSG_EVENT_BUTTON_LONG:
      ao_led_send(led_blue, AO_LED_MESSAGE_BLUE_ON);
      break;
    default:
  }
}
