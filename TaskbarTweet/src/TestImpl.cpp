#ifdef _UI_TEST

#include "TestHeader.h"
#include "TweetBand.h"

extern HINSTANCE g_hInstDll;

DLLAPI void TestUI(HINSTANCE hInst)
{
	g_hInstDll = hInst;
	TweetBand* t = new TweetBand();
	t->RegisterAndCreateWindow(NULL);
	t->ShowDW(TRUE);
	HWND h;
	t->GetWindow(&h);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	t->Release();

}

#endif // _UI_TEST
