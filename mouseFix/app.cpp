#include <iostream>
#include <Windows.h>
#include "MouseHook.h"

// cooldown in milliseconds
#define COOLDOWN 200

HHOOK hMouseHook;
SYSTEMTIME systemTime;
MouseHook mouseHook = MouseHook::instance();

FILETIME addTime(FILETIME ft) {
	ULARGE_INTEGER tmp;
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	// convert cooldownTime to milliseconds
	tmp.QuadPart += (COOLDOWN * 10000);
	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	return ft;
}

bool shouldBlock(SYSTEMTIME lastTime, SYSTEMTIME currentTime) {
	// conver SYSTEMTIME TO FILETIME
	FILETIME flastTime;
	FILETIME fcurrentTime;
	SystemTimeToFileTime(&lastTime, &flastTime);
	SystemTimeToFileTime(&currentTime, &fcurrentTime);
	
	// add cooldown time
	flastTime = addTime(flastTime);

	auto test = CompareFileTime(&flastTime, &fcurrentTime);
	if (test == -1 || test == 0) return false;
	return true;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
	if (wParam == WM_LBUTTONDOWN) {
		GetSystemTime(&systemTime);

		// block
		if (shouldBlock(mouseHook.lastCheck, systemTime)) return CallNextHookEx(hMouseHook, -1, wParam, lParam);
		
		// allow and update lastCheck
		std::cout << "Allowed click" << std::endl;

		mouseHook.lastCheck = systemTime;
		return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
	}
	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
};

int main() {
	MSG msg;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	GetSystemTime(&systemTime);
	mouseHook.lastCheck = systemTime;

	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInstance, NULL);
	while (true) {
		GetMessage(&msg, 0, 0, 0);
		DispatchMessage(&msg);
	}
	return 0;
};