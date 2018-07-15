#include "lwf.h"
#include "globals.h"

namespace win_xdp
{

constexpr ULONG DATA_BUFFER_SIZE = 0xFFFF;

// init functions can be removed from memory once they have completed.
#pragma NDIS_INIT_FUNCTION(driver_entry)

// driver_entry
_Use_decl_annotations_ NTSTATUS driver_entry(struct _DRIVER_OBJECT *driver_object,
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
        driver_object, &global_FILTER_GLOBAL_CONTEXT, &fchars, &global_driver_handle);
    if (!NT_SUCCESS(status))
    {
        return status;
    }

    // TODO: register a device to receive IRPs
}

// driver_unload
_Use_decl_annotations_ void driver_unload(struct _DRIVER_OBJECT *driver_object)
{
    // TODO: do any cleanup/disposal operations here
}

// filter_attach
_Use_decl_annotations_ NDIS_STATUS filter_attach(NDIS_HANDLE ndis_filter_handle,
                                                 NDIS_HANDLE filter_FILTER_GLOBAL_CONTEXT,
                                                 PNDIS_FILTER_ATTACH_PARAMETERS attach_parameters)
{
    NET_BUFFER_LIST_POOL_PARAMETERS params = {};

    params.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
    params.Header.Version = NET_BUFFER_LIST_POOL_PARAMETERS_REVISION_1;
    params.Header.Size = NDIS_SIZEOF_NET_BUFFER_LIST_POOL_PARAMETERS_REVISION_1;

    params.ProtocolId = 0;

    params.fAllocateNetBuffer = TRUE;

    params.ContextSize = 0;

    params.PoolTag = DRIVER_SIGNATURE;

    params.DataSize = static_cast<ULONG>(MAX_PACKET_SIZE);

    NdisAllocateNetBufferListPool(,&params);
    // TODO: allocate NBL pool
}

// filter_detach
_Use_decl_annotations_ VOID filter_detach(NDIS_HANDLE filter_instance_context_void)
{
    // TODO: deallocate NBL pool
}

// filter_restart
_Use_decl_annotations_ NDIS_STATUS filter_restart(
    NDIS_HANDLE filter_instance_context_void, PNDIS_FILTER_RESTART_PARAMETERS restart_parameters)
{
}

// filter_pause
_Use_decl_annotations_ NDIS_STATUS filter_pause(NDIS_HANDLE filter_instance_context_void,
                                                PNDIS_FILTER_PAUSE_PARAMETERS pause_parameters)
{
}

// filter_set_options
_Use_decl_annotations_ NDIS_STATUS filter_set_options(NDIS_HANDLE ndis_driver_handle,
                                                      NDIS_HANDLE FILTER_GLOBAL_CONTEXT)
{
}

// filter_set_module_options
_Use_decl_annotations_ NDIS_STATUS
filter_set_module_options(NDIS_HANDLE filter_instance_context_void)
{
}

// filter_status
_Use_decl_annotations_ VOID filter_status(NDIS_HANDLE filter_instance_context_void,
                                          PNDIS_STATUS_INDICATION status_indication)
{
}

// filter_cancel_send_net_buffer_lists
_Use_decl_annotations_ VOID
filter_cancel_send_net_buffer_lists(NDIS_HANDLE filter_instance_context_void, PVOID cancel_id)
{
    // this function is required, but this driver does not queue NBLs for a "long" period of time
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;
    NdisFCancelSendNetBufferLists(context->ndis_filter_handle, cancel_id);
}

// filter_send_net_buffer_lists
_Use_decl_annotations_ VOID filter_send_net_buffer_lists(NDIS_HANDLE filter_instance_context_void,
                                                         PNET_BUFFER_LIST net_buffer_lists,
                                                         NDIS_PORT_NUMBER port_number,
                                                         ULONG send_flags)
{
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;

    // copy the NBLs, because they need to be contiguous for XDP to be able to modify them as a
    // linear buffer.
    PNET_BUFFER_LIST net_buffer_lists_copy = copy_net_buffer_lists<TX>(context, net_buffer_lists);

    // return the old NBLs. We won't be using them.
    NdisFSendNetBufferListsComplete(context->ndis_filter_handle, net_buffer_lists);

    // run our xdp chain
    auto xrv = run_xdp(context, net_buffer_lists_copy);

    // handle our xdp results
    if (xrv.pass_net_buffer_list != NULL)
    {
        NdisFSendNetBufferLists(
            context->ndis_filter_handle, xrv.pass_net_buffer_list, port_number, send_flags);
    }
    // dropped NBLs are simply freed since we allocated them.
    while (xrv.drop_net_buffer_lists != NULL)
    {
        PNET_BUFFER_LIST popped = xrv.drop_net_buffer_lists;
        xrv.drop_net_buffer_lists = NET_BUFFER_LIST_NEXT_NBL(xrv.drop_net_buffer_lists);
        NdisFreeNetBufferList(popped);
    }
    // XDP_TX is invalid on the send path; drop as above.
    while (xrv.tx_net_buffer_lists != NULL)
    {
        PNET_BUFFER_LIST popped = xrv.tx_net_buffer_lists;
        xrv.tx_net_buffer_lists = NET_BUFFER_LIST_NEXT_NBL(xrv.tx_net_buffer_lists);
        NdisFreeNetBufferList(popped);
    }
}

// filter_send_net_buffer_lists_complete
_Use_decl_annotations_ VOID
filter_send_net_buffer_lists_complete(NDIS_HANDLE filter_instance_context_void,
                                      PNET_BUFFER_LIST net_buffer_lists,
                                      ULONG send_complete_flags)
{
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;

    // look for NBLs we allocated
    for (PNET_BUFFER_LIST *current_nbl_ptr = &net_buffer_lists; *current_nbl_ptr != NULL;
         current_nbl_ptr = &NET_BUFFER_LIST_NEXT_NBL(*current_nbl_ptr))
    {
        // if we allocated this NBL (likely any we receive on this path), free it
        if ((*current_nbl_ptr)->SourceHandle == context->ndis_filter_handle)
        {
            // remove the NBL
            PNET_BUFFER_LIST popped = *current_nbl_ptr;
            current_nbl_ptr = &NET_BUFFER_LIST_NEXT_NBL(popped);
            // free it
            NdisFreeNetBufferList(popped);
        }
    }

    // any remaining NBLs (unlikely) belong to another driver.
    if (net_buffer_lists != NULL)
    {
        NdisFSendNetBufferListsComplete(
            context->ndis_filter_handle, net_buffer_lists, send_complete_flags);
    }
}

// filter_receive_net_buffer_lists
_Use_decl_annotations_ VOID filter_receive_net_buffer_lists(NDIS_HANDLE filter_mdoule_context,
                                                            PNET_BUFFER_LIST net_buffer_lists,
                                                            NDIS_PORT_NUMBER port_number,
                                                            ULONG number_of_net_buffer_lists,
                                                            ULONG receive_flags)
{
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;

    // copy the NBLs, because they need to be contiguous for XDP to be able to modify them as a
    // linear buffer.
    PNET_BUFFER_LIST net_buffer_lists_copy = copy_net_buffer_lists<RX>(context, net_buffer_lists);

    // return the old NBLs. We won't be using them.
    ULONG return_flags = 0;
    if (NDIS_TEST_RECEIVE_AT_DISPATCH_LEVEL(receive_flags))
    {
        NDIS_SET_RETURN_FLAG(return_flags, NDIS_RETURN_FLAGS_DISPATCH_LEVEL);
    }
    NdisFReturnNetBufferLists(context->ndis_filter_handle, net_buffer_lists, return_flags);

    // run our XDP chain
    auto xrv = run_xdp(context, net_buffer_lists_copy);

    // we don't have a resources flag
    NDIS_SET_RECEIVE_FLAG
    receive_flags &= ~NDIS_RECEIVE_FLAGS_RESOURCES;

    // handle our XDP results
    if (xrv.pass_net_buffer_list != NULL)
    {
        NdisFIndicateReceiveNetBufferLists(context->ndis_filter_handle,
                                           xrv.pass_net_buffer_list,
                                           port_number,
                                           xrv.pass_count,
                                           receive_flags);
    }
    // XDP_TX is valid on the receive path; send.
    if (xrv.tx_net_buffer_lists != NULL)
    {
        ULONG send_flags;
        if (NDIS_TEST_RECEIVE_AT_DISPATCH_LEVEL(receive_flags))
        {
            NDIS_SET_SEND_FLAG(send_flags, NDIS_SEND_FLAGS_DISPATCH_LEVEL);
        }
        NdisFSendNetBufferLists(
            context->ndis_filter_handle, xrv.tx_net_buffer_list, port_number, send_flags);
        // TODO: does port_number need to get updated from each NET_BUFFER? Therefore, do we need to
        // send each separately to ensure correct port handling?
    }
    // dropped NBLs are simply freed since we allocated them.
    while (xrv.drop_net_buffer_lists != NULL)
    {
        PNET_BUFFER_LIST popped = xrv.drop_net_buffer_lists;
        xrv.drop_net_buffer_lists = NET_BUFFER_LIST_NEXT_NBL(xrv.drop_net_buffer_lists);
        NdisFreeNetBufferList(popped);
    }
}

// filter_return_net_buffer_lists
_Use_decl_annotations_ VOID filter_return_net_buffer_lists(NDIS_HANDLE filter_instance_context_void,
                                                           PNET_BUFFER_LIST net_buffer_lists,
                                                           ULONG return_flags)
{
    FILTER_INSTANCE_CONTEXT *context = filter_instance_context_void;

    // look for NBLs we allocated
    for (PNET_BUFFER_LIST *current_nbl_ptr = &net_buffer_lists; *current_nbl_ptr != NULL;
         current_nbl_ptr = &NET_BUFFER_LIST_NEXT_NBL(*current_nbl_ptr))
    {
        // if we allocated this NBL (likely any we receive on this path), free it
        if ((*current_nbl_ptr)->SourceHandle == context->ndis_filter_handle)
        {
            // remove the NBL
            PNET_BUFFER_LIST popped = *current_nbl_ptr;
            current_nbl_ptr = &NET_BUFFER_LIST_NEXT_NBL(popped);
            // free it
            NdisFreeNetBufferList(popped);
        }
    }

    // any remaining NBLs (unlikely) belong to another driver.
    if (net_buffer_lists != NULL)
    {
        NdisFReturnNetBufferLists(context->ndis_filter_handle, net_buffer_lists, return_flags);
    }
}

}; // namespace win_xdp