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

#pragma once

#include "main.h"

// NET_BUFFER_LIST manipulation

// remove_nbl removes a NET_BUFFER_LIST from the linked list by taking the pointer to the Next field
// that points to the NET_BUFFER_LIST to be removed.
PNET_BUFFER_LIST remove_nbl(PNET_BUFFER_LIST *remove_at);

// copy_net_buffer_lists copies a net_buffer_list chain into a new chain in linear buffers and
// returns the new net_buffer_list chain.
PNET_BUFFER_LIST copy_net_buffer_lists(FilterInstanceContext *context, PNET_BUFFER_LIST src);