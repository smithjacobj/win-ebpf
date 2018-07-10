#pragma once

namespace win_xdp {

// FILTER_GLOBAL_CONTEXT is the driver-global context area for the entire driver.
struct FILTER_GLOBAL_CONTEXT {};

// FILTER_INSTANCE_CONTEXT is the module-local storage for each driver module instance.
struct FILTER_INSTANCE_CONTEXT {
    NDIS_HANDLE ndis_filter_handle;
    NDIS_HANDLE net_buffer_list_pool;
};

// ROOT_DEVICE_EXTENSION is the device-local storage for each device instance.
struct ROOT_DEVICE_EXTENSION {};

} // namespace win_xdp