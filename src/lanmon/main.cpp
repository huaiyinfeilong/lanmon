#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

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
