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

#include <array>

#include "device.h"
#include "globals.h"
#include "main.h"

namespace win_xdp {

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

    dev_attribs.ExtensionSize = sizeof(RootDeviceExtension);

    dev_attribs.DefaultSDDLString = DEFAULT_SDDL;

    // Register device
    status = NdisRegisterDeviceEx(global_driver_handle, &dev_attribs, &global_root_device_object,
                                  &global_root_device_handle);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    RootDeviceExtension *device_extension = static_cast<RootDeviceExtension *>(
        NdisGetDeviceReservedExtension(global_root_device_object));

    // TODO: set extension fields here

    return status;
}

// root_device_manage
_Use_decl_annotations_ static NTSTATUS root_device_manage(PDEVICE_OBJECT dev_obj, PIRP irp) {}

// root_device_ioctl
_Use_decl_annotations_ static NTSTATUS root_device_ioctl(PDEVICE_OBJECT dev_obj, PIRP irp) {}

} // namespace win_xdp