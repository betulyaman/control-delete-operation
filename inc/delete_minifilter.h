#ifndef DELETE_MINIFILTER_H
#define DELETE_MINIFILTER_H

#define COMMUNICATION_PORT_NAME L"\\CommunicationPort"

#define MAX_FILE_NAME_LENGTH 260

typedef struct {
	ULONG operation_id;
	BOOLEAN allow;
} USER_REPLY;

typedef struct {
	ULONG operation_id;
	UINT16 operation_type;
	WCHAR target_name[260];
	WCHAR file_name[260];
} CONFIRMATION_MESSAGE;

#endif //DELETE_MINIFILTER_H