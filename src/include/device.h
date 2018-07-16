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

namespace win_xdp
{

// https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/sddl-for-device-objects
const PCUNICODE_STRING DEFAULT_SDDL = SDDL_DEVOBJ_SYS_ALL_ADM_ALL;

// root_device_register registers a virtual device to receive IRPs. The root device's
// responsibilities are general queries, e.g. listing all available filter instances. Filter
// instances will have their own device to use for assigning eBPF programs, etc.
_IRQL_requires_max_(PASSIVE_LEVEL) NDIS_STATUS root_device_register();

}; // namespace win_xdp