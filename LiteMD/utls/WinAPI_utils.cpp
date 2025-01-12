#include "WinAPI_utils.h"
#include "restartmanager.h"
#include "qdebug.h"

#pragma comment(lib, "Rstrtmgr.lib")

void freeProcess(const WCHAR* file)
{
	DWORD hSess = 0;
	WCHAR key[48] = L"MySession";
	DWORD res;

	res = RmStartSession(&hSess, 0, key);

	if (res != 0)
	{
		return;
	}

	LPCWSTR filenames[] = { file };
	res = RmRegisterResources(hSess, 1, filenames, 0, NULL, 0, NULL);

	if (res != 0)
	{
		RmEndSession(hSess);
		return;
	}

	UINT procInfoNeeded;
	UINT procInfo = 10;
	DWORD rebootReasons;
	RM_PROCESS_INFO info[10];
	res = RmGetList(hSess, &procInfoNeeded, &procInfo, info, &rebootReasons);

	if (res != 0)
	{
		RmEndSession(hSess);
		return;
	}

	for (INT i = 0; i < procInfo; ++i)
		qDebug() << info[i].Process.dwProcessId << " " << info[i].strAppName;

	res = RmShutdown(hSess, RmForceShutdown, NULL);

	if (res != 0)
	{
		qDebug() << "—бой завершени€ работы";
		RmEndSession(hSess);
		return;
	}

	//RmRestart(hSess, 0, NULL);

	RmEndSession(hSess);
}

void newInstance(LPCSTR iPath)
{
	LPSTR null = const_cast<char*>("");
	STARTUPINFO update = { sizeof(update) };
	PROCESS_INFORMATION proc_info;
	if (CreateProcess(iPath, null, NULL, NULL, TRUE, 0, NULL, NULL, &update, &proc_info))
	{
		CloseHandle(proc_info.hProcess);
		CloseHandle(proc_info.hThread);
	}
	else
		qDebug() << GetLastError();
}
