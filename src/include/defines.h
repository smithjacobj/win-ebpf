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

#pragma once

#define NTDDI_VERSION NTDDI_VISTA
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#define NDIS60

namespace win_xdp {

#define __WIN_XDP_DRIVER_NAME L"win-xdp"
extern constexpr wchar_t DRIVER_NAME[] = __WIN_XDP_DRIVER_NAME;
extern constexpr wchar_t DOS_NAME[] = L"\\DosDevices\\" __WIN_XDP_DRIVER_NAME;
extern constexpr wchar_t NT_NAME[] = L"\\Device\\" __WIN_XDP_DRIVER_NAME;
#undef __WIN_XDP_DRIVER_NAME

extern constexpr wchar_t DRIVER_GUID[] = L"{56da9066-9ac2-4cab-ba27-98c75da45291}";
extern constexpr uint32_t DRIVER_SIGNATURE = 'PDXW'; // 'WXDP' when viewed in the debugger

extern constexpr int DRIVER_MAJOR_VERSION = 0;
extern constexpr int DRIVER_MINOR_VERSION = 1;

extern constexpr size_t MAX_PACKET_SIZE = std::numeric_limits<uint16_t>::max();

enum tx_rx {
    TX = 1,
    RX = 2,
};

}; // namespace win_xdp