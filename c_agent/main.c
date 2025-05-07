#include <Windows.h>
#include <fltUser.h>
#include <stdio.h>
#include <winioctl.h>

#include "delete_minifilter.h"

typedef struct {
	FILTER_MESSAGE_HEADER header;
	CONFIRMATION_MESSAGE body;
} GET_MESSAGE;

typedef enum {
	INVALID_OPERATION = 0,
	DELETE_OPERATION,
	MOVE_OPERATION,
	RENAME_OPERATION
}OPERATION_TYPE;


void split_device_and_path(const wchar_t* full_path,
	wchar_t* device_name, size_t device_name_size,
	wchar_t* remaining_path, size_t remaining_path_size);

int main() {
	HANDLE port = INVALID_HANDLE_VALUE;
	HRESULT result;
	boolean is_connected = 0;
	while (!is_connected) {

		result = FilterConnectCommunicationPort(COMMUNICATION_PORT_NAME, 0, NULL, 0, NULL, &port);
		if (FAILED(result)) {
			printf("Connect error: 0x%08x\n", result);
			continue;
		}
		else {
			is_connected = 1;
		}
		Sleep(1000);
	}

	GET_MESSAGE message;
	HRESULT hresult;
	while (TRUE) {
		hresult = FilterGetMessage(
			port,
			&(message.header),
			sizeof(GET_MESSAGE),
			NULL
		);

		if (hresult != S_OK) {
			printf("FilterGetMessage FAILED hresult: %d\n", hresult);
			continue;
		}

		switch (message.body.operation_type) {
			case DELETE_OPERATION:
				printf("\nDELETE: ");
				break;
			case RENAME_OPERATION:
				printf("\nRENAME: ");
				break;
			case MOVE_OPERATION:
				printf("\nMOVE: ");
				break;
		}
		
		// File name
		wchar_t device_name[256];
		wchar_t remaining_path[256];

		split_device_and_path(message.body.file_name, device_name, 256, remaining_path, 256);

		wchar_t dos_path[256];
		HRESULT result = FilterGetDosName(device_name, dos_path, 256);
		if (result != S_OK) {
			printf("\n%S", message.body.file_name);
		}
		else {
			printf("\n%S%S", dos_path, remaining_path);
		}
		//----------------------------------------------//
		if (message.body.operation_type == RENAME_OPERATION || message.body.operation_type == MOVE_OPERATION) {
			wchar_t device_name[256];
			wchar_t remaining_path[256];

			split_device_and_path(message.body.target_name, device_name, 256, remaining_path, 256);

			wchar_t dos_path[256];
			HRESULT result = FilterGetDosName(device_name, dos_path, 256);
			if (result != S_OK) {
				printf("\t-->\t%S", message.body.target_name);
			}
			else {
				printf("\t-->\t%S%S", dos_path, remaining_path);
			}
		}

		LONG operation_id = message.body.operation_id;
		BOOLEAN allow;
		printf("\nY or N ?\n");
		while (TRUE) {
			int reply = fgetc(stdin);
			if (reply == 'Y' || reply == 'y') {
				allow = 1;
				break;
			}
			else if (reply == 'N' || reply == 'n') {
				allow = 0;
				break;
			}
		}

		USER_REPLY reply = { .operation_id = operation_id, .allow = allow };
		DWORD bytes_returned;
		hresult = FilterSendMessage(port, &reply, sizeof(USER_REPLY), NULL, 0, &bytes_returned);
		if (hresult != S_OK) {
			printf("FilterSendMessage FAILED hresult: %d\n", hresult);
			continue;
		}

	}
	return 0;
}

void split_device_and_path(const wchar_t* full_path,
	wchar_t* device_name, size_t device_name_size,
	wchar_t* remaining_path, size_t remaining_path_size)
{
	int slash_count = 0;
	const wchar_t* ptr = full_path;

	while (*ptr) {
		if (*ptr == L'\\') {
			slash_count++;
			if (slash_count == 3) {
				break;
			}
		}
		ptr++;
	}

	if (slash_count < 3) {
		wcsncpy(device_name, full_path, device_name_size - 1);
		device_name[device_name_size - 1] = L'\0';
		remaining_path[0] = L'\0';
		return;
	}

	size_t deviceLen = ptr - full_path;
	if (deviceLen >= device_name_size) deviceLen = device_name_size - 1;
	wcsncpy(device_name, full_path, deviceLen);
	device_name[deviceLen] = L'\0';

	wcsncpy(remaining_path, ptr, remaining_path_size - 1);
	remaining_path[remaining_path_size - 1] = L'\0';
}