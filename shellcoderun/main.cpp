#include <Windows.h>
#include <stdio.h>

#include <winternl.h>
#include "../shellcodedriver/shellcodedriver.h"

extern "C" BOOLEAN ManageDriver(LPCTSTR  DriverName, LPCTSTR ServiceName, USHORT Function);
extern "C" BOOLEAN SetupDriverName(PCHAR DriverLocation, ULONG BufferLength);

HANDLE get_device(char *szDevice)
{
	return CreateFileA(szDevice, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

static int test_val = 0;

void test()
{
	test_val = 1;
}

int main()
{
	TCHAR driverLocation[MAX_PATH] = { 0 };
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	char *szDevice = "\\\\.\\ShellcodeDriver";
	char outputBuffer[100];
	DWORD bytesReturned;

	if ((hDevice = get_device(szDevice)) == INVALID_HANDLE_VALUE)
	{
		if (!SetupDriverName(driverLocation, sizeof(driverLocation)))
			return -1;

		if (!ManageDriver(DRIVER_NAME, driverLocation, DRIVER_FUNC_INSTALL))
		{
			ManageDriver(DRIVER_NAME, driverLocation, DRIVER_FUNC_REMOVE);
			printf("Unable to install driver. \n");
			return -1;
		}

		if ((hDevice = get_device(szDevice)) == INVALID_HANDLE_VALUE)
		{
			printf("Error: CreatFile Failed : %d\n", GetLastError());
			return -1;
		}
	}

	printf("BEFORE: test_val = %d\n", test_val);

	SCD_IOCTL_IN_BUFFER inBuf;
	inBuf.ShellcodeAddress = test;

	if (!DeviceIoControl(hDevice,
		(DWORD)SCD_EXEC_IOCTL_BUFFERED,
		&inBuf,
		sizeof(inBuf),
		&outputBuffer,
		sizeof(outputBuffer),
		&bytesReturned,
		NULL
	))
	{
		printf("Error: ioctl Failed : %08x\n", GetLastError());
		ManageDriver(DRIVER_NAME, driverLocation, DRIVER_FUNC_REMOVE);
		return -1;
	}

	printf("AFTER: test_val = %d\n", test_val);


	ManageDriver(DRIVER_NAME, driverLocation, DRIVER_FUNC_REMOVE);
	system("PAUSE");

	return 0;
}