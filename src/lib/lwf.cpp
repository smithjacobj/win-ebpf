#include "lwf.h"
#include "globals.h"

namespace win_xdp {

constexpr ULONG DATA_BUFFER_SIZE = 0xFFFF;

// init functions can be removed from memory once they have completed.
#pragma NDIS_INIT_FUNCTION(driver_entry)

// driver_entry
_Use_decl_annotations_ NTSTATUS driver_entry(struct _DRIVER_OBJECT *driver_object,
                                             PUNICODE_STRING registry_path) {
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
    status = NdisFRegisterFilterDriver(driver_object, &global_FILTER_GLOBAL_CONTEXT, &fchars,
                                       &global_driver_handle);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    // TODO: register a device to receive IRPs
}

// driver_unload
_Use_decl_annotations_ void driver_unload(struct _DRIVER_OBJECT *driver_object) {
    // TODO: do any cleanup/disposal operations here
}

// filter_attach
_Use_decl_annotations_ NDIS_STATUS filter_attach(NDIS_HANDLE ndis_filter_handle,
                                                 NDIS_HANDLE filter_FILTER_GLOBAL_CONTEXT,
                                                 PNDIS_FILTER_ATTACH_PARAMETERS attach_parameters) {
    // TODO: allocate NBL pool
}

// filter_detach
_Use_decl_annotations_ VOID filter_detach(NDIS_HANDLE filter_instance_context_void) {
    // TODO: deallocate NBL pool
}

// filter_restart
_Use_decl_annotations_ NDIS_STATUS filter_restart(
    NDIS_HANDLE filter_instance_context_void, PNDIS_FILTER_RESTART_PARAMETERS restart_parameters) {}

// filter_pause
_Use_decl_annotations_ NDIS_STATUS filter_pause(NDIS_HANDLE filter_instance_context_void,
                                                PNDIS_FILTER_PAUSE_PARAMETERS pause_parameters) {}

// filter_set_options
_Use_decl_annotations_ NDIS_STATUS filter_set_options(NDIS_HANDLE ndis_driver_handle,
                                                      NDIS_HANDLE FILTER_GLOBAL_CONTEXT) {}

// filter_set_module_options
_Use_decl_annotations_ NDIS_STATUS
filter_set_module_options(NDIS_HANDLE filter_instance_context_void) {}

// filter_status
_Use_decl_annotations_ VOID filter_status(NDIS_HANDLE filter_instance_context_void,
                                          PNDIS_STATUS_INDICATION status_indication) {}

// send_copy copies the data in a NET_BUFFER_LIST and sends the copy.
static void send_copy(FILTER_INSTANCE_CONTEXT context, PNET_BUFFER_LIST net_buffer_list) {

}

// filter_cancel_send_net_buffer_lists
_Use_decl_annotations_ VOID
filter_cancel_send_net_buffer_lists(NDIS_HANDLE filter_instance_context_void, PVOID cancel_id) {
    // this function is required, but this driver does not queue NBLs for a "long" period of time
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;
    NdisFCancelSendNetBufferLists(context->ndis_filter_handle, cancel_id);
}

// filter_send_net_buffer_lists
_Use_decl_annotations_ VOID filter_send_net_buffer_lists(NDIS_HANDLE filter_instance_context_void,
                                                         PNET_BUFFER_LIST net_buffer_list,
                                                         NDIS_PORT_NUMBER port_number,
                                                         ULONG send_flags) {
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;
    PNET_BUFFER_LIST next;

    // allocate a contiguous buffer for packets
    void *data = NdisAllocateMemoryWithTagPriority(context->ndis_filter_handle, DATA_BUFFER_SIZE,
                                                   DRIVER_SIGNATURE, NormalPoolPriority);
    if (data == NULL) {
        // ERROR: alloc failure. default abort (like invalid xdp flags)
        wx_error_print("buffer allocation failed");
        NdisFSendNetBufferListsComplete(context->ndis_filter_handle, net_buffer_list);
        return;
    }

    for (PNET_BUFFER_LIST *current_nbl_ptr = &net_buffer_list; *current_nbl_ptr != NULL;
         current_nbl_ptr = &(*current_nbl_ptr)->Next) {

        // copy NBL data into a single contiguous buffer
        size_t data_length = coallate_nbl(data, net_buffer_list);
        u32 xdp_flag = run_xdp(data, data_length);

        // move dropped/aborted/error NBLs to the cancel list
        switch (xdp_flag) {
            case XDP_PASS:
                next = (*current_nbl_ptr)->Next;
                (*current_nbl_ptr)->Next = NULL;
                NdisFSendNetBufferLists(context->ndis_filter_handle, *current_nbl_ptr);
                (*current_nbl_ptr)->Next = next;
                break;

            case XDP_TX:
            case XDP_DROP:
            case XDP_ABORTED:
            default:
                next = (*current_nbl_ptr)->Next;
                (*current_nbl_ptr)->Next = NULL;
                NdisFSendNetBufferListsComplete(context->ndis_filter_handle, *current_nbl_ptr);
                break;
        }
    }

    // free the contiguous buffer
    NdisFreeMemoryWithTagPriority(filter_instance_context_void, data, DRIVER_SIGNATURE);
}

// filter_send_net_buffer_lists_complete
_Use_decl_annotations_ VOID
filter_send_net_buffer_lists_complete(NDIS_HANDLE filter_instance_context_void,
                                      PNET_BUFFER_LIST net_buffer_list, ULONG send_complete_flags) {
    // we don't allocate any new NBLs, so we simply pass along requests.
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;
    NdisFSendNetBufferListsComplete(context->ndis_filter_handle, net_buffer_list,
                                    send_complete_flags);
}

// filter_receive_net_buffer_lists
_Use_decl_annotations_ VOID filter_receive_net_buffer_lists(NDIS_HANDLE filter_mdoule_context,
                                                            PNET_BUFFER_LIST net_buffer_list,
                                                            NDIS_PORT_NUMBER port_number,
                                                            ULONG number_of_net_buffer_lists,
                                                            ULONG receive_flags) {
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;
    PNET_BUFFER_LIST next;

    // allocate a contiguous buffer for packets
    void *data = NdisAllocateMemoryWithTagPriority(context->ndis_filter_handle, DATA_BUFFER_SIZE,
                                                   DRIVER_SIGNATURE, NormalPoolPriority);
    if (data == NULL) {
        // ERROR: alloc failure. default abort (like invalid xdp flags)
        wx_error_print("buffer allocation failed");
        NdisFSendNetBufferListsComplete(context->ndis_filter_handle, net_buffer_list);
        return;
    }

    for (PNET_BUFFER_LIST *current_nbl_ptr = &net_buffer_list; *current_nbl_ptr != NULL;
         current_nbl_ptr = &(*current_nbl_ptr)->Next) {

        // copy NBL data into a single contiguous buffer
        size_t data_length = coallate_nbl(data, net_buffer_list);
        u32 xdp_flag = run_xdp(data, data_length);

        // move dropped/aborted/error NBLs to the cancel list
        switch (xdp_flag) {
            case XDP_PASS:
                next = (*current_nbl_ptr)->Next;
                (*current_nbl_ptr)->Next = NULL;
                NdisFIndicateReceiveNetBufferLists(context->ndis_filter_handle, *current_nbl_ptr,
                                                   port_number, 1, receive_flags);
                break;
                (*current_nbl_ptr)->Next = next;

            case XDP_TX:
                // copy/allocate new NBL and cancel the old one
                next = (*current_nbl_ptr)->Next;
                (*current_nbl_ptr)->Next = NULL;
                send_copy(context, *current_nbl_ptr);
                NdisFReturnNetBufferLists(context->ndis_filter_handle, *current_nbl_ptr, 0);
                (*current_nbl_ptr)->Next = next;
                break;

            case XDP_DROP:
            case XDP_ABORTED:
            default:
                next = (*current_nbl_ptr)->Next;
                (*current_nbl_ptr)->Next = NULL;
                NdisFReturnNetBufferLists(context->ndis_filter_handle, *current_nbl_ptr, 0);
                (*current_nbl_ptr)->Next = next;
                break;
        }
    }

    // free the contiguous buffer
    NdisFreeMemoryWithTagPriority(context->ndis_filter_handle, data, DRIVER_SIGNATURE);
}

// filter_return_net_buffer_lists
_Use_decl_annotations_ VOID filter_return_net_buffer_lists(NDIS_HANDLE filter_instance_context_void,
                                                           PNET_BUFFER_LIST net_buffer_list,
                                                           ULONG return_flags) {}

}; // namespace win_xdp