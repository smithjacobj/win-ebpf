#pragma once

#include "main.h"

// NET_BUFFER_LIST manipulation

// remove_nbl removes a NET_BUFFER_LIST from the linked list by taking the pointer to the Next field
// that points to the NET_BUFFER_LIST to be removed.
PNET_BUFFER_LIST remove_nbl(PNET_BUFFER_LIST *remove_at);

// copy_net_buffer_lists copies a net_buffer_list chain into a new chain in linear buffers and
// returns the new net_buffer_list chain.
PNET_BUFFER_LIST copy_net_buffer_lists(FILTER_INSTANCE_CONTEXT *context, PNET_BUFFER_LIST src);