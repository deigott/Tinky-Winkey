#include "Tinky.hpp"

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	(void)dwArgc;
	(void)lpszArgv;

	while (1);
}

int     main( int argc, char **argv )
{
	if (argc < 2)
	{
		SERVICE_TABLE_ENTRY ServiceStartTable[2];
		ServiceStartTable[0].lpServiceName = "tinkey1";
		ServiceStartTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
		ServiceStartTable[1].lpServiceName = NULL;
		ServiceStartTable[1].lpServiceProc = NULL;
		if (StartServiceCtrlDispatcher(ServiceStartTable))
			return 0;
		else if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
			return -1; // Program not started as a service.
		else
			return -2; // Other error.
	}
	try {
		Tinky	__tinkyService("tinkey1");

		if (strcmp(argv[1], "install") == 0)
		{
			if (__tinkyService.createService())
				std::cout << "Service {tinky} installed successfully." << std::endl;
			else
				std::cout << "(-) failed to creates a service {tinky} object." << std::endl;
		}
		else if (strcmp(argv[1], "start") == 0)
		{
			if (__tinkyService.startService())
				std::cout << "Service {tinky} started successfully.";
			else
				std::cout << "(-) failed to start service {tinky} object." << std::endl;
		}
		else if (strcmp(argv[1], "stop") == 0)
		{
			__tinkyService.stopService();
			std::cout << "Service {tinky} stopped successfully.";
		}
		else if (strcmp(argv[1], "delete") == 0)
		{
			__tinkyService.deleteService();
			std::cout << "Service {tinky} deleted successfully.";
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	return (0);
}