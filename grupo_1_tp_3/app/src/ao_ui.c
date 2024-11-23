#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include <ao.h>
#include <ao_ui.h>
#include <pao.h>
#include <pao_led.h>

/********************** internal functions declaration ************************/
static void event_handler(ao_event_t event);

/*************************** internal data definition *****************************/
static pao_t led;

/********************** external functions definition *****************************/

ao_t ao_ui_init()
{
  ao_t ao_ui_h = ao_init(event_handler);

  led = pao_led_init();

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
      pao_led_send(led, AO_LED_MESSAGE_RED_BLINK, HIGH);
      break;
    case MSG_EVENT_BUTTON_SHORT:
      pao_led_send(led, AO_LED_MESSAGE_GREEN_BLINK, MEDIUM);
      break;
    case MSG_EVENT_BUTTON_LONG:
      pao_led_send(led, AO_LED_MESSAGE_BLUE_BLINK, LOW);
      break;
    default:
  }
}
