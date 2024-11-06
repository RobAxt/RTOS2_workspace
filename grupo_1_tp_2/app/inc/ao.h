#ifndef INC_AO_H_
#define INC_AO_H_

#define AO_MAX_INSTANCES 4

typedef void* ao_msj_t;
typedef struct ao_event_s* ao_event_t;
typedef void (*ao_event_handler_t)(ao_event_t);
typedef struct ao_s * ao_t;

ao_t ao_init(ao_event_handler_t event_handler);
bool ao_send(ao_t ao, ao_msj_t eventMsj);

#endif /* INC_AO_H_ */