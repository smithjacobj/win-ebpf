#pragma once

#include "main.h"

// NET_BUFFER_LIST manipulation

// remove_nbl removes a NET_BUFFER_LIST from the linked list by taking the pointer to the Next field
// that points to the NET_BUFFER_LIST to be removed.
PNET_BUFFER_LIST remove_nbl(PNET_BUFFER_LIST *remove_at);

// insert_nbl inserts a NET_BUFFER_LIST from the linked list by taking the pointer to the Next field
// it should be inserted into
//
// This function only inserts single NET_BUFFER_LISTS. It will corrupt NET_BUFFER_LIST chains.
void insert_nbl(PNET_BUFFER_LIST *insert_at, PNET_BUFFER_LIST to_insert)

// coallate_nbl copies NBL data into a contiguous buffer and returns a pointer to that buffer (which
// must subsequently be freed)
//
// The returned value is the size of the copied data, in bytes.
size_t coallate_nbl(void *dst, PNET_BUFFER_LIST src);