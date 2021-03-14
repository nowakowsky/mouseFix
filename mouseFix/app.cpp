#include <iostream>
#include <Windows.h>

class MouseHook{
private:
	MouseHook() {}
public:
	DOUBLE lastCheck;
	static MouseHook& instance(){
		static MouseHook mouseHook;
		return mouseHook;
	}
	
};


int main() {
	MouseHook mouseHook = MouseHook::instance();
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);
	SystemTimeToVariantTime(&systemTime, &mouseHook.lastCheck);


	while (true) {
		std::cout << "ok";
		break;
	}
	return 0;
};