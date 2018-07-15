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

namespace win_xdp
{

// FilterGlobalContext is the driver-global context area for the entire driver.
struct FilterGlobalContext
{
};

// FilterInstanceContext is the module-local storage for each driver module instance.
struct FilterInstanceContext
{
    NDIS_HANDLE ndis_filter_handle;
    NDIS_HANDLE net_buffer_list_pool;
};

// RootDeviceExtension is the device-local storage for the base device that tracks all filter
// instance devices.
struct RootDeviceExtension
{
};

} // namespace win_xdp