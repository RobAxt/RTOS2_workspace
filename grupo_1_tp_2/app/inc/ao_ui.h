#ifndef INC_AO_UI_H_
#define INC_AO_UI_H_

typedef enum
{
  MSG_EVENT_BUTTON_PULSE,
  MSG_EVENT_BUTTON_SHORT,
  MSG_EVENT_BUTTON_LONG,
  MSG_EVENT__N,
} ao_ui_msg_t;

/********************** external functions declaration ***********************/

ao_t ao_ui_init();
bool ao_ui_send(ao_t ao, ao_ui_msg_t event);


#endif /* INC_AO_UI_H_ */
