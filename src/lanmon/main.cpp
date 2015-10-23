#include <tchar.h>
#include <winsock2.h>
#include <commctrl.h>
#include <winnetwk.h>
#include "resource.h"


INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void EnumComputers(LPNETRESOURCE lpNetResource, LPARAM lParam);


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc);

	WSACleanup();

	return 0;
}


INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		LVCOLUMN lvColumn = {0};
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lvColumn.pszText = TEXT("计算机名");
		lvColumn.cx = 200;
		lvColumn.iSubItem = 0;
		HWND hwndCtrl = GetDlgItem(hDlg, IDC_LIST_COMPUTER);
		ListView_InsertColumn(hwndCtrl, 0, &lvColumn);
		lvColumn.pszText = TEXT("IP地址");
		lvColumn.cx = 200;
		lvColumn.iSubItem = 1;
		ListView_InsertColumn(hwndCtrl, 1, &lvColumn);
		lvColumn.pszText = TEXT("MAC地址");
		lvColumn.cx = 200;
		lvColumn.iSubItem = 2;
		ListView_InsertColumn(hwndCtrl, 2, &lvColumn);
		EnumComputers(NULL, (LPARAM)hwndCtrl);
		return TRUE;
	}
	break;
	case WM_COMMAND:
	{
		int nId = LOWORD(wParam);
		int nEvent = HIWORD(wParam);
	}
	break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return TRUE;
	break;
	}

	return FALSE;
}


void EnumComputers(LPNETRESOURCE lpNetResource, LPARAM lParam)
{
	HANDLE hEnum = NULL;
	DWORD dwCount = -1;
	LPNETRESOURCE lpNetResourceList = NULL;
	DWORD dwBufferSize = 64 * 1024;

	lpNetResourceList = (LPNETRESOURCE)new char[dwBufferSize];
	memset(lpNetResourceList, 0, dwBufferSize);

	WNetOpenEnum(RESOURCE_CONTEXT, NULL, NULL, lpNetResource, &hEnum);
	if (WNetEnumResource(hEnum, &dwCount, lpNetResourceList, &dwBufferSize) == NO_ERROR)
	{
		for (DWORD i = 0; i < dwCount; i++)
		{
			if (lpNetResourceList[i].dwUsage == RESOURCEUSAGE_CONTAINER && lpNetResourceList[i].dwType == RESOURCETYPE_ANY && lpNetResourceList[i].lpRemoteName && lpNetResourceList[i].lpRemoteName[0] == TEXT('\\') && lpNetResourceList[i].lpRemoteName[1] == TEXT('\\'))
			{
				HWND hwndCtrl = (HWND)lParam;
				LVITEM lvItem = {0};
				lvItem.mask = LVIF_TEXT;
				lvItem.pszText = lpNetResourceList[i].lpRemoteName + 2;
				lvItem.iItem = ListView_GetItemCount(hwndCtrl);
				lvItem.iSubItem = 0;
				ListView_InsertItem(hwndCtrl, &lvItem);
				HOSTENT *host = NULL;
				char szHostName[MAX_PATH] = {0};
				WideCharToMultiByte(CP_ACP, 0, lvItem.pszText, -1, szHostName, MAX_PATH, NULL, NULL);
				host = gethostbyname(szHostName);
				if (host == NULL)
				{

					continue;				}
				TCHAR wszIpAddress[MAX_PATH] = {0};
				char szIpAddress[MAX_PATH] = {0};
				strcpy(szIpAddress, inet_ntoa(*((in_addr *)host->h_addr_list[0])));
				MultiByteToWideChar(CP_ACP, NULL, szIpAddress, -1, wszIpAddress, MAX_PATH);
				lvItem.pszText = wszIpAddress;
				lvItem.iSubItem = 1;
				ListView_SetItem(hwndCtrl, &lvItem);
			}
		}
	}
	WNetCloseEnum(hEnum);
}
