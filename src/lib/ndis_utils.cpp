// Windows XDP Packet Filter
// Copyright (C) 2018  Jacob Masen-Smith

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "ndis_utils.h"

// copy_single_net_buffer_list
template <tx_rx direction>
static PNET_BUFFER_LIST copy_single_net_buffer_list(FilterInstanceContext *context,
                                                    PNET_BUFFER_LIST src)
{
    PNET_BUFFER_LIST dst = NULL;

    // allocate NBL from pool (set to pre-allocate a NET_BUFFER and MDL)
    dst = NdisAllocateNetBufferList(context->net_buffer_list_pool, 0, 0);
    if (dst == NULL)
    {
        wnx_error_print("failed to allocate destination NET_BUFFER_LIST");
        goto fail;
    }

    // copy NBL metadata
    static_assert(direction == TX || direction == RX, "send or receive path must be defined");
    if (direction == TX)
    {
        NdisCopySendNetBufferListInfo(dst, src);
    }
    else if (direction == RX)
    {
        NdisCopyReceiveNetBufferListInfo(dst, src);
    }

    // ensure a valid SourceHandle
    dst->SourceHandle = context->ndis_filter_handle;

    // copy buffer data
    ULONG total_bytes = NET_BUFFER_DATA_LENGTH(NET_BUFFER_LIST_FIRST_NB(src));
    ULONG bytes_copied = 0;
    NTSTATUS status = NdisCopyFromNetBufferToNetBuffer(NET_BUFFER_LIST_FIRST_NB(dst),
                                                       0,
                                                       total_bytes,
                                                       NET_BUFFER_LIST_FIRST_NB(src),
                                                       0,
                                                       &bytes_copied);
    if (!NT_SUCCESS(status))
    {
        wnx_error_print("failed to copy NET_BUFFER data, status %d", status);
        goto fail;
    }
    // unlikely, but should be available as a warning.
    if (total_bytes != bytes_copied)
    {
        wnx_warn_print("expected to copy %d bytes, instead copied %d", total_bytes, bytes_copied);
    }

    return dst;

fail:
    NdisFreeNetBufferList(dst);

    return null;
}

// copy_net_buffer_lists
PNET_BUFFER_LIST copy_net_buffer_lists(FilterInstanceContext *context, PNET_BUFFER_LIST src)
{
    PNET_BUFFER_LIST head = NULL, *tail = &head;

    for (PNET_BUFFER_LIST *current_src_ptr = &src; *current_src_ptr != NULL;
         current_src_ptr = &NET_BUFFER_LIST_NEXT_NBL(*current_src_ptr))
    {
        *tail = copy_single_net_buffer_list(context, *current_src_ptr);
        if (*tail == NULL)
        {
            wnx_error_print("failed to copy NET_BUFFER_LIST");
            return NULL;
        }

        *tail = &NET_BUFFER_LIST_NEXT_NBL(*tail);
    }

    return head;
}
