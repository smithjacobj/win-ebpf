#pragma once

#define NTDDI_VERSION NTDDI_VISTA
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#define NDIS60

namespace win_ebpf {

#define __WIN_EBPF_DRIVER_NAME  L"win-ebpf"
extern constexpr wchar_t DRIVER_NAME[] = __WIN_EBPF_DRIVER_NAME;
extern constexpr wchar_t DOS_NAME[] = L"\\DosDevices\\" __WIN_EBPF_DRIVER_NAME;
extern constexpr wchar_t NT_NAME[] = L"\\Device\\" __WIN_EBPF_DRIVER_NAME;
#undef __WIN_EBPF_DRIVER_NAME

extern constexpr wchar_t DRIVER_GUID[] = L"{56da9066-9ac2-4cab-ba27-98c75da45291}";
extern constexpr uint32_t DRIVER_SIGNATURE = 'FPBe'; // 'eBPF' when viewed in the debugger

extern constexpr int DRIVER_MAJOR_VERSION = 0;
extern constexpr int DRIVER_MINOR_VERSION = 1;

}; // namespace win_ebpf