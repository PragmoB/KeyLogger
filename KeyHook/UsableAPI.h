#pragma once

WCHAR* ConvertCtoWC(const char* str);
ARCH_UINT FindProcessName(DWORD pid, WCHAR* out);