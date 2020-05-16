/*
MIT License

Copyright (c) 2020 Nexo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Memory.h"

#include <Windows.h>
#include <TlHelp32.h>

cMemory::cMemory() {
	ProcessHandle = NULL;
	ProcessID = NULL;
}

cMemory::~cMemory() {
	CloseHandle(ProcessHandle);
}

DWORD cMemory::FindProcess(const char* ProcessName) {
	HANDLE Module = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(ProcessEntry);
	do {
		if (!strcmp(ProcessEntry.szExeFile, ProcessName)) {
			ProcessID = ProcessEntry.th32ProcessID;
			CloseHandle(Module);
			ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);
		}
	} while (Process32Next(Module, &ProcessEntry));
	return ProcessID;
}

DWORD cMemory::FindAddress(DWORD Address, std::vector<DWORD> Vector) {
	for (unsigned int i = 0; i < Vector.size(); i++) {
		ReadProcessMemory(ProcessHandle, (BYTE*)Address, &Address, sizeof(Address), 0);
		Address += Vector[i];
	}
	return Address;
}

uintptr_t cMemory::GetModuleBaseAddress(const char* ModuleName) {
	uintptr_t ModuleBaseAddress = NULL;
	HANDLE Module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
	MODULEENTRY32 ModuleEntry;
	ModuleEntry.dwSize = sizeof(ModuleEntry);

	if (Module32First(Module, &ModuleEntry)) {
		do {
			if (!strcmp(ModuleEntry.szModule, ModuleName)) {
				ModuleBaseAddress = (uintptr_t)ModuleEntry.modBaseAddr;
				break;
			}
		} while (Module32Next(Module, &ModuleEntry));
	}

	CloseHandle(Module);
	return ModuleBaseAddress;
}

uintptr_t cMemory::FindModule(const char* ModuleName) {
	HANDLE Module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
	MODULEENTRY32 ModuleEntry;
	ModuleEntry.dwSize = sizeof(ModuleEntry);
	do {
		if (!strcmp(ModuleEntry.szModule, ModuleName)) {
			CloseHandle(Module);
			return (DWORD)ModuleEntry.hModule;
		}
	} while (Module32Next(Module, &ModuleEntry));
	return 0;
}

uintptr_t cMemory::FindDMAAddy(uintptr_t Pointer, std::vector<unsigned int> Offsets) {
	uintptr_t Address = Pointer;

	for (unsigned int i = 0; i < Offsets.size(); ++i) {
		Address = ReadMemory<uintptr_t>(Address);
		Address += Offsets[i];
	}

	return Address;
}