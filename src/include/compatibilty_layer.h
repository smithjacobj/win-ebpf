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

namespace win_xdp
{

// run_xdp_return is a structure of all return data from run_xdp
struct run_xdp_return
{
    PNET_BUFFER_LIST pass_net_buffer_list, tx_net_buffer_list, drop_net_buffer_list;

    ULONG pass_count, tx_count;
};

// run_xdp runs the chain of XDP eBFP programs for the given context and buffers.
run_xdp_return run_xdp(FilterInstanceContext *context, PNET_BUFFER_LIST net_buffer_lists);

}; // namespace win_xdp