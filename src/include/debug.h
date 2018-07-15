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

#define wx_print(level, format, ...)                                                               \
    DbgPrintExA(DPFLTR_IHVNETWORK_ID, level, "%s:%d %s: "(format), __FILE__, __LINE__,             \
                __FUNCTION__, __VA_ARGS__)

#define wx_error_print(format, ...) wx_print(DPFLTR_ERROR_LEVEL, "ERROR: "(format), __VA_ARGS__)
#define wx_warn_print(format, ...) wx_print(DPFLTR_WARNING_LEVEL, "WARNING: "(format), __VA_ARGS__)
#define wx_trace_print(format, ...) wx_print(DPFLTR_TRACE_LEVEL, "TRACE: "(format), __VA_ARGS__)
#define wx_info_print(format, ...) wx_print(DPFLTR_INFO_LEVEL, "INFO: "(format), __VA_ARGS__)