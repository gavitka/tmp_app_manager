#include "hook.h"

#pragma comment(linker, "/SECTION:.SHARED,RWS")
#pragma data_seg(".SHARED")

static HWND g_callerHWND = NULL;

#pragma data_seg()

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    (void)lpReserved;
    (void)hModule;
    (void)ul_reason_for_call;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || nCode == HC_NOREMOVE) {
        return ::CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    HWND hwnd = g_callerHWND;
    if(hwnd != NULL)
        PostMessage(hwnd, WM_KEYSTROKE, wParam, lParam);
    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}


void setGlobalHwnd(HWND hwnd)
{
    g_callerHWND = hwnd;
}


