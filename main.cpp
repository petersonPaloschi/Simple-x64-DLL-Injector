#include <iostream>
#include <Windows.h>
#include <thread> 
#include <chrono>


int main()
{

	ShellExecuteW(NULL, L"open", L"notepad.exe",  NULL, NULL, SW_SHOWNORMAL);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	LPCSTR DllPath = ""; //put your dir \\dll.dll
	
	//HWND hwnd = FindWindowA(NULL, "Notepad"); 
	HWND hwnd = ::FindWindow(TEXT("Notepad"), NULL); //dll inject in notepad

	DWORD procID; 
	GetWindowThreadProcessId(hwnd, &procID); 
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID); 

	LPVOID pDllPath = VirtualAllocEx(handle, 0, strlen(DllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(handle, pDllPath, (LPVOID)DllPath, strlen(DllPath) + 1, 0);

	HANDLE hLoadThread = CreateRemoteThread(handle, 0, 0,(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), pDllPath, 0, 0);

	WaitForSingleObject(hLoadThread, INFINITE); 

	VirtualFreeEx(handle, pDllPath, strlen(DllPath) + 1, MEM_RELEASE); 


	return 0;
}
