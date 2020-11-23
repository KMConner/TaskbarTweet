#pragma once

#include <Windows.h>

class AuthInfo
{
public:
	AuthInfo();
	~AuthInfo();
	BOOL Load();
	LPCTSTR GetConsumerKey() const;
	LPCTSTR GetConsumerSecret() const;
	LPCTSTR GetAccessToken() const;
	LPCTSTR GetAccessTokenSecret() const;
	BOOL GetIsLoaded() const;

private:
	LPTSTR ConsumerKey;
	LPTSTR ConsumerSecret;
	LPTSTR AccessToken;
	LPTSTR AccessTokenSecret;
	BOOL IsLoaded;
};
