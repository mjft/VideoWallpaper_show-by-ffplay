#include <windows.h>
#include <stdio.h>
#include <atlbase.h>//char转LPCWSTR
#include <string.h>
#include <cstring>
#include <string>

using namespace std;

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		// 找它的下一个窗口，类名为WorkerW，隐藏它
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);

		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK EnumWindowsProc2(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		// 找它的下一个窗口，类名为WorkerW
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		if (hWorkerw != 0)
		{
			// 找它的下一个窗口，类名为WorkerW，隐藏
			HWND hWorkerw2 = FindWindowEx(0, hwnd, L"WorkerW", 0);
			ShowWindow(hWorkerw2, SW_HIDE);
			return FALSE;
		}
		
	}
	return TRUE;
}

int ShowVideoWapper(LPCWSTR lpParameter, LPCWSTR ffplayPath)
{
	STARTUPINFO si{ 0 };//隐藏进程窗口
	si.cb = sizeof(STARTUPINFO);
	si.lpReserved = NULL;
	si.lpDesktop = NULL;
	si.lpTitle = NULL;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.cbReserved2 = NULL;
	si.lpReserved2 = NULL;
	PROCESS_INFORMATION pi{ 0 };

	if (CreateProcess(ffplayPath, (LPWSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		WaitForSingleObject(pi.hProcess, INFINITE);// 等待视频播放器启动完成。否则会显示不全
		CloseHandle(pi.hThread);

		Sleep(1000);// 等待视频播放器启动完成。否则会显示不到全屏

		HWND hProgman = FindWindow(L"Progman", 0);				// 找到PM窗口
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
		HWND hFfplay = FindWindow(L"SDL_app", 0);				// 找到视频窗口
		SetWindowPos(hFfplay, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);//设置窗口大小，使
		SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
		EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
	}
	return 0;
}

int CloseVideoWapper()
{
	HWND hProgman = FindWindow(L"Progman", 0);// 找到PM窗口
	HWND hFfplay = FindWindowExW(//找到PM窗口的子窗口，也就是视频窗口
		_In_opt_ hProgman,
		_In_opt_ NULL,
		_In_opt_ L"SDL_app",
		_In_opt_ NULL);
	//获得窗口句柄
	DWORD dwPID = 0;
	GetWindowThreadProcessId(hFfplay, &dwPID);
	//编写关闭语句
	char strCmd[MAX_PATH] = { 0 };
	sprintf_s(strCmd, "taskkill /pid %d -f", dwPID);
	//执行
	WinExec(strCmd, SW_HIDE);//不显示黑框
	return 0;
}

int Do(LPCWSTR lpParameter, LPCWSTR ffplayPath)
{
	STARTUPINFO si{ 0 };
	PROCESS_INFORMATION pi{ 0 };

	if (CreateProcess(ffplayPath, (LPWSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		//Sleep(99999);
	}
	return 0;
}
