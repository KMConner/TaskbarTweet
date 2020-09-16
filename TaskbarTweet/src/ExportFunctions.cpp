#include <Windows.h>
#include "CClassFactory.h"
#include <ShlObj.h>
#include <Shlwapi.h>

#pragma comment (lib, "shlwapi.lib")

/// <summary>
/// このDLL全体の参照カウンタ。
/// </summary>
ULONG g_lLocks = 0;
extern HINSTANCE g_hInstDll;
// CLSID
const CLSID CLSID_TWEET_BAND = { 0xb4c849ef, 0x85b3, 0x4f55, { 0x96, 0xa7, 0x33, 0xfe, 0xee, 0x54, 0xb0, 0xeb } };
const TCHAR CLSID_TEXT_TWEET_BAND[] = TEXT("{B4C849EF-85B3-4F55-96A7-33FEEE54B0EB}");

BOOL CreateRegistryKey(HKEY hKeyRoot, LPCWSTR lpszKey, LPCWSTR lpszValue, LPCWSTR lpszData)
{
	HKEY hkey;
	LONG lResult;
	DWORD dwSize;

	lResult = RegCreateKeyEx(hKeyRoot, lpszKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkey, NULL);

	if (lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	if (lpszData != NULL)
	{
		dwSize = (lstrlenW(lpszData) + 1) * sizeof(WCHAR);
	}
	else
	{
		dwSize = 0;
	}

	RegSetValueEx(hkey, lpszValue, 0, REG_SZ, (LPBYTE)lpszData, dwSize);
	RegCloseKey(hkey);

	return TRUE;
}


/// <summary>
/// このモジュール全体の参照カウンタの操作。
/// </summary>
/// <param name="bLock"></param>
void LockModule(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_lLocks);
	}
	else
	{
		InterlockedDecrement(&g_lLocks);
	}
}

BOOL RegisterCategoryManager(BOOL bRegister)
{
	HRESULT      hr;
	ICatRegister* pCatRegister;
	CATID        catid = CATID_DeskBand;

	CoInitialize(NULL);

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pCatRegister));
	if (SUCCEEDED(hr))
	{
		if (bRegister)
			hr = pCatRegister->RegisterClassImplCategories(CLSID_TWEET_BAND, 1, &catid);
		else
			hr = pCatRegister->UnRegisterClassImplCategories(CLSID_TWEET_BAND, 1, &catid);
		pCatRegister->Release();
	}

	CoUninitialize();

	return hr;
}


BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstDll = hInstDll;
		DisableThreadLibraryCalls(hInstDll);
		return TRUE;
	}
	return TRUE;
}

STDAPI DllCanUnloadNow()
{
	return g_lLocks == 0 ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	static CClassFactory serverFactory;
	HRESULT hr;

	*ppv = NULL;

	if (IsEqualCLSID(rclsid, CLSID_TWEET_BAND))
	{
		hr = serverFactory.QueryInterface(riid, ppv);
	}
	else
	{
		hr = CLASS_E_CLASSNOTAVAILABLE;
	}
	return hr;
}

STDAPI DllRegisterServer(void)
{
	TCHAR szModulePath[256];
	TCHAR szKey[256];

	wsprintf(szKey, TEXT("CLSID\\%s"), CLSID_TEXT_TWEET_BAND);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, NULL, TEXT("Task Bar Tweeting")))
	{
		return E_FAIL;
	}

	GetModuleFileName(g_hInstDll, szModulePath, sizeof(szModulePath) / sizeof(TCHAR));
	wsprintf(szKey, TEXT("CLSID\\%s\\InprocServer32"), CLSID_TEXT_TWEET_BAND);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, NULL, szModulePath))
	{
		return E_FAIL;
	}

	wsprintf(szKey, TEXT("CLSID\\%s\\InprocServer32"), CLSID_TEXT_TWEET_BAND);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, TEXT("ThreadingModel"), TEXT("Apartment")))
	{
		return E_FAIL;
	}

	RegisterCategoryManager(TRUE);

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	TCHAR szKey[256];

	wsprintf(szKey, TEXT("CLSID\\%s"), CLSID_TEXT_TWEET_BAND);
	SHDeleteKey(HKEY_CLASSES_ROOT, szKey);

	RegisterCategoryManager(FALSE);

	return S_OK;
}
