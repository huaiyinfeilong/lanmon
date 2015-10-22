#include <tchar.h>
//#include <windows.h>
#include <winsock2.h>
#include <commctrl.h>
#include <winnetwk.h>
#include "resource.h"

#pragma comment(lib, "ws2_32.lib")

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc);

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

		HANDLE hEnum = NULL;
		DWORD dwCount = -1;
		LPNETRESOURCE lpNetResource = NULL;
		DWORD dwBufferSize = MAX_PATH * sizeof(NETRESOURCE);
		lpNetResource = (LPNETRESOURCE)new char[dwBufferSize];
		memset(lpNetResource, 0, dwBufferSize);
		WSADATA wsaData = {0};
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		WNetOpenEnum(RESOURCE_CONTEXT, NULL, NULL, NULL, &hEnum);
		if (WNetEnumResource(hEnum, &dwCount, lpNetResource, &dwBufferSize) == NO_ERROR)
		{
			int j = 0;
			for (int i = 0; i < dwCount; i++)
			{
				if (lpNetResource[i].lpRemoteName == NULL)
				{
					continue;
				}
				LVITEM lvItem = {0};
				lvItem.mask = LVIF_TEXT;
				lvItem.pszText = lpNetResource[i].lpRemoteName;
				if (lvItem.pszText[0] == TEXT('\\') && lvItem.pszText[1] == TEXT('\\'))
				{
					lvItem.pszText = lvItem.pszText + 2;
				}
				lvItem.iItem = j++;
				lvItem.iSubItem = 0;
				ListView_InsertItem(hwndCtrl, &lvItem);
			}
		}
		WNetCloseEnum(hEnum);
		delete []lpNetResource;
		lpNetResource = NULL;
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
