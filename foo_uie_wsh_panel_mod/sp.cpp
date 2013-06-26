#include "stdafx.h"
#include "script_interface_impl.h"
#include "sp.h"
#include "script_interface_playlist_impl.h"
#include "helpers.h"
#include "InputBox.h"
#include "com_array.h"
#include "gdiplus_helpers.h"
#include "boxblurfilter.h"
#include "user_message.h"
#include "panel_manager.h"
#include "popup_msg.h"
#include "dbgtrace.h"
#include "obsolete.h"
#include "../TextDesinger/OutlineText.h"
#include "../TextDesinger/PngOutlineText.h"


/***********************
twinkle,twinkle little star (> w <)v
***********************/
STDMETHODIMP WSHAliens::get_fbHWND(UINT * p)
{
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	
	*p = (UINT)core_api::get_main_window();

	return S_OK;
}

STDMETHODIMP WSHAliens::get_IsWin6(VARIANT_BOOL * p)
{
	TRACK_FUNCTION();
	if (!p) return E_POINTER;

	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(ovi);
	::GetVersionEx(&ovi);

	*p = TO_VARIANT_BOOL(ovi.dwMajorVersion >= 6);
	
	return S_OK;
}

STDMETHODIMP WSHAliens::GetHWND(BSTR classID, UINT * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	
	*p = (UINT)::FindWindow((LPCTSTR) classID, NULL);

	return S_OK;
}

STDMETHODIMP WSHAliens::GetHWND2(BSTR title, UINT * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	
	*p = (UINT)::FindWindow(NULL, (LPCTSTR) title);

	return S_OK;
}

STDMETHODIMP WSHAliens::GetChildHWND(UINT parentHWND, BSTR classID, int index, UINT * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	
	helpers::CFindWnd fw((HWND)parentHWND,(LPCTSTR) classID, index);

	*p = (UINT)fw.m_hWnd;

	return S_OK;
}

STDMETHODIMP WSHAliens::GetParentHWND(UINT hWnd, UINT * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	
	*p = (UINT)::GetParent((HWND)hWnd);

	return S_OK;
}

STDMETHODIMP WSHAliens::SendMessageEx(UINT hWnd, UINT MsgNum, UINT wParam, UINT lParam){
	TRACK_FUNCTION();

	::SendMessage((HWND)hWnd,MsgNum,(WPARAM)wParam, (LPARAM) lParam);

	return S_OK;
}

STDMETHODIMP WSHAliens::ReleaseCaptureEx(){
	TRACK_FUNCTION();

	::ReleaseCapture();

	return S_OK;
}

STDMETHODIMP WSHAliens::ShareInfo(UINT target, VARIANT name, VARIANT info)
{
	TRACK_FUNCTION();

	if (info.vt & VT_BYREF) return E_INVALIDARG;
	if (name.vt & VT_BYREF) return E_INVALIDARG;

	HRESULT hr = S_OK;
	_variant_t v_info, v_name;

	hr = VariantCopy(&v_info, &info);
	if (FAILED(hr)) return hr;

	hr = VariantCopy(&v_name, &name);
	if (FAILED(hr)) return hr;

	simple_callback_data_2<_variant_t, _variant_t> * notify_data
		= new simple_callback_data_2<_variant_t, _variant_t>(NULL, NULL);

	notify_data->m_item1.Attach(v_name.Detach());
	notify_data->m_item2.Attach(v_info.Detach());

//	notify_data->refcount_add_ref();
	SendMessage((HWND)target, CALLBACK_UWM_NOTIFY_DATA, reinterpret_cast<WPARAM>(notify_data), 0);
	return S_OK;
}

STDMETHODIMP WSHAliens::MoveWindowEx(UINT hWnd, int x, int y, int w, int h, VARIANT_BOOL refresh){
	TRACK_FUNCTION();

	::MoveWindow((HWND)hWnd,x,y,w,h,(BOOL)refresh);

	return S_OK;
}

STDMETHODIMP WSHAliens::GetWindowInfoEx(UINT hWnd, int Idx ,int * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;

	WINDOWINFO pwi;

	pwi.cbSize = sizeof(WINDOWINFO);

	::GetWindowInfo((HWND)hWnd, &pwi);

	int ret = 0;
	
	switch(Idx){
	case 0:
		ret = pwi.rcClient.left;
	break;

	case 1:
		ret = pwi.rcClient.top;
	break;

	case 2:
		ret = pwi.rcClient.right-pwi.rcClient.left;
	break;

	case 3:
		ret = pwi.rcClient.bottom-pwi.rcClient.top;
	break;

	case 4:
		ret = pwi.rcWindow.left;
	break;

	case 5:
		ret = pwi.rcWindow.top;
	break;

	case 6:
		ret = pwi.rcWindow.right-pwi.rcWindow.left;
	break;

	case 7:
		ret = pwi.rcWindow.bottom-pwi.rcWindow.top;
	break;

	default:
		ret = pwi.dwWindowStatus;
	break;
	}

	*p = ret;
	
	return S_OK;
}

STDMETHODIMP WSHAliens::GetWindowInfoAx(UINT hWnd, VARIANT * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;

	WINDOWINFO pwi;

	pwi.cbSize = sizeof(WINDOWINFO);

	::GetWindowInfo((HWND)hWnd, &pwi);

	const int elements[] = 
    {
		pwi.rcClient.left,
		pwi.rcClient.top,
		pwi.rcClient.right-pwi.rcClient.left,
		pwi.rcClient.bottom-pwi.rcClient.top,

		pwi.rcWindow.left,
		pwi.rcWindow.top,
		pwi.rcWindow.right-pwi.rcWindow.left,
		pwi.rcWindow.bottom-pwi.rcWindow.top,

		pwi.dwWindowStatus
    };

    helpers::com_array_writer<> helper;

    if (!helper.create(_countof(elements)))
        return E_OUTOFMEMORY;

    for (long i = 0; i < helper.get_count(); ++i)
    {
        _variant_t var;
        var.vt = VT_I4;
        var.lVal = elements[i];

        if (FAILED(helper.put(i, var)))
        {
            helper.reset();
            return E_OUTOFMEMORY;
        }
    }

    p->vt = VT_ARRAY | VT_VARIANT;
    p->parray = helper.get_ptr();

	return S_OK;
}


STDMETHODIMP WSHAliens::ShowWindowEx(UINT hWnd, int command){
	TRACK_FUNCTION();

	::ShowWindow((HWND)hWnd,command);

	return S_OK;
}

STDMETHODIMP WSHAliens::IsWindowVisibleEx(UINT hWnd, VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	*p = TO_VARIANT_BOOL(::IsWindowVisible((HWND)hWnd));

	return S_OK;
}

STDMETHODIMP WSHAliens::IsMinimizedEx(UINT hWnd, VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	*p = TO_VARIANT_BOOL(::IsIconic((HWND)hWnd));

	return S_OK;
}

STDMETHODIMP WSHAliens::IsMaximizedEx(UINT hWnd, VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	*p = TO_VARIANT_BOOL(::IsZoomed((HWND)hWnd));

	return S_OK;
}

STDMETHODIMP WSHAliens::CreateGlass(UINT hWnd, int l, int r, int t, int b){
	TRACK_FUNCTION();
	
#ifdef _ENABLE_DWM_
	MARGINS margins = {l,r,t,b};
	::DwmExtendFrameIntoClientArea((HWND)hWnd,&margins);
#endif
	return S_OK;
}

STDMETHODIMP WSHAliens::IsGlassEnabled(VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;
	BOOL ret = false;

#ifdef _ENABLE_DWM_
	::DwmIsCompositionEnabled(&ret);
#endif
	*p = TO_VARIANT_BOOL(ret);
	
	return S_OK;
}

STDMETHODIMP WSHAliens::ShowScrollBarEx(UINT hWnd, int target, VARIANT_BOOL show){
	TRACK_FUNCTION();

	::ShowScrollBar((HWND)hWnd,target,(BOOL)show);

	return S_OK;
}

STDMETHODIMP WSHAliens::IsScrollbarVisible(UINT hWnd, int target, VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;

	LONG wndStyle = ::GetWindowLong((HWND)hWnd,GWL_STYLE);

	switch(target){
	case 0:
		*p = TO_VARIANT_BOOL(wndStyle&WS_HSCROLL);
	break;
	case 1:
		*p = TO_VARIANT_BOOL(wndStyle&WS_VSCROLL);
	break;
	default:
		*p = TO_VARIANT_BOOL(wndStyle&WS_HSCROLL&WS_VSCROLL);
	break;
	}
	return S_OK;
}

STDMETHODIMP WSHAliens::AddStyle(UINT hWnd, long style){
	TRACK_FUNCTION();

	uAddWindowStyle((HWND)hWnd, style);

	return S_OK;
}

STDMETHODIMP WSHAliens::RemoveStyle(UINT hWnd, long style){
	TRACK_FUNCTION();

	uRemoveWindowStyle((HWND)hWnd, style);
	
	return S_OK;
}

STDMETHODIMP WSHAliens::AddExStyle(UINT hWnd, long exstyle){
	TRACK_FUNCTION();

	uAddWindowExStyle((HWND)hWnd, exstyle);

	return S_OK;
}

STDMETHODIMP WSHAliens::RemoveExStyle(UINT hWnd, long exstyle){
	TRACK_FUNCTION();

	uRemoveWindowExStyle((HWND)hWnd, exstyle);

	return S_OK;
}

/*
STDMETHODIMP WSHAliens::DecodeBase64Image(BSTR data, IGdiBitmap **pp){
	TRACK_FUNCTION();
	if (!data) return E_INVALIDARG;
    if (!pp) return E_POINTER;
    (*pp) = NULL;

	std::string codeImg = _com_util::ConvertBSTRToString(data);
	std::string decodedImage = helpers::base64_decode(codeImg);
	
	DWORD imageSize = decodedImage.length();

	IStreamPtr pStream;

	BYTE* pImage = new BYTE[imageSize];

	memcpy(pImage, decodedImage.c_str(), imageSize);
	pStream = ::SHCreateMemStream(pImage,imageSize);

	Gdiplus::Bitmap * img = new Gdiplus::Bitmap(pStream, PixelFormat32bppPARGB);

	if (!helpers::ensure_gdiplus_object(img))
    {
        if (img) delete img;
        return S_OK;
    }
	delete [] pImage;
	(*pp) = new com_object_impl_t<GdiBitmap>(img);

    return S_OK;
}*/

STDMETHODIMP WSHAliens::GetWindowStyleEx(UINT hWnd, UINT* p){
	TRACK_FUNCTION();

	*p = GetWindowStyle((HWND)hWnd);

	return S_OK;
}

STDMETHODIMP WSHAliens::SetWindowStyleEx(UINT hWnd, int style){
	TRACK_FUNCTION();

	::SetWindowLong((HWND)hWnd,GWL_STYLE,style);

	return S_OK;
}

STDMETHODIMP WSHAliens::CalcTTCode(int ID, BSTR HexString, int * p){
	TRACK_FUNCTION();

    if (!ID || !HexString) return E_INVALIDARG;
    if (!p) return E_POINTER;

	CString buffer((LPCTSTR)HexString);
	
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString (buffer);

	// construct a std::string using the LPCSTR input
	std::string ttHexStr (pszConvertedAnsiString);

	*p = helpers::TTCode(ttHexStr, ID);

    return S_OK;
}

/*STDMETHODIMP WSHAliens::HttpRequestText(BSTR url, BSTR * pp){
	TRACK_FUNCTION();
	if (!url) return E_INVALIDARG;
	if (!pp) return E_POINTER;
	static_api_ptr_t<http_client> hc;
	_bstr_t burl(url);

	file::ptr fp = hc->create_request("GET")->run(burl,	foobar2000_io::abort_callback_dummy());

	pfc::string pstr = fp->read_string(foobar2000_io::abort_callback_dummy());

	_bstr_t bpstr(pstr.ptr());

	*pp = bpstr;

	return S_OK;
}*/

STDMETHODIMP WSHAliens::CreateCursor(BSTR filePath,LONG* p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;

	*p = (LONG)::LoadCursorFromFile((LPCWSTR)filePath);

	return S_OK;
}

STDMETHODIMP WSHAliens::ChangeCursor(LONG hCUR, LONG * p){
	TRACK_FUNCTION();
	if (!p) return E_POINTER;

	*p = (LONG)::SetCursor((HCURSOR)(hCUR));

	return S_OK;
}

STDMETHODIMP WSHAliens::ReleaseCursor(LONG hCUR){
	TRACK_FUNCTION();

	::DestroyCursor((HCURSOR)(hCUR));

	return S_OK;
}

STDMETHODIMP WSHAliens::CreateMessageBox(UINT hWnd, BSTR caption, BSTR prompt, UINT type, int * p){
	TRACK_FUNCTION();
	
	if (!p) return E_POINTER;
	*p = ::MessageBox((HWND)hWnd,prompt,caption,type);

	return S_OK;
}

STDMETHODIMP WSHAliens::CreateFileDialog(UINT hWnd, BSTR fileType, BSTR ext, BSTR window_title, VARIANT_BOOL save, BSTR * pp){
	TRACK_FUNCTION();
	if (!pp) return E_POINTER;

	_bstr_t fileT(fileType);
	_bstr_t extT(ext);
	_bstr_t wndTitle(window_title);

	pfc::string8 filename;
	if(uGetOpenFileName((HWND)hWnd, fileT, 0, extT, wndTitle, NULL, filename, (BOOL)save))
	{
		static pfc::stringcvt::string_wide_from_utf8 path(filename);
		(*pp) = SysAllocString(path.get_ptr());
	}

	return S_OK;
}

STDMETHODIMP WSHAliens::CreateInputBox(UINT hWnd, BSTR caption, BSTR prompt,BSTR defText, BSTR * pp){
	TRACK_FUNCTION();
	if (!pp) return E_POINTER;

	CInputBox ibox((HWND)hWnd);
	if (ibox.DoModal(caption, prompt,defText)){
		(*pp) = SysAllocString(ibox.Text);
	}

	return S_OK;
}

STDMETHODIMP WSHAliens::AnimateWindowEx(UINT hWnd,UINT time, UINT flags)
{
	TRACK_FUNCTION();

	AnimateWindow((HWND)hWnd, time, flags);

	return S_OK;
}

/*
STDMETHODIMP WSHAliens::RunMainMenuCommandByGUID(BSTR bguid,VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!bguid) return E_INVALIDARG;
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(standard_commands::run_main(helpers::BSTRtoGUID(bguid)));

	return S_OK;
}

STDMETHODIMP WSHAliens::RunContextMenuCommandByGUID(BSTR bguid,UINT flag, VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!bguid) return E_INVALIDARG;
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(standard_commands::run_context(helpers::BSTRtoGUID(bguid),metadb_handle_list()));

	return S_OK;
}
STDMETHODIMP WSHAliens::RunContextMenuCommandByGUIDWithMetadb(BSTR bguid, VARIANT handle, UINT flag,VARIANT_BOOL * p){
	TRACK_FUNCTION();
	if (!bguid) return E_INVALIDARG;
	if (!p) return E_POINTER;

	*p = TO_VARIANT_BOOL(true);

	return S_OK;
}
STDMETHODIMP WSHAliens::GetCommandGUID(BSTR name, BSTR * pp){
	TRACK_FUNCTION();
	if (!pp) return E_POINTER;
	
	std::string commName = _com_util::ConvertBSTRToString(name);

	GUID gret;
	menu_helpers::guid_from_name(commName.c_str(),commName.length(),gret);

	pfc::print_guid pguid(gret);
	
	*pp = _com_util::ConvertStringToBSTR(pguid);
	return S_OK;
}*/