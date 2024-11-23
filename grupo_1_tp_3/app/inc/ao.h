#ifndef INC_AO_H_
#define INC_AO_H_

typedef void* ao_event_t;
typedef void (*ao_event_handler_t)(ao_event_t);
typedef struct ao_s * ao_t;

ao_t ao_init(ao_event_handler_t event_handler);
bool ao_send(ao_t ao, ao_event_t event);

#endif /* INC_AO_H_ */
