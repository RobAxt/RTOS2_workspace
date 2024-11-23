#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "pao.h"
#include "pao_led.h"

/********************** internal data definition *****************************/
static GPIO_TypeDef* led_port_[] = {LED_RED_PORT, LED_GREEN_PORT,  LED_BLUE_PORT};
static uint16_t led_pin_[] = {LED_RED_PIN,  LED_GREEN_PIN, LED_BLUE_PIN };
static const int32_t BLINK_DELAY = 5000;

typedef enum
{
  AO_LED_COLOR_RED,
  AO_LED_COLOR_GREEN,
  AO_LED_COLOR_BLUE,
} pao_led_color;

/********************** internal functions declaration ************************/
static void event_handler(pao_event_t event);

/********************** external functions definition *****************************/

pao_t pao_led_init()
{
  pao_t pao_led_h = pao_init(event_handler);
  return pao_led_h;
}

bool pao_led_send(pao_t pao, pao_led_event_t event, priority_t priority)
{
  return pao_send(pao, (pao_event_t)event, priority);
}

/********************** internal functions definition ************************/

static void event_handler(pao_event_t event)
{
  pao_led_event_t event_ = (pao_led_event_t) event;
  switch (event_)
  {
    case AO_LED_MESSAGE_RED_BLINK:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_RED], led_pin_[AO_LED_COLOR_RED], GPIO_PIN_SET);
      vTaskDelay((TickType_t)((BLINK_DELAY) / portTICK_PERIOD_MS));
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_RED], led_pin_[AO_LED_COLOR_RED], GPIO_PIN_RESET);
      break;
    case AO_LED_MESSAGE_GREEN_BLINK:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_GREEN], led_pin_[AO_LED_COLOR_GREEN], GPIO_PIN_SET);
      vTaskDelay((TickType_t)((BLINK_DELAY) / portTICK_PERIOD_MS));
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_GREEN], led_pin_[AO_LED_COLOR_GREEN], GPIO_PIN_RESET);
      break;
    case AO_LED_MESSAGE_BLUE_BLINK:
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_BLUE], led_pin_[AO_LED_COLOR_BLUE], GPIO_PIN_SET);
      vTaskDelay((TickType_t)((BLINK_DELAY) / portTICK_PERIOD_MS));
      HAL_GPIO_WritePin(led_port_[AO_LED_COLOR_BLUE], led_pin_[AO_LED_COLOR_BLUE], GPIO_PIN_RESET);
      break;
    default:
  }
}
