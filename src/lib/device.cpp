#include <array>

#include "device.h"
#include "globals.h"
#include "main.h"

namespace win_ebpf {

DRIVER_DISPATCH root_device_manage;
DRIVER_DISPATCH root_device_ioctl;

#pragma NDIS_INIT_FUNCTION(root_device_register)

// root_device_register
_Use_decl_annotations_ NDIS_STATUS root_device_register() {
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;

    NDIS_STRING dos_name = RTL_CONSTANT_STRING(DOS_NAME);
    NDIS_STRING nt_name = RTL_CONSTANT_STRING(NT_NAME);

    // IRP dispatch table
    std::array<PDRIVER_DISPATCH, IRP_MJ_MAXIMUM_FUNCTION + 1> dispatch_table = {};
    dispatch_table[IRP_MJ_CREATE] = root_device_manage;
    dispatch_table[IRP_MJ_CLEANUP] = root_device_manage;
    dispatch_table[IRP_MJ_CLOSE] = root_device_manage;
    dispatch_table[IRP_MJ_DEVICE_CONTROL] = root_device_ioctl;

    // Device properties
    NDIS_DEVICE_OBJECT_ATTRIBUTES dev_attribs = {0};

    dev_attribs.Header.Type = NDIS_OBJECT_TYPE_DEVICE_OBJECT_ATTRIBUTES;
    dev_attribs.Header.Revision = NDIS_DEVICE_OBJECT_ATTRIBUTES_REVISION_1;
    dev_attribs.Header.Size = NDIS_SIZEOF_DEVICE_OBJECT_ATTRIBUTES_REVISION_1;

    dev_attribs.DeviceName = &nt_name;
    dev_attribs.SymbolicName = &dos_name;

    dev_attribs.MajorFunctions = dispatch_table.data();

    dev_attribs.ExtensionSize = sizeof(ROOT_DEVICE_EXTENSION);

    dev_attribs.DefaultSDDLString = DEFAULT_SDDL;

    // Register device
    status = NdisRegisterDeviceEx(global_driver_handle, &dev_attribs, &global_root_device_object,
                                  &global_root_device_handle);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    ROOT_DEVICE_EXTENSION *device_extension = static_cast<ROOT_DEVICE_EXTENSION *>(
        NdisGetDeviceReservedExtension(global_root_device_object));

    // TODO: set extension fields here

    return status;
}

// root_device_manage
_Use_decl_annotations_ static NTSTATUS root_device_manage(PDEVICE_OBJECT dev_obj, PIRP irp) {}

// root_device_ioctl
_Use_decl_annotations_ static NTSTATUS root_device_ioctl(PDEVICE_OBJECT dev_obj, PIRP irp) {}

} // namespace win_ebpf