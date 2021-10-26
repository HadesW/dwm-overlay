// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <windows.h>
#include <process.h>

#include "dwm-overlay-a.hpp"

//
BOOL APIENTRY DllMain(HMODULE /* hModule */, DWORD ul_reason_for_call, LPVOID /* lpReserved */)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		uintptr_t handle = _beginthreadex(nullptr, NULL, main_thread, nullptr, NULL, nullptr);
		if (!handle)
		{
			CloseHandle(reinterpret_cast<HANDLE>(handle));
		}

		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
