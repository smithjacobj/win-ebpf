#include "ndis_utils.h"

// remove_nbl
PNET_BUFFER_LIST remove_nbl(PNET_BUFFER_LIST *remove_at) {
    PNET_BUFFER_LIST removed = *remove_at;

    *remove_at = (*remove_at)->Next;

    return removed;
}

// coallate_nbl
size_t coallate_nbl(void *dst, PNET_BUFFER_LIST src) {
    PNET_BUFFER net_buffer = src->FirstNetBuffer;
    data_length = net_buffer->DataLength;

    data_length = min(DATA_BUFFER_SIZE, data_length);

    NdisGetDataBuffer(net_buffer, data_length, dst, 0, 0); // TODO: check alignment needs

    return data_length;
}
