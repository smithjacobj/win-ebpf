#pragma once

#define NTDDI_VERSION NTDDI_VISTA 
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#define NDIS60

namespace nfq {

extern const int DRIVER_MAJOR_VERSION = 0;
extern const int DRIVER_MINOR_VERSION = 1;

extern const wchar_t DRIVER_NAME[] = L"netfilter_queue";
extern const wchar_t DRIVER_GUID[] = L"{c727f4a2-775d-463e-982a-edaca377fdda}";
extern const uint32_t DRIVER_SIGNATURE = 'wqfn' // 'nfqw' when viewed in the debugger

extern const wchar_t DOS_NAME[] = L"\\DosDevices\\" DRIVER_NAME;
extern const wchar_t NT_NAME[] = L"\\Device\\" DRIVER_NAME;

}; // namespace nfq