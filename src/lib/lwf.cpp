#include "lwf.h"

namespace nfq {

// init functions can be removed from memory once they have completed.
#pragma NDIS_INIT_FUNCTION(driver_entry)

// driver_entry
_Use_decl_annotations_
NTSTATUS
driver_entry(
    struct _DRIVER_OBJECT *driver_object,
    PUNICODE_STRING registry_path)
{
    UNREFERENCED_PARAMETER(registry_path);

    NDIS_STATUS status = NDIS_STATUS_SUCCESS;

    global_driver_object = driver_object;
    driver_object->DriverUnload = driver_unload;

    NDIS_FILTER_DRIVER_CHARACTERISTICS fchars = {0};
    
    fchars.Header.Type = NDIS_OBJECT_TYPE_FILTER_DRIVER_CHARACTERISTICS;
    fchars.Header.Revision = NDIS_FILTER_CHARACTERISTICS_REVISION_1;
    fchars.Header.Size = NDIS_SIZEOF_FILTER_DRIVER_CHARACTERISTICS_REVISION_1;

    fchars.MajorNdisVersion = NDIS_FILTER_MAJOR_VERSION;
    fchars.MinorNdisVersion = NDIS_FILTER_MINOR_VERSION;

    fchars.MajorDriverVersion = DRIVER_MAJOR_VERSION;
    fchars.MinorDriverVersion = DRIVER_MINOR_VERSION;

    fchars.FriendlyName = RTL_CONSTANT_STRING(DRIVER_NAME);
    fchars.ServiceName = RTL_CONSTANT_STRING(DRIVER_NAME);
    fchars.UniqueName = RTL_CONSTANT_STRING(DRIVER_GUID);

    fchars.SetOptionsHandler = filter_set_options;
    fchars.SetFilterModuleOptionsHandler = filter_set_module_options;
    fchars.AttachHandler = filter_attach;
    fchars.DetachHandler = filter_detach;
    fchars.RestartHandler = filter_restart;
    fchars.PauseHandler = filter_pause;
    fchars.StatusHandler = filter_status;

    // per-module callbacks are registered with options so that they can be unregistered when the
    // module should be inactive.

    // register the driver properties
    status = NdisFRegisterFilterDriver(
        driver_object,
        NDIS_HANDLE(global_driver_context),
        &fchars,
        &global_driver_handle);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    // TODO: register a device to receive IRPs
}

// driver_unload
_Use_decl_annotations_
void
driver_unload(
    struct _DRIVER_OBJECT *driver_object)
{

}

// filter_attach
_Use_decl_annotations_
NDIS_STATUS
filter_attach(
    NDIS_HANDLE ndis_filter_handle,
    NDIS_HANDLE filter_driver_context,
    PNDIS_FILTER_ATTACH_PARAMETERS attach_parameters)
{

}

// filter_detach
_Use_decl_annotations_
VOID
filter_detach(
    NDIS_HANDLE filter_module_context)
{

}

// filter_restart
_Use_decl_annotations_
NDIS_STATUS
filter_restart(
    NDIS_HANDLE filter_module_context,
    PNDIS_FILTER_RESTART_PARAMETERS restart_parameters)
{

}

// filter_pause
_Use_decl_annotations_
NDIS_STATUS
filter_pause(
    NDIS_HANDLE filter_module_context,
    PNDIS_FILTER_PAUSE_PARAMETERS pause_parameters)
{
    
}

// filter_set_options
_Use_decl_annotations_
NDIS_STATUS
filter_set_options(
    NDIS_HANDLE ndis_driver_handle,
    NDIS_HANDLE driver_context)
{

}

// filter_set_module_options
_Use_decl_annotations_
NDIS_STATUS
filter_set_module_options(
    NDIS_HANDLE filter_module_context)
{
    
}

// filter_status
_Use_decl_annotations_
VOID
filter_status(
    NDIS_HANDLE filter_module_context,
    PNDIS_STATUS_INDICATION status_indication)
{

}

// filter_cancel_send_net_buffer_lists
_Use_decl_annotations_
VOID
filter_cancel_send_net_buffer_lists (
    NDIS_HANDLE filter_module_context,
    PNET_BUFFER_LIST net_buffer_lists,
    ULONG return_flags)
{
    
}
// filter_send_net_buffer_lists
_Use_decl_annotations_
VOID
filter_send_net_buffer_lists(
    NDIS_HANDLE filter_module_context,
    PNET_BUFFER_LIST net_buffer_list,
    NDIS_PORT_NUMBER port_number,
    ULONG send_flags)
{

}

// filter_send_net_buffer_lists_complete
_Use_decl_annotations_
VOID
filter_send_net_buffer_lists_complete(
    NDIS_HANDLE filter_module_context,
    PNET_BUFFER_LIST net_buffer_lists,
    ULONG send_complete_flags)
{

}

// filter_receive_net_buffer_lists
_Use_decl_annotations_
VOID
filter_receive_net_buffer_lists(
    NDIS_HANDLE filter_mdoule_context,
    PNET_BUFFER_LIST net_buffer_lists,
    NDIS_PORT_NUMBER port_number,
    ULONG number_of_net_buffer_lists,
    ULONG receive_flags)
{

}

// filter_return_net_buffer_lists
_Use_decl_annotations_
VOID
filter_return_net_buffer_lists(
    NDIS_HANDLE filter_module_context,
    PNET_BUFFER_LIST net_buffer_lists,
    ULONG return_flags)
{

}

}; // namespace nfq