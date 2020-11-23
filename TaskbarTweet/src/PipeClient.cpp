#include <atlstr.h>
#include "nlohmann/json.hpp"
#include "PipeClient.h"

using namespace std;
using namespace nlohmann;

BOOL Send(LPCWSTR content, LPCWSTR pipeName)
{
	HANDLE hPipe = CreateFileW(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	u16string sss;
	auto contentLength = wcslen(content);
	for (size_t i = 0; i < contentLength; i++)
	{
		sss.push_back(content[i]);
	}
	json j = {
		{"text", sss }
	};

	string jsonStr = j.dump(-1, ' ', true) + "\r\n";
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		HRESULT result = HRESULT_FROM_WIN32(err);
		printf("Failed to open the pipe... %d \n", err);
		return FALSE;
	}

	WriteFile(hPipe, jsonStr.c_str(), jsonStr.size(), nullptr, NULL);
	CloseHandle(hPipe);

	return TRUE;
}
