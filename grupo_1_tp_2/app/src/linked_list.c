/*
 * Copyright (c) 2024 Sebastian Bedin <sebabedin@gmail.com>.
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
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "linked_list.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void linked_list_init(linked_list_t* hlist)
{
  hlist->pfirst_node = NULL;
  hlist->plast_node = NULL;
  hlist->len = 0;
}

void linked_list_node_init(linked_list_node_t* hnode, void* pdata)
{
  hnode->pdata = pdata;
  hnode->pnext_node = NULL;
}

linked_list_node_t* linked_list_node_remove(linked_list_t* hlist)
{
  linked_list_node_t* hnode;
  if(0 == hlist->len)
  {
    hnode = NULL;
  }
  else if(1 == hlist->len)
  {
    hnode = hlist->pfirst_node;
    hlist->pfirst_node = NULL;
    hlist->plast_node = NULL;
    hnode->pnext_node = NULL;
    hlist->len--;
  }
  else
  {
    hnode = hlist->pfirst_node;
    hlist->pfirst_node = hnode->pnext_node;
    hnode->pnext_node = NULL;
    hlist->len--;
  }
  return hnode;
}

void linked_list_node_add(linked_list_t* hlist, linked_list_node_t* hnode)
{
  if(0 == hlist->len)
  {
    hlist->pfirst_node = hnode;
    hlist->plast_node = hnode;
  }
  else
  {
    hlist->plast_node->pnext_node = hnode;
    hlist->plast_node = hnode;
  }
  hlist->len++;
}

/********************** end of file ******************************************/
