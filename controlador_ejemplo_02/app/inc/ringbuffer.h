/*
 * eringbuffer.h
 *
 *  Created on: Feb 15, 2023
 *      Author: seb
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef struct
{
    uint8_t *buffer;
    size_t size;
    size_t len;
    size_t w;
    size_t r;
} ringbuffer_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void ringbuffer_init(ringbuffer_t *rb, uint8_t *buffer, size_t size);

size_t ringbuffer_size(const ringbuffer_t *rb);

size_t ringbuffer_len(const ringbuffer_t *rb);

size_t ringbuffer_free(const ringbuffer_t *rb);

bool ringbuffer_is_full(const ringbuffer_t *rb);

bool ringbuffer_is_empty(const ringbuffer_t *rb);

size_t ringbuffer_write_byte(ringbuffer_t *rb, uint8_t byte);

size_t ringbuffer_write(ringbuffer_t *rb, const uint8_t *buffer, size_t size);

size_t ringbuffer_read_byte(ringbuffer_t *rb, uint8_t *byte);

size_t ringbuffer_read(ringbuffer_t *rb, uint8_t *buffer, size_t size);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* APP_H_ */

/********************** end of file ******************************************/

