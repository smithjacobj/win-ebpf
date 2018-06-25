#pragma once

#include "main.h"

namespace nfq {
    
// https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/sddl-for-device-objects
const PCUNICODE_STRING DEFAULT_SDDL = SDDL_DEVOBJ_SYS_ALL_ADM_ALL;

// root_device_register registers a virtual device to receive IRPs
_IRQL_requires_max_(PASSIVE_LEVEL)
NDIS_STATUS root_device_register();


}; // namespace nfq