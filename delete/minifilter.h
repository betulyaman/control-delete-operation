#ifndef KERNEL_DELETE_MINIFILTER_H
#define KERNEL_DELETE_MINIFILTER_H

#include <fltKernel.h>

#include "delete_minifilter.h"
#include "log.h"

typedef enum {
	INVALID_OPERATION = 0,
	DELETE_OPERATION,
	MOVE_OPERATION,
	RENAME_OPERATION
}OPERATION_TYPE;

typedef struct {
	PFLT_FILTER registered_filter;
	PFLT_PORT server_port;
	PFLT_PORT client_port;
} DELETE_CONTEXT;

extern DELETE_CONTEXT g_context;

NTSTATUS register_filter(_In_ PDRIVER_OBJECT driver_object);

NTSTATUS connect_notify_callback(
	_In_ PFLT_PORT client_port,
	_In_ PVOID server_port_cookie,
	_In_reads_bytes_(size_of_context) PVOID conneciton_context,
	_In_ ULONG size_of_context,
	_Outptr_result_maybenull_ PVOID* connection_cookie);

VOID disconnect_notify_callback(_In_opt_ PVOID connection_cookie);

NTSTATUS filter_unload_callback(FLT_FILTER_UNLOAD_FLAGS flags);

NTSTATUS user_reply_notify_callback(
	_In_ PVOID port_cookie,
	_In_reads_bytes_opt_(input_buffer_length) PVOID input_buffer,
	_In_ ULONG input_buffer_length,
	_Out_writes_bytes_to_opt_(output_buffer_length, *return_output_buffer_length) PVOID output_buffer,
	_In_ ULONG output_buffer_length,
	_Out_ PULONG return_output_buffer_length
);

FLT_PREOP_CALLBACK_STATUS pre_operation_callback(
	_Inout_ PFLT_CALLBACK_DATA data,
	_In_ PCFLT_RELATED_OBJECTS filter_objects,
	_Flt_CompletionContext_Outptr_ PVOID* completion_context
);

#endif //KERNEL_DELETE_MINIFILTER_H