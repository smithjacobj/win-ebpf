#pragma once

namespace win_ebpf {

// DRIVER_CONTEXT is the driver-global context area for the entire driver.
struct DRIVER_CONTEXT {};

// DRIVER_MODULE is the module-local storage for each driver module instance.
struct DRIVER_MODULE {};

// DEVICE_EXTENSION is the device-local storage for each device instance.
struct DEVICE_EXTENSION {};

} // namespace win_ebpf