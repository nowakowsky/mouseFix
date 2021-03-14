#include <Windows.h>

class MouseHook {
private:
	MouseHook() {}
public:
	SYSTEMTIME lastCheck;
	static MouseHook& instance() {
		static MouseHook mouseHook;
		return mouseHook;
	}
};