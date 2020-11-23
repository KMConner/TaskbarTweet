#include <atlstr.h>
#include "nlohmann/json.hpp"
#include "PipeClient.h"

using namespace std;
using namespace nlohmann;


void TextToU16String(LPCTSTR text, u16string& str)
{
	auto contentLength = wcslen(text);
	for (size_t i = 0; i < contentLength; i++)
	{
		str.push_back(text[i]);
	}
}

BOOL Send(LPCWSTR content, LPCWSTR pipeName, const AuthInfo& authInfo)
{
	HANDLE hPipe = CreateFileW(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	u16string text, consumerKey, consumerSecret, accessToken, accessTokenSecret;
	TextToU16String(content, text);
	TextToU16String(authInfo.GetConsumerKey(), consumerKey);
	TextToU16String(authInfo.GetConsumerSecret(), consumerSecret);
	TextToU16String(authInfo.GetAccessToken(), accessToken);
	TextToU16String(authInfo.GetAccessTokenSecret(), accessTokenSecret);

	json j = {
		{ "text", text },
		{ "consumerKey", consumerKey },
		{ "consumerSecret", consumerSecret },
		{ "accessToken", accessToken },
		{ "accessTokenSecret", accessTokenSecret},
	};

	string jsonStr = j.dump(-1, ' ', true) + "\r\n";
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		HRESULT result = HRESULT_FROM_WIN32(err);
		printf("Failed to open the pipe... %d \n", err);
		return FALSE;
	}

	if (!WriteFile(hPipe, jsonStr.c_str(), jsonStr.size(), nullptr, NULL))
	{
		CloseHandle(hPipe);
		return FALSE;
	}

	const DWORD respBufSize = 16;
	char resp[respBufSize];
	DWORD length;
	if (!ReadFile(hPipe, resp, respBufSize, &length, NULL))
	{
		CloseHandle(hPipe);
		return FALSE;
	}
	if (length < 1 || resp[0] != '+')
	{
		CloseHandle(hPipe);
		return FALSE;
	}
	CloseHandle(hPipe);

	return TRUE;
}
