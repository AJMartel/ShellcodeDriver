# ShellcodeDriver
Windows driver to execute arbitrary usermode code (essentially same vulnerability as capcom.sys)

### Functionality

The driver takes an ioctl with a pointer to a user-land function (or shellcode). It disables SMEP, calls the function and passes a pointer to the MmGetSystemRoutineAddress as an argument.

https://github.com/zerosum0x0/ShellcodeDriver/blob/master/shellcodedriver/shellcodedriver.c#L80

### Exploitation

If you want to get SYSTEM, you can use the following functions to copy a system processes token to your current process. The whole point of MmGetSystemRoutineAddress is these function pointers are simple to obtain.

- PsGetCurrentProcessId
- PsLookupProcessByProcessId
- ObDereferenceObject
- PsReferencePrimaryToken
- PsDereferencePrimaryToken
