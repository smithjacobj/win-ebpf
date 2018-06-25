#include "device.h"

// root_device_register
_Use_decl_annotations_
NDIS_STATUS
root_device_register()
{
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;

    const PUNICODE_STRING dos_name = RTL_CONSTANT_STRING(DOS_NAME);
    const PUNICODE_STRING nt_name = RTL_CONSTANT_STRING(NT_NAME);

    NDIS_DEVICE_OBJECT_ATTRIBUTES dev_attribs = {0};

    dev_attribs.Header.Type = NDIS_OBJECT_TYPE_DEVICE_OBJECT_ATTRIBUTES;
    dev_attribs.Header.Revision = NDIS_DEVICE_OBJECT_ATTRIBUTES_REVISION_1;
    dev_attribs.Header.Size = NDIS_SIZEOF_DEVICE_OBJECT_ATTRIBUTES_REVISION_1;

    dev_attribs.DeviceName = nt_name;
    dev_attribs.SymbolicName = dos_name;

    dev_attribs.MajorFunctions = ; // TODO

    dev_attribs.ExtensionSize = ; // TODO

    dev_attribs.DefaultSDDLString = DEFAULT_SDDL;

    status = NdisRegisterDeviceEx(
        global_driver_handle,
        &dev_attribs,
        &global_root_device_object,
        &global_root_device_handle);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    PFILTER_DEVICE_EXTENSION device_extension = NdisGetDeviceReservedExtension(global_device_object);

    device_extension->Signature = DRIVER_SIGNATURE;
    device_extension->Handle = global_driver_handle;

    return status
}