#include "pending_operation_list.h"

#include "log.h"

FAST_MUTEX g_pending_operation_list_lock;
LIST_ENTRY g_pending_operation_list;

VOID pending_operation_list_initialize() {
	LOG_MSG("pending_operation_list_initialize START");

	InitializeListHead(&g_pending_operation_list);
	ExInitializeFastMutex(&g_pending_operation_list_lock);
	LOG_MSG("pending_operation_list_initialize END");

}

VOID pending_operation_list_append(_In_ PENDING_OPERATION* operation) {
	LOG_MSG("pending_operation_list_append START");

	ExAcquireFastMutex(&g_pending_operation_list_lock);
	InsertTailList(&g_pending_operation_list, &(operation->list_entry));
	ExReleaseFastMutex(&g_pending_operation_list_lock);
	LOG_MSG("pending_operation_list_append END");

}

PENDING_OPERATION* pending_operation_list_remove_by_id(_In_ CONST ULONG operation_id)
{
	LOG_MSG("pending_operation_list_remove_by_id START");

	ExAcquireFastMutex(&g_pending_operation_list_lock);
	PLIST_ENTRY entry = g_pending_operation_list.Flink;
	while (entry != &g_pending_operation_list) {
		// CONTAINING_RECORD: get a pointer to a parent structure when you have a pointer to one of its fields
		PENDING_OPERATION* pending_operation = CONTAINING_RECORD(entry, PENDING_OPERATION, list_entry);
		if (pending_operation->operation_id == operation_id) {
			RemoveEntryList(entry);
			ExReleaseFastMutex(&g_pending_operation_list_lock);
			return pending_operation;
		}

		entry = entry->Flink;
	}
	ExReleaseFastMutex(&g_pending_operation_list_lock);

	LOG_MSG("pending_operation_list_remove_by_id END");

	return NULL;
}

NTSTATUS add_operation_to_pending_list(_In_ PFLT_CALLBACK_DATA data, _In_ ULONG operation_id)
{
	LOG_MSG("add_operation_to_pending_list START");

	// Set up pending op context, store calllback data and wait event
	PENDING_OPERATION* pending = ExAllocatePoolWithTag(NonPagedPool, sizeof(PENDING_OPERATION), PENDING_OPERATION_TAG);
	if (!pending) {
		LOG_MSG("ExAllocatePoolWithTag Pending: STATUS_INSUFFICIENT_RESOURCES");
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	RtlZeroMemory(pending, sizeof(PENDING_OPERATION));
	pending->operation_id = operation_id;
	pending->data = data;
	pending_operation_list_append(pending);

	LOG_MSG("add_operation_to_pending_list END");

	return STATUS_SUCCESS;

}

VOID pending_operation_list_clear() {
	LOG_MSG("pending_operation_list_clear START");

	ExAcquireFastMutex(&g_pending_operation_list_lock);
	PLIST_ENTRY head = RemoveHeadList(&g_pending_operation_list);
	while (head != &g_pending_operation_list) {
		ExFreePoolWithTag(head, PENDING_OPERATION_TAG);
		head = RemoveHeadList(&g_pending_operation_list);
	}

	head->Flink = NULL;
	head->Blink = NULL;

	ExReleaseFastMutex(&g_pending_operation_list_lock);
	LOG_MSG("pending_operation_list_clear END");

}

// TODO deinitialize entry_list, lock