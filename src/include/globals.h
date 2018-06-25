#pragma once

#include "main.h"

namespace nfq {

extern PDRIVER_OBJECT global_driver_object;
extern DRIVER_CONTEXT global_driver_context;
extern NDIS_HANDLE global_driver_handle;

extern PDEVICE_OBJECT global_root_device_object;
extern NDIS_HANDLE global_root_device_handle;

}; // namespace nfq