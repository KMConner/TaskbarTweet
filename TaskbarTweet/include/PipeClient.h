#pragma once

#include <Windows.h>
#include "AuthInfo.h"

BOOL Send(LPCWSTR content, LPCWSTR pipeName, const AuthInfo& authInfo);
