#pragma once
#define DLLAPI extern "C" __declspec(dllexport)

#include <ShlObj.h>

/// <summary>
/// Main module of this desk band,
/// </summary>
class TweetBand :public IDeskBand2, public IObjectWithSite, public IPersistStream
{
public:

#pragma region Inherited from IUnknown

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);

	STDMETHODIMP_(ULONG) AddRef();

	STDMETHODIMP_(ULONG) Release();

#pragma endregion

#pragma region Inhereted from IOleWindow

	STDMETHODIMP GetWindow(HWND* phwnd);

	STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

#pragma endregion

#pragma region Inhereted from IDockingWindow

	STDMETHODIMP ShowDW(BOOL bShow);

	STDMETHODIMP CloseDW(DWORD dwReserved);

	STDMETHODIMP ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);

#pragma endregion

#pragma region Inhereted from IDeskBand

	STDMETHODIMP GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);

#pragma endregion

#pragma region Inhereted from IDeskBand2

	STDMETHODIMP CanRenderComposited(BOOL* pfCanRenderComposited);

	STDMETHODIMP SetCompositionState(BOOL fCompositionEnabled);

	STDMETHODIMP GetCompositionState(BOOL* pfCompositionEnabled);

#pragma endregion

#pragma region Inhereted from IObjectWithSite

	STDMETHODIMP SetSite(IUnknown* pUnkSite);

	STDMETHODIMP GetSite(REFIID riid, void** ppvSite);

#pragma endregion

#pragma region Inhereted from IPersistent

	STDMETHODIMP GetClassID(CLSID* pClassID);

#pragma endregion

#pragma region Inhereted from IPersistentStream

	STDMETHODIMP IsDirty();

	STDMETHODIMP Load(IStream* pStm);

	STDMETHODIMP Save(IStream* pStm, BOOL fClearDirty);

	STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pcbSize);

#pragma endregion

	/// <summary>
	/// Constructor.
	/// </summary>
	TweetBand();

	/// <summary>
	/// Destructor.
	/// </summary>
	~TweetBand();

	/// <summary>
	/// Create and register a new window.
	/// </summary>
	/// <param name="hwndParent">Window handle to the parent window.</param>
	/// <returns>Returns TRUE if the creation and registeration is successful, otherwise FALSE.</returns>
	BOOL RegisterAndCreateWindow(HWND hwndParent);

	/// <summary>
	/// Static window procedure function.
	/// </summary>
	static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Windows procedure function.
	/// </summary>
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Window procedure function for the terxtbox.
	/// </summary>
	static LRESULT CALLBACK TextWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	BOOL OnPaint(HWND hwnd);

	void AdjustSize();

private:
	/// <summary>
	/// Reference counter,
	/// </summary>
	LONG m_cRef;

	/// <summary>
	/// Window handle to this deskband.
	/// </summary>
	HWND m_hwnd;

	/// <summary>
	/// Window handle to text edit.
	/// </summary>
	HWND m_textEdit;

	/// <summary>
	/// Window handle to the tweet button.
	/// </summary>
	HWND m_tweetButton;

	BOOL m_bCompositionEnabled;

	IUnknown* m_pSite;
};
