#include "appmanager.h"

#include <QFileInfo>

#include "lib.h"

extern HWND g_hwnd;

typedef void (__stdcall *f_funci)(HWND hwnd);

AppManager::AppManager(QObject *parent):
    QObject(parent)
{

}


void AppManager::refresh()
{
    // remove closed windows
    QMutableVectorIterator<APPDATA> it(m_data);
    while(it.hasNext()) {
        HWND hwnd = it.next().hwnd;
        if(!IsWindow(hwnd))
            it.remove();
    }

    BOOL ret = EnumWindows(windowListCallback, reinterpret_cast<LPARAM>(this));
    if(!ret)
        throw std::exception("[TASK FAILED SUCCESSFULLY] Could not get windows list");

    emit listChanged();
}


void AppManager::add(HWND hwnd)
{
    for(auto it : m_data) {
        if(it.hwnd == hwnd) {
            return;
        }
    }

    QString name = "";
    QString exeName = "";
    bool is64 = false;
    try {
        BOOL res = getWindowInfo(hwnd, name, exeName, is64);
        if(res && IsWindowVisible(hwnd) && is64) {
            m_data.push_back({hwnd, name, exeName, is64, false});
        }
    } catch(std::exception e) {
        qDebug() << e.what();
    }
}


void AppManager::installHooks()
{
    for(auto it : m_data) {
        if(it.selected && it.is64) {
            try {
                InstallSingleHook(it.hwnd);
            } catch(std::exception e) {
                qDebug() << e.what();
            }
        }
    }

    m_hooksactive = true;
}


void AppManager::InstallSingleHook(HWND hwnd)
{
    BOOL ret;

    DWORD pid = NULL;
    DWORD tid = GetWindowThreadProcessId(hwnd, &pid);
    if (tid == NULL)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] Could not get thread ID of the target window.");

    HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hproc == NULL)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] Failed to open process.");

    BOOL isWow64;
    ret = IsWow64Process(hproc, &isWow64);
    if(!ret)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] Could not get determie process bitness");

    if(isWow64)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] It's a 32bit process");

    HMODULE dll = LoadLibraryEx(L"hooks_dll.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (dll == NULL)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] The DLL could not be found.");

    HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "HookProc");
    if (addr == NULL)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] The function was not found.");

    f_funci setGlobalHwnd = (f_funci)GetProcAddress(dll, "setGlobalHwnd");
    if(!setGlobalHwnd)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] The function setGlobalHwnd was not found.");

    setGlobalHwnd(g_hwnd);

    HHOOK handle = SetWindowsHookEx(WH_MOUSE, addr, dll, tid);
    if (handle == NULL)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] Couldn't set the hook with SetWindowsHookEx.");

    m_hooks.push_back({handle, dll});
}


void AppManager::uninstallHooks()
{
    for(auto it : m_hooks) {
        BOOL res = UnhookWindowsHookEx(it.hook);
        if(res == FALSE)
            throw std::exception("[ TASK FAILED SUCCESSFULLY ] Could not remove the hook.");

        BOOL res1 = FreeLibrary(it.hdll);
        if(!res1)
            throw std::exception("[ TASK FAILED SUCCESSFULLY ] Could not unload dll.");
    }

    m_hooks.clear();
}


void AppManager::select(int i)
{
    m_data[i].selected = !m_data.at(i).selected;
}


int AppManager::size()
{
    return m_data.size();
}


const APPDATA AppManager::at(int i)
{
    return m_data.at(i);
}


BOOL windowListCallback(HWND hwnd, LPARAM lParam)
{
    auto appmanager = reinterpret_cast<AppManager*>(lParam);
    appmanager->add(hwnd);
    return TRUE;
}

