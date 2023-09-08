#include "Tinky.hpp"

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	(void)dwArgc;
	(void)lpszArgv;
	STARTUPINFOW si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	CreateProcessW(
		L"C:\\Windows\\System32\\notepad.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		BELOW_NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&si,
		&pi
	);
}

// Logs messages to the event log
//VOID SvcReportEvent(LPTSTR szFunction)
//{
//	HANDLE hEventSource;
//	LPCTSTR lpszStrings[2];
//	TCHAR Buffer[80];
//
//	hEventSource = RegisterEventSource(NULL, SVCNAME);
//
//	if (NULL != hEventSource)
//	{
//		StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());
//
//		lpszStrings[0] = SVCNAME;
//		lpszStrings[1] = Buffer;
//
//		ReportEvent(hEventSource,        // event log handle
//			EVENTLOG_ERROR_TYPE, // event type
//			0,                   // event category
//			SVC_ERROR,           // event identifier
//			NULL,                // no security identifier
//			2,                   // size of lpszStrings array
//			0,                   // no binary data
//			lpszStrings,         // array of strings
//			NULL);               // no binary data
//
//		DeregisterEventSource(hEventSource);
//	}
//}

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

		const char* commands[] = { "install", "start", "stop", "delete" };

		// Define an array of function pointers
		bool (Tinky:: * serviceFunctions[])() = {
			&(Tinky::createService),
			&(Tinky::startService),
			&(Tinky::stopService),
			&(Tinky::deleteService)
		};

		for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
			if (strcmp(argv[1], commands[i]) == 0) {
				if ((__tinkyService.*serviceFunctions[i])()) {
					std::cout << "Service {tinky} " << argv[1] << "ed successfully." << std::endl;
				}
			}
		}
	}
	return (0);
}