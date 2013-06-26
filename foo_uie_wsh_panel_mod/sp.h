#pragma once

#include "script_interface_impl.h"
#include "script_interface.h"
#include "com_tools.h"
#include "helpers.h"

class WSHAliens : public IDispatchImpl3<IWSHAliens>
{
protected:
	WSHAliens() {}
	virtual ~WSHAliens() {}

public:

	STDMETHODIMP get_fbHWND(UINT * p);
	STDMETHODIMP get_IsWin6(VARIANT_BOOL * p);
	STDMETHODIMP GetHWND(BSTR classID, UINT * p);
	STDMETHODIMP GetHWND2(BSTR title, UINT * p);
	STDMETHODIMP GetChildHWND(UINT parentHWND, BSTR classID, int index, UINT * p);
	STDMETHODIMP GetParentHWND(UINT hWnd, UINT * p);

	STDMETHODIMP SendMessageEx(UINT hWnd, UINT MsgNum, UINT wParam, UINT lParam);
	STDMETHODIMP ReleaseCaptureEx();
	STDMETHODIMP ShareInfo(UINT target, VARIANT name, VARIANT info);

	STDMETHODIMP MoveWindowEx(UINT hWnd, int x, int y, int w, int h,VARIANT_BOOL refresh);
	STDMETHODIMP GetWindowInfoEx(UINT hWnd, int Idx ,int * p);
	STDMETHODIMP GetWindowInfoAx(UINT hWnd, VARIANT * p);

	STDMETHODIMP ShowWindowEx(UINT hWnd, int command);
	STDMETHODIMP IsWindowVisibleEx(UINT hWnd, VARIANT_BOOL * p);
	STDMETHODIMP IsMinimizedEx(UINT hWnd, VARIANT_BOOL * p);
	STDMETHODIMP IsMaximizedEx(UINT hWnd, VARIANT_BOOL * p);
	
	STDMETHODIMP CreateGlass(UINT hWnd, int l, int r, int t, int b);
	STDMETHODIMP IsGlassEnabled(VARIANT_BOOL * p);

	STDMETHODIMP ShowScrollBarEx(UINT hWnd, int target, VARIANT_BOOL show);
	STDMETHODIMP IsScrollbarVisible(UINT hWnd, int target, VARIANT_BOOL * p);

	STDMETHODIMP AddStyle(UINT hWnd, long style);
	STDMETHODIMP RemoveStyle(UINT hWnd, long style);
	STDMETHODIMP AddExStyle(UINT hWnd, long exstyle);
	STDMETHODIMP RemoveExStyle(UINT hWnd, long exstyle);

//	STDMETHODIMP DecodeBase64Image(BSTR data,IGdiBitmap **pp);

	STDMETHODIMP GetWindowStyleEx(UINT hWnd, UINT* p);
	STDMETHODIMP SetWindowStyleEx(UINT hWnd, int style);

	STDMETHODIMP CalcTTCode(int ID, BSTR HexString, int * p);
//	STDMETHODIMP HttpRequestText(BSTR url, BSTR * pp);
	STDMETHODIMP CreateCursor(BSTR filePath, LONG * p);
	STDMETHODIMP ChangeCursor(LONG hCUR, LONG * p);
	STDMETHODIMP ReleaseCursor(LONG hCUR);

	STDMETHODIMP CreateMessageBox(UINT hWnd, BSTR caption, BSTR prompt, UINT type, int * p);
	STDMETHODIMP CreateInputBox(UINT hWnd, BSTR caption, BSTR prompt,BSTR defText, BSTR * pp);
	STDMETHODIMP CreateFileDialog(UINT hWnd, BSTR fileType, BSTR ext,BSTR window_title, VARIANT_BOOL save, BSTR * pp);


/*	STDMETHODIMP RunMainMenuCommandByGUID(BSTR bguid, VARIANT_BOOL * p);
	STDMETHODIMP RunContextMenuCommandByGUID(BSTR bguid,UINT flag, VARIANT_BOOL * p);
	STDMETHODIMP RunContextMenuCommandByGUIDWithMetadb(BSTR bguid, VARIANT handle, UINT flag,VARIANT_BOOL * p);
	STDMETHODIMP GetCommandGUID(BSTR name, BSTR * pp);*/

	STDMETHODIMP AnimateWindowEx(UINT hWnd,UINT time, UINT flags);
};