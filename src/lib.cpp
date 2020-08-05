#include "lib.h"

#include <psapi.h>
#include <QFileInfo>


BOOL getWindowInfo(HWND hwnd, QString& title, QString& exeName, bool& is64)
{
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];
    int res1 = ::GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);
    if(res1 != 0)
    {
        title = QString::fromWCharArray(windowTitle);
    } else {
        title = QStringLiteral("");
        return FALSE;
    }

    title = QString::fromWCharArray(windowTitle);

    WCHAR fileName[MAX_PATH];
    DWORD dwPID;
    ::GetWindowThreadProcessId(hwnd, &dwPID);
    HANDLE hproc = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
    if(!hproc)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] Unable to read process.");

    DWORD res2 = ::GetModuleFileNameEx(hproc, 0, fileName, MAX_PATH);
    if(res2 == 0)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] Unable to read process name.");

    exeName = QFileInfo(QString::fromWCharArray(fileName)).fileName();

    BOOL isWow64;
    BOOL res3 = IsWow64Process(hproc, &isWow64);
    if(!res3)
        throw std::exception("[ TASK FAILED SUCCESSFULLY ] Could not get determie process bitness");

    is64 = !isWow64;

    return TRUE;
}
