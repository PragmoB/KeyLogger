#pragma once

#include <windef.h>

typedef struct tagMSG32 {
	DWORD   hwnd;
	DWORD   message;
	DWORD wParam;
	DWORD lParam;
	DWORD  time;
	POINT  pt;
} MSG32, *PMSG32, *NPMSG32, *LPMSG32;

typedef struct tagMSG64 {
	ULONGLONG hwnd;
	ULONGLONG message;
	ULONGLONG wParam;
	ULONGLONG lParam;
	DWORD  time;
	POINT  pt;
} MSG64, *PMSG64, *NPMSG64, *LPMSG64;