// ForceShutdown.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <Windows.h>
#include <stdio.h>

int DoShutDown(int waittime)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// 特権を取得するためのトークンを開く
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		printf("OpenProcessToken failed\n");
		return 1;
	}

	// 特権を有効にする
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
	{
		printf("AdjustTokenPrivileges failed\n");
		return 1;
	}

	// シャットダウンする
	//if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCEIFHUNG, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED))
	if (!InitiateSystemShutdownExW(NULL, NULL, waittime, TRUE, FALSE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED))
	{
		printf("InitiateSystemShutdownExW failed\n");
		return 1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		int waitTime = atoi(argv[1]);
		if (waitTime == 0) {
			printf("Usage: ForceShutdown [wait time (sec)]\n");
			return 1;
		}
		return DoShutDown(waitTime);
	}
	else 
	{
		return DoShutDown(0);
	}
}
