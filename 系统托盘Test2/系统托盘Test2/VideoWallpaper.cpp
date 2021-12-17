#include <windows.h>
#include <stdio.h>
#include <atlbase.h>//charתLPCWSTR
#include <string.h>
#include <cstring>
#include <string>

using namespace std;

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		// ��������һ�����ڣ�����ΪWorkerW��������
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
		// ��������һ�����ڣ�����ΪWorkerW
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		if (hWorkerw != 0)
		{
			// ��������һ�����ڣ�����ΪWorkerW������
			HWND hWorkerw2 = FindWindowEx(0, hwnd, L"WorkerW", 0);
			ShowWindow(hWorkerw2, SW_HIDE);
			return FALSE;
		}
		
	}
	return TRUE;
}

int ShowVideoWapper(LPCWSTR lpParameter, LPCWSTR ffplayPath)
{
	STARTUPINFO si{ 0 };//���ؽ��̴���
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
		WaitForSingleObject(pi.hProcess, INFINITE);// �ȴ���Ƶ������������ɡ��������ʾ��ȫ
		CloseHandle(pi.hThread);

		Sleep(1000);// �ȴ���Ƶ������������ɡ��������ʾ����ȫ��

		HWND hProgman = FindWindow(L"Progman", 0);				// �ҵ�PM����
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// ������������Ϣ
		HWND hFfplay = FindWindow(L"SDL_app", 0);				// �ҵ���Ƶ����
		SetWindowPos(hFfplay, HWND_TOP, 0, 0, 1920, 1080, SWP_SHOWWINDOW);//���ô��ڴ�С��ʹ
		SetParent(hFfplay, hProgman);							// ����Ƶ��������ΪPM���Ӵ���
		EnumWindows(EnumWindowsProc, 0);						// �ҵ��ڶ���WorkerW���ڲ�������
	}
	return 0;
}

int CloseVideoWapper()
{
	HWND hProgman = FindWindow(L"Progman", 0);// �ҵ�PM����
	HWND hFfplay = FindWindowExW(//�ҵ�PM���ڵ��Ӵ��ڣ�Ҳ������Ƶ����
		_In_opt_ hProgman,
		_In_opt_ NULL,
		_In_opt_ L"SDL_app",
		_In_opt_ NULL);
	//��ô��ھ��
	DWORD dwPID = 0;
	GetWindowThreadProcessId(hFfplay, &dwPID);
	//��д�ر����
	char strCmd[MAX_PATH] = { 0 };
	sprintf_s(strCmd, "taskkill /pid %d -f", dwPID);
	//ִ��
	WinExec(strCmd, SW_HIDE);//����ʾ�ڿ�
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
