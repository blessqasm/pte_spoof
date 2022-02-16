#include "./other.h"

PEPROCESS other::getObjectProcess(const wchar_t* name_process) {
	constexpr LONG activeLinks_offset = 0x448; // 0x448;
	PEPROCESS objSystem = PsInitialSystemProcess;
	NTSTATUS status;

	PLIST_ENTRY listEntry = reinterpret_cast<PLIST_ENTRY>(reinterpret_cast<runtime::uint64_t>(objSystem) + activeLinks_offset);
	PLIST_ENTRY prevEntry = listEntry->Blink;
	PLIST_ENTRY nextEntry = listEntry->Flink;

	while (prevEntry != nextEntry)
	{
		PEPROCESS objProcess = reinterpret_cast<PEPROCESS>(reinterpret_cast<runtime::uint64_t>(nextEntry) - activeLinks_offset);
		PUNICODE_STRING nameProcess;

		nextEntry = nextEntry->Flink;

		status = SeLocateProcessImageName(objProcess, &nameProcess);
		if (!NT_SUCCESS(status))
			continue;
		if (MmIsAddressValid(nameProcess) && nameProcess->Length)
		{
			if (wcsstr(nameProcess->Buffer, name_process))
				return objProcess;
		}
	}
	return nullptr;
}