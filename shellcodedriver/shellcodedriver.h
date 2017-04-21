#pragma once

#define SCD_DEVICE_TYPE 0x13371337
#define SCD_EXEC_IOCTL_BUFFERED CTL_CODE( SCD_DEVICE_TYPE, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define NT_DEVICE_NAME L"\\Device\\ShellcodeDriver"
#define DOS_DEVICE_NAME L"\\DosDevices\\ShellcodeDriver"

#define DRIVER_NAME       "shellcodedriver"

#define DRIVER_FUNC_INSTALL     0x01
#define DRIVER_FUNC_REMOVE      0x02

typedef struct _SCD_IOCTL_IN_BUFFER
{
	void *ShellcodeAddress;
} SCD_IOCTL_IN_BUFFER, *PSCD_IOCTL_IN_BUFFER;

typedef PVOID(NTAPI * MmGetSystemRoutineAddress_t)(PUNICODE_STRING/* SystemRoutineName */);
typedef void(__cdecl * shellcode_function_t)(MmGetSystemRoutineAddress_t);