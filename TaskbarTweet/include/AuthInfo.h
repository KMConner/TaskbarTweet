#pragma once

#include <Windows.h>

class AuthInfo
{
public:
	AuthInfo();
	~AuthInfo();
	BOOL Load();
	LPCTSTR GetConsumerKey();
	LPCTSTR GetConsumerSecret();
	LPCTSTR GetAccessToken();
	LPCTSTR GetAccessTokenSecret();
	BOOL GetIsLoaded();

private:
	LPTSTR ConsumerKey;
	LPTSTR ConsumerSecret;
	LPTSTR AccessToken;
	LPTSTR AccessTokenSecret;
	BOOL IsLoaded;
};
