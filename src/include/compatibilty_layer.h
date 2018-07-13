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
run_xdp_return run_xdp(FILTER_INSTANCE_CONTEXT *context, PNET_BUFFER_LIST net_buffer_lists);

}; // namespace win_xdp