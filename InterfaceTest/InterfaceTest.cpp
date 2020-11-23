#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#ifdef _UI_TEST
#include "TestHeader.h"
#endif

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	// UIテストモード以外での実行を禁止する。
#ifndef _UI_TEST
	MessageBox(NULL, TEXT("You should build with _UI_TEST preprocessor derective."), TEXT("Error"), 0);
	return 0;
#else
	TestUI(hinst);
	_CrtDumpMemoryLeaks();
	return 0;
#endif
}
