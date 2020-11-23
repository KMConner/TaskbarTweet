#include <ShlObj.h>
#include <Shlwapi.h>
#include "AuthInfo.h"

AuthInfo::AuthInfo()
{
	ConsumerKey = new TCHAR[256];
	ConsumerSecret = new TCHAR[256];
	AccessToken = new TCHAR[256];
	AccessTokenSecret = new TCHAR[256];
	IsLoaded = FALSE;
}

AuthInfo::~AuthInfo()
{
	delete[] ConsumerKey;
	delete[] ConsumerSecret;
	delete[] AccessToken;
	delete[] AccessTokenSecret;
}

BOOL AuthInfo::Load()
{
	TCHAR lpIniPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, lpIniPath, CSIDL_LOCAL_APPDATA, FALSE);
	if (!PathAppend(lpIniPath, TEXT("TaskBarTweet\\auth.ini")))
	{
		return FALSE;
	}

	if (GetPrivateProfileString(TEXT("account"), TEXT("ConsumerKey"), NULL, ConsumerKey, 256, lpIniPath) == 0)
	{
		return FALSE;
	}
	if (GetPrivateProfileString(TEXT("account"), TEXT("ConsumerSecret"), NULL, ConsumerSecret, 256, lpIniPath) == 0)
	{
		return FALSE;
	}
	if (GetPrivateProfileString(TEXT("account"), TEXT("AccessToken"), NULL, AccessToken, 256, lpIniPath) == 0)
	{
		return FALSE;
	}
	if (GetPrivateProfileString(TEXT("account"), TEXT("AccessTokenSecret"), NULL, AccessTokenSecret, 256, lpIniPath) == 0)
	{
		return FALSE;
	}
	IsLoaded = TRUE;

	return TRUE;
}

LPCTSTR AuthInfo::GetConsumerKey() const
{
	return ConsumerKey;
}

LPCTSTR AuthInfo::GetConsumerSecret() const
{
	return ConsumerSecret;
}

LPCTSTR AuthInfo::GetAccessToken() const
{
	return AccessToken;
}

LPCTSTR AuthInfo::GetAccessTokenSecret() const
{
	return AccessTokenSecret;
}

BOOL AuthInfo::GetIsLoaded() const
{
	return IsLoaded;
}
