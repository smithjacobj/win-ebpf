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

#include "main.h"

namespace win_xdp {

extern PDRIVER_OBJECT global_driver_object;
extern FilterGlobalContext global_filter_context;
extern NDIS_HANDLE global_driver_handle;

extern PDEVICE_OBJECT global_root_device_object;
extern NDIS_HANDLE global_root_device_handle;

}; // namespace win_xdp