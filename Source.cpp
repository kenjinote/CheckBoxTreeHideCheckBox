#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"comctl32")

#include<windows.h>
#include<commctrl.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hTree;
	switch (msg)
	{
	case WM_CREATE:
	{
		InitCommonControls();
		hTree = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			WC_TREEVIEW,
			0,
			WS_CHILD | WS_VISIBLE /*| TVS_CHECKBOXES*/,
			10, 10, 200, 300,
			hWnd,
			0,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);

		SetWindowLongPtr(hTree, GWL_STYLE, GetWindowLong(hTree, GWL_STYLE) | TVS_CHECKBOXES);

		TV_INSERTSTRUCT tv = { 0 };
		tv.hInsertAfter = TVI_LAST;
		tv.item.mask = TVIF_STATE | TVIF_TEXT;
		tv.item.stateMask = TVIS_STATEIMAGEMASK;
		tv.item.state = 0;
		tv.hParent = TVI_ROOT;
		tv.item.pszText = TEXT("アイテム1");
		HTREEITEM hParent = TreeView_InsertItem(hTree, &tv);

		// チェックボックスを非表示にする
		TVITEMEX itemex = { 0 };
		itemex.mask = TVIF_STATE;
		itemex.stateMask = TVIS_STATEIMAGEMASK;
		itemex.state = 0;
		itemex.hItem = hParent;
		SendMessage(hTree, TVM_SETITEM, 0, (LPARAM)&itemex);

		tv.item.pszText = TEXT("アイテム2");
		TreeView_InsertItem(hTree, &tv);

		tv.item.pszText = TEXT("アイテム3");
		TreeView_InsertItem(hTree, &tv);
	}
	break;
	case WM_NOTIFY:
		if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
		{
			TCHAR    szBuf[256];
			TVITEMEX item;

			item.mask = TVIF_HANDLE | TVIF_TEXT;
			item.hItem = ((LPNMTREEVIEW)lParam)->itemNew.hItem;
			item.pszText = szBuf;
			item.cchTextMax = sizeof(szBuf) / sizeof(TCHAR);
			TreeView_GetItem(hTree, &item);

			MessageBox(hWnd, szBuf, 0, 0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("チェックボックス付きのツリーコントロールで特定のアイテムだけチェックボックスを非表示にする"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
