#pragma once

#define wx_print(level, format, ...)                                                               \
    DbgPrintExA(DPFLTR_IHVNETWORK_ID, level, "%s:%d %s: "(format), __FILE__, __LINE__,             \
                __FUNCTION__, __VA_ARGS__)

#define wx_error_print(format, ...) wx_print(DPFLTR_ERROR_LEVEL, "ERROR: "(format), __VA_ARGS__)
#define wx_warn_print(format, ...) wx_print(DPFLTR_WARNING_LEVEL, "WARNING: "(format), __VA_ARGS__)
#define wx_trace_print(format, ...) wx_print(DPFLTR_TRACE_LEVEL, "TRACE: "(format), __VA_ARGS__)
#define wx_info_print(format, ...) wx_print(DPFLTR_INFO_LEVEL, "INFO: "(format), __VA_ARGS__)