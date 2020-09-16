#include "TweetBand.h"
#include "ClsId.h"
#include <Uxtheme.h>
#include <CommCtrl.h>
#include "PipeClient.h"
#define ID_BANDWINDOW 100
#pragma comment (lib, "uxtheme.lib")
#pragma comment (lib,"comctl32.lib")
#define ID_BUTTON 2
#define ID_TEXT_HOTKEY 0xC000

extern void LockModule(BOOL bLock);

HINSTANCE g_hInstDll = NULL;

#pragma region IUnknown

STDMETHODIMP TweetBand::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_IUnknown) ||
		IsEqualIID(riid, IID_IOleWindow) ||
		IsEqualIID(riid, IID_IDockingWindow) ||
		IsEqualIID(riid, IID_IDeskBand) ||
		IsEqualIID(riid, IID_IDeskBand2))
	{
		*ppvObject = static_cast<IDeskBand*>(this);
	}
	else if (IsEqualIID(riid, IID_IPersist) ||
		IsEqualIID(riid, IID_IPersistStream))
	{
		*ppvObject = static_cast<IPersistStream*>(this);
	}
	else if (IsEqualIID(riid, IID_IObjectWithSite))
	{
		*ppvObject = static_cast<IObjectWithSite*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) TweetBand::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) TweetBand::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}
#pragma endregion

#pragma region IOleWindow

STDMETHODIMP TweetBand::GetWindow(HWND* phwnd)
{
	*phwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP TweetBand::ContextSensitiveHelp(BOOL fEnterMode)
{
	return E_NOTIMPL;

}
#pragma endregion

#pragma region IDockingWindow

STDMETHODIMP TweetBand::ShowDW(BOOL bShow)
{
	if (m_hwnd != NULL)
	{
		ShowWindow(m_hwnd, bShow ? SW_SHOW : SW_HIDE);
	}

	if (m_textEdit != NULL)
	{
		ShowWindow(m_textEdit, bShow ? SW_SHOW : SW_HIDE);
	}

	if (m_tweetButton != NULL)
	{
		ShowWindow(m_tweetButton, bShow ? SW_SHOW : SW_HIDE);
	}
	return S_OK;
}

STDMETHODIMP TweetBand::CloseDW(DWORD dwReserved)
{
	if (m_hwnd != NULL)
	{
		ShowWindow(m_hwnd, SW_HIDE);
		DestroyWindow(m_hwnd);
	}
	return S_OK;
}

STDMETHODIMP TweetBand::ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
	return E_NOTIMPL;
}

#pragma endregion

#pragma region IDeskBand

STDMETHODIMP TweetBand::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
	if (pdbi)
	{
		// Minimum size of this deskband
		if (pdbi->dwMask & DBIM_MINSIZE)
		{
			pdbi->ptMinSize.x = 200;
			pdbi->ptMinSize.y = 30;
		}

		// Maximum size of this deskband
		if (pdbi->dwMask & DBIM_MAXSIZE)
		{
			// x vakue is ignored.
			// -1 means unlimited.
			pdbi->ptMaxSize.y = -1;
		}

		if (pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.y = 1;
		}

		if (pdbi->dwMask & DBIM_ACTUAL)
		{
			pdbi->ptActual.x = 200;
			pdbi->ptActual.y = 50;
		}

		if (pdbi->dwMask & DBIM_TITLE)
		{
			pdbi->dwMask &= ~DBIM_TITLE;
		}

		if (pdbi->dwMask & DBIM_MODEFLAGS)
		{
			pdbi->dwModeFlags = DBIMF_NORMAL | DBIMF_VARIABLEHEIGHT;
		}

		if (pdbi->dwMask & DBIM_BKCOLOR)
		{
			pdbi->dwMask &= ~DBIM_BKCOLOR;
		}
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

#pragma endregion

#pragma region IDeskBand2

STDMETHODIMP TweetBand::CanRenderComposited(BOOL* pfCanRenderComposited)
{
	*pfCanRenderComposited = TRUE;
	return S_OK;
}

STDMETHODIMP TweetBand::SetCompositionState(BOOL fCompositionEnabled)
{
	m_bCompositionEnabled = fCompositionEnabled;

	InvalidateRect(m_hwnd, NULL, FALSE);
	UpdateWindow(m_hwnd);

	return S_OK;
}

STDMETHODIMP TweetBand::GetCompositionState(BOOL* pfCompositionEnabled)
{
	*pfCompositionEnabled = m_bCompositionEnabled;
	return S_OK;
}

#pragma endregion

#pragma region IObjectWithSite

STDMETHODIMP TweetBand::SetSite(IUnknown* pUnkSite)
{
	if (m_pSite)
	{
		m_pSite->Release();
		m_pSite = NULL;
	}

	if (pUnkSite)
	{
		HWND hwndParent;
		IOleWindow* pOleWindow;
		HRESULT hr = S_OK;

		pUnkSite->QueryInterface(IID_PPV_ARGS(&pOleWindow));
		pOleWindow->GetWindow(&hwndParent);
		pOleWindow->Release();
		RegisterAndCreateWindow(hwndParent);
		pUnkSite->QueryInterface(IID_PPV_ARGS(&m_pSite));
		return hr;
	}
	return E_FAIL;
}

STDMETHODIMP TweetBand::GetSite(REFIID riid, void** ppvSite)
{
	*ppvSite = NULL;
	if (m_pSite)
	{
		return m_pSite->QueryInterface(riid, ppvSite);
	}
	else
	{
		return E_FAIL;
	}
}
#pragma endregion

#pragma region IPersist

STDMETHODIMP TweetBand::GetClassID(CLSID* pClassID)
{
	*pClassID = CLSID_TWEET_BAND;
	return S_OK;
}

#pragma endregion

#pragma region IPersistStream

STDMETHODIMP TweetBand::IsDirty()
{
	return E_NOTIMPL;
}

STDMETHODIMP TweetBand::Load(IStream* pStm)
{
	return S_OK;
}

STDMETHODIMP TweetBand::Save(IStream* pStm, BOOL fClearDirty)
{
	return S_OK;
}

STDMETHODIMP TweetBand::GetSizeMax(ULARGE_INTEGER* pcbSize)
{
	return E_NOTIMPL;
}

#pragma endregion

TweetBand::TweetBand()
{
	m_cRef = 1;
	m_hwnd = NULL;
	m_bCompositionEnabled = FALSE;
	m_pSite = NULL;
	m_textEdit = NULL;
	m_tweetButton = NULL;

	LockModule(TRUE);
}

TweetBand::~TweetBand()
{
	LockModule(FALSE);
}

BOOL TweetBand::RegisterAndCreateWindow(HWND hwndParent)
{
	// Window クラスの名前
	TCHAR TWEETBAND_WINDOW_CLASS_NAME[] = TEXT("tweet_band");

	// 本体のウィンドウのクラスを登録
	WNDCLASSEX wc;
	if (!GetClassInfoEx(g_hInstDll, TWEETBAND_WINDOW_CLASS_NAME, &wc))
	{
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = WindowProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = g_hInstDll;
		wc.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
		wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
		wc.hbrBackground = GetSysColorBrush(COLOR_MENU);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = TWEETBAND_WINDOW_CLASS_NAME;
		wc.hIconSm = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);

		RegisterClassEx(&wc);
	}

	m_hwnd = CreateWindowEx(NULL, TWEETBAND_WINDOW_CLASS_NAME, TEXT(""), WS_CHILD | WS_CLIPSIBLINGS, 0, 0, 0, 0, hwndParent, (HMENU)ID_BANDWINDOW, g_hInstDll, this);
	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);

	// Text box
	m_textEdit = CreateWindowEx(NULL, TEXT("EDIT"), TEXT(""), WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 0, 0, 150, 30, m_hwnd, (HMENU)1, g_hInstDll, NULL);
	SendMessage(m_textEdit, EM_SETLIMITTEXT, WPARAM(140), 0);
	HFONT textBoxFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(m_textEdit, WM_SETFONT, (WPARAM)textBoxFont, MAKELPARAM(FALSE, 0));
	SetWindowSubclass(m_textEdit, TextWndProc, 1, 0);

	// Tweet button
	m_tweetButton = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Tweet"), WS_CHILD | BS_CENTER, 151, 0, 30, 30, m_hwnd, (HMENU)ID_BUTTON, g_hInstDll, NULL);

	return TRUE;
}

LRESULT CALLBACK TweetBand::WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TweetBand* p = (TweetBand*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (p == NULL)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		return p->WindowProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK TweetBand::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_THEMECHANGED:
		m_bCompositionEnabled = !m_bCompositionEnabled;
		break;
	case WM_ERASEBKGND:
		return 0;
	case WM_PAINT:
		OnPaint(hwnd);
		return 0;
	case WM_SIZE:
		AdjustSize();
		return 0;
	case WM_COMMAND:
		if (wParam != ID_BUTTON)
			break;

		TCHAR text[200];
		GetWindowText(m_textEdit, text, 200);
		bool ans;
		ans = Send(text, TEXT("\\\\.\\pipe\\tweet"));
		if (ans)
		{
			SetWindowText(m_textEdit, TEXT(""));
			MessageBox(m_hwnd, TEXT("OK"), TEXT(""), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(m_hwnd, TEXT("ERROR"), TEXT(""), MB_OK | MB_ICONWARNING);
		}
		SetFocus(m_textEdit);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK TweetBand::TextWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	/*
	 * Not pass Ctrl + Enter to the DefSubclassProc
	*/
	static bool ctrlKeyPressed = false;
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_CONTROL)
		{
			ctrlKeyPressed = true;
		}
		else if (wParam == VK_RETURN && ctrlKeyPressed)
		{
			HWND parentWnd = GetParent(hWnd);
			SendMessage(parentWnd, WM_COMMAND, ID_BUTTON, 0);
			ctrlKeyPressed = false;
			return 0;
		}
		break;
	case WM_KEYUP:
		if (wParam == VK_CONTROL)
		{
			ctrlKeyPressed = false;
		}
		break;
	case WM_CHAR:
		if (ctrlKeyPressed && wParam == 10)
		{
			return 0;
		}
		break;
	default:
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

BOOL TweetBand::OnPaint(HWND hwnd)
{
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rc);

	if (m_bCompositionEnabled)
	{
		HTHEME htheme;
		HDC hdcPaint;
		HPAINTBUFFER hBufferdPaint;

		htheme = OpenThemeData(NULL, L"TASKBAND");
		hBufferdPaint = BeginBufferedPaint(hdc, &rc, BPBF_TOPDOWNDIB, NULL, &hdcPaint);

		DrawThemeParentBackground(m_hwnd, hdcPaint, &rc);

		EndBufferedPaint(hBufferdPaint, TRUE);
		CloseThemeData(htheme);
	}
	else
	{
		FillRect(hdc, &rc, GetSysColorBrush(COLOR_MENU));
		SetBkMode(hdc, TRANSPARENT);
	}
	EndPaint(hwnd, &ps);
	return TRUE;
}

void TweetBand::AdjustSize()
{
	RECT clientRect;
	GetClientRect(m_hwnd, &clientRect);
	LONG width = clientRect.right - clientRect.left;
	LONG height = clientRect.bottom - clientRect.top;

	SetWindowPos(m_textEdit, HWND_TOP, 2, 2, width - 60, height - 4, SWP_NOZORDER);
	SetWindowPos(m_tweetButton, HWND_TOP, width - 58, 2, 56, height - 2, SWP_NOZORDER);
}
