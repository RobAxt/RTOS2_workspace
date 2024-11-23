/*
 * Copyright (c) YEAR NOMBRE <MAIL>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : driver_uart.c
 * @date   : Apr 1, 2023
 * @author : NOMBRE <MAIL>
 * @version v1.0.0
 */

/********************** inclusions *******************************************/

#include <ringbuffer.h>

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

inline static void inc_idex_(size_t *index, size_t size)
{
  *index += 1;
  if(size <= *index)
  {
    *index = 0;
  }
}

inline static void write_byte_(ringbuffer_t *rb, uint8_t byte)
{
  rb->buffer[rb->w] = byte;
  inc_idex_(&(rb->w), rb->size);
  rb->len++;
}

inline static void read_byte_(ringbuffer_t *rb, uint8_t *byte)
{
  *byte = rb->buffer[rb->r];
  inc_idex_(&(rb->r), rb->size);
  rb->len--;
}

/********************** external functions definition ************************/

void ringbuffer_init(ringbuffer_t *rb, uint8_t *buffer, size_t size)
{
  rb->buffer = buffer;
  rb->size = size;
  rb->len = 0;
  rb->w = 0;
  rb->r = 0;
}

size_t ringbuffer_size(const ringbuffer_t *rb)
{
  return rb->size;
}

size_t ringbuffer_len(const ringbuffer_t *rb)
{
  return rb->len;
}

size_t ringbuffer_free(const ringbuffer_t *rb)
{
  return rb->size - rb->len;
}

bool ringbuffer_is_full(const ringbuffer_t *rb)
{
  return (0 == ringbuffer_free(rb));
}

bool ringbuffer_is_empty(const ringbuffer_t *rb)
{
  return (0 == ringbuffer_len(rb));
}

size_t ringbuffer_write_byte(ringbuffer_t *rb, uint8_t byte)
{
  if(ringbuffer_is_full(rb))
  {
    return 0;
  }
  write_byte_(rb, byte);
  return 1;
}

size_t ringbuffer_write(ringbuffer_t *rb, const uint8_t *buffer, size_t size)
{
  size_t i;
  for(i = 0; i < size; ++i)
  {
    if(0 == ringbuffer_write_byte(rb, buffer[i]))
    {
      break;
    }
  }
  return i;
}

size_t ringbuffer_read_byte(ringbuffer_t *rb, uint8_t *byte)
{
  if(ringbuffer_is_empty(rb))
  {
    return 0;
  }
  read_byte_(rb, byte);
  return 1;
}

size_t ringbuffer_read(ringbuffer_t *rb, uint8_t *buffer, size_t size)
{
  size_t i;
  for(i = 0; i < size; ++i)
  {
    if(0 == ringbuffer_read_byte(rb, buffer + i))
    {
      break;
    }
  }
  return i;
}

/********************** end of file ******************************************/
