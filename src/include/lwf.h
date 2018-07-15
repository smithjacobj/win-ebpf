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

namespace win_xdp {

// https://msdn.microsoft.com/en-us/library/windows/hardware/ff544113(v=vs.85).aspx
DRIVER_INITIALIZE driver_entry;

// https://msdn.microsoft.com/en-us/library/windows/hardware/ff564886(v=vs.85).aspx
DRIVER_UNLOAD driver_unload;

// NDIS calls a filter driver's FilterAttach function to allocate and initialize a filter module's
// data structures.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549905(v=vs.85).aspx
FILTER_ATTACH filter_attach;

// NDIS calls a filter driver's FilterDetach function to release all the resources that are
// associated with a filter module.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549918(v=vs.85).aspx
FILTER_DETACH filter_detach;

// The FilterRestart function initiates a restart operation for the specified filter module.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549962(v=vs.85).aspx
FILTER_RESTART filter_restart;

// NDIS calls a filter driver's FilterPause function to initiate a pause operation for the specified
// filter module.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549957(v=vs.85).aspx
FILTER_PAUSE filter_pause;

// NDIS calls a driver's XxxSetOptions function to allow the driver to register optional services.
// This function is defined as MiniportSetOptions for miniport drivers, ProtocolSetOptions for
// protocol drivers, or FilterSetOptions for filter drivers
// https://msdn.microsoft.com/library/windows/hardware/ff570269
FILTER_SET_OPTIONS filter_set_options;

// If FilterSetModuleOptions is defined, NDIS calls FilterSetModuleOptions before it calls the
// FilterRestart function to start the filter module.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549970(v=vs.85).aspx
FILTER_SET_MODULE_OPTIONS filter_set_module_options;

// The FilterStatus function indicates status changes that are reported by NDIS or an underlying
// driver.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549973(v=vs.85).aspx
FILTER_STATUS filter_status;

// NDIS calls a filter driver's FilterCancelSendNetBufferLists function to cancel the transmission
// of all NET_BUFFER_LIST structures that are marked with a specified cancellation identifier.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549915(v=vs.85).aspx
FILTER_CANCEL_SEND_NET_BUFFER_LISTS filter_cancel_send_net_buffer_lists;

// NDIS calls the FilterSendNetBufferLists function to allow a filter driver to filter a linked list
// of NET_BUFFER_LIST structures.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549966(v=vs.85).aspx
FILTER_SEND_NET_BUFFER_LISTS filter_send_net_buffer_lists;

// NDIS calls the FilterSendNetBufferListsComplete function to complete a send request that a filter
// driver started by calling the NdisFSendNetBufferLists function.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549967(v=vs.85).aspx
FILTER_SEND_NET_BUFFER_LISTS_COMPLETE filter_send_net_buffer_lists_complete;

// NDIS calls the FilterReceiveNetBufferLists function to request a filter driver to process a
// receive indication.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549960(v=vs.85).aspx
FILTER_RECEIVE_NET_BUFFER_LISTS filter_receive_net_buffer_lists;

// NDIS calls the FilterReturnNetBufferLists function to return a linked list of NET_BUFFER_LIST
// structures and associated data to a filter driver.
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff549964(v=vs.85).aspx
FILTER_RETURN_NET_BUFFER_LISTS filter_return_net_buffer_lists;

}; // namespace win_xdp