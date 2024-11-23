#ifndef INC_PAO_H_
#define INC_PAO_H_

typedef void* pao_event_t;
typedef void (*pao_event_handler_t)(pao_event_t);
typedef struct pao_s * pao_t;

pao_t pao_init(pao_event_handler_t event_handler);
bool pao_send(pao_t pao, pao_event_t event);

#endif /* INC_AO_H_ */
