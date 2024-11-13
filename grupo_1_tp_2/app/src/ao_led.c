#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "ao.h"
#include "ao_led.h"

/********************** internal data definition *****************************/
static GPIO_TypeDef* led_port_[] = {LED_RED_PORT, LED_GREEN_PORT,  LED_BLUE_PORT};
static uint16_t led_pin_[] = {LED_RED_PIN,  LED_GREEN_PIN, LED_BLUE_PIN };

typedef enum
{
  AO_LED_COLOR_RED,
  AO_LED_COLOR_GREEN,
  AO_LED_COLOR_BLUE,
} ao_led_color;

/********************** internal functions declaration ************************/
static void event_handler(ao_msg_t event);
static void postevent_handler(ao_msg_t event);
/********************** external functions definition *****************************/

ao_t ao_led_init()
{
  ao_t ao_led_h = ao_init(event_handler, postevent_handler);
  return ao_led_h;
}

bool ao_led_send(ao_t ao, ao_led_msg_t event)
{
  ao_led_msg_t* current_msg = (ao_led_msg_t*)pvPortMalloc(sizeof(ao_led_msg_t));
  if(NULL != current_msg)
  {
    *current_msg = event;
    if(pdFALSE == ao_send(ao, (ao_msg_t)current_msg))
    {
       vPortFree((void*)current_msg);
       return pdFALSE;
    }
    return pdTRUE;
  }
  return pdFALSE;
}

/********************** internal functions definition ************************/

static void event_handler(ao_msg_t event)
{
  ao_led_msg_t* event_ = (ao_led_msg_t*) event;
  switch (*event_)
  {
    case AO_LED_MESSAGE_RED_ON:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_RED], led_pin_[AO_LED_COLOR_RED], GPIO_PIN_SET);
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_GREEN], led_pin_[AO_LED_COLOR_GREEN], GPIO_PIN_RESET);
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_BLUE], led_pin_[AO_LED_COLOR_BLUE], GPIO_PIN_RESET);
      break;
    case AO_LED_MESSAGE_RED_OFF:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_RED], led_pin_[AO_LED_COLOR_RED], GPIO_PIN_RESET);
      break;
    case AO_LED_MESSAGE_GREEN_ON:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_RED], led_pin_[AO_LED_COLOR_RED], GPIO_PIN_RESET);
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_GREEN], led_pin_[AO_LED_COLOR_GREEN], GPIO_PIN_SET);
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_BLUE], led_pin_[AO_LED_COLOR_BLUE], GPIO_PIN_RESET);
      break;
    case AO_LED_MESSAGE_GREEN_OFF:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_GREEN], led_pin_[AO_LED_COLOR_GREEN], GPIO_PIN_RESET);
      break;
    case AO_LED_MESSAGE_BLUE_ON:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_RED], led_pin_[AO_LED_COLOR_RED], GPIO_PIN_RESET);
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_GREEN], led_pin_[AO_LED_COLOR_GREEN], GPIO_PIN_RESET);
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_BLUE], led_pin_[AO_LED_COLOR_BLUE], GPIO_PIN_SET);
      break;
    case AO_LED_MESSAGE_BLUE_OFF:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_BLUE], led_pin_[AO_LED_COLOR_BLUE], GPIO_PIN_RESET);
      break;
    default:
  }
}

void postevent_handler(ao_msg_t event)
{
  vPortFree((void*)event);
}
