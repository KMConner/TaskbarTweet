#pragma once
#ifdef _UI_TEST

#define DLLAPI extern "C" __declspec(dllexport)
#include <Windows.h>
DLLAPI void TestUI(HINSTANCE hInst);

#endif // __UI_TEST
