#include "Tinky.hpp"

Tinky::Tinky( const std::string &serviceName ): _tinkyServiceName(serviceName)
{
	/** TO-DO: Establishes a connection to the service control manager
	 ** And opens the specified service control manager database.
	 **/
	_hServiceControlManager =  OpenSCManager(
		NULL,
		NULL,
		SC_MANAGER_ALL_ACCESS
	);

	if (_hServiceControlManager == NULL)
	{
		std::cout << "(-) failed to establishe a connection to the SCM. Error: " << GetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}

	return ;
}

Tinky::Tinky( const Tinky& copy )
{
	*this = copy;
	return ;
}

Tinky::~Tinky( void )
{
	BOOL	__closeHandleStatus;

	__closeHandleStatus = CloseServiceHandle(_hServiceControlManager);

	if (!__closeHandleStatus)
		return;
	return ;
}

std::string Tinky::getServiceName(void) const
{
	return (_tinkyServiceName);
}

bool Tinky::createService(void)
{
	BOOL	__closeHandleStatus;
	char	__ServiceFullPath[260];


	/**
	* Check if a service is already installed/created if not install/create it
	*/

	if (!__openService())
		return (false);

	/**
	* Retrieves the fully qualified path for the file that contains the specified module.
	* The module must have been loaded by the current process.
	*/

	if (!GetCurrentDirectory(260, __ServiceFullPath))
	{
		/** Closes the service object handle **/
		__closeServiceHandle();

		std::cout << "(-) failed to install service {" << _tinkyServiceName << "} handle." << std::endl;
		return false;
	}

	strcat_s(__ServiceFullPath, "\\svc.exe");

	/* Creates a service object and adds it to the specified service control manager database. */
	_hServiceTinky = CreateService(
		_hServiceControlManager,
		_tinkyServiceName.c_str(),
		_tinkyServiceName.c_str(),
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		__ServiceFullPath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if (_hServiceTinky == NULL)
	{
		/** Closes the service object handle **/
		__closeServiceHandle();

		std::cerr << "(-) failed to install service {" << _tinkyServiceName << "} handle." << std::endl;
		return (false);
	}

	/** Closes the service object handle **/
	__closeHandleStatus = __closeServiceHandle();
	if (!__closeHandleStatus)
		return (false);

	return true;
}

bool Tinky::startService(void)
{
	BOOL	__closeHandleStatus;
	BOOL	__startServiceStatus;


	/**
	* Check if a service is already installed/created if not install/create it
	*/
	
	if (!__openService())
		return (false);

	/** Starts a service object. **/
	__startServiceStatus =  StartService(
		_hServiceTinky,
		1,
		NULL
	);

	if (__startServiceStatus == 0)
	{
		/** Closes the service object handle **/
		__closeServiceHandle();

		std::cout << "(-) failed to start service {tinky} object. ERROR: " << GetLastError() << " "  << std::endl;
		return (false);
	}

	/** Closes the service object handle **/
	__closeHandleStatus = __closeServiceHandle();
	if (!__closeHandleStatus)
		return (false);

	return true;
}

bool Tinky::stopService(void)
{
	SERVICE_STATUS_PROCESS	__serviceStatusProcess;
	DWORD					__dwBytesNeeded;
	BOOL					__serviceQueryStatus;
	BOOL					__controlServiceStatus;
	BOOL					__closeHandleStatus;

	// Get a handle to the service.

	if (!__openService())
		return (false);

	// Make sure the service is not already stopped.

	__serviceQueryStatus = QueryServiceStatusEx(
		_hServiceTinky,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&__serviceStatusProcess,
		sizeof(SERVICE_STATUS_PROCESS),
		&__dwBytesNeeded
	);

	if (!__serviceQueryStatus)
	{
		/** Closes the service object handle **/
		__closeServiceHandle();

		std::cout << "(-) service {" << _tinkyServiceName << "} is already stopped." << std::endl;
		return (false);
	}

	if (__serviceStatusProcess.dwCurrentState == SERVICE_STOPPED)
	{
		/** Closes the service object handle **/
		__closeServiceHandle();

		std::cout << "(-) failed stopping service {" << _tinkyServiceName << "} handle." << std::endl;
		return (false);
	}

	// Send a stop code to the service.
	__controlServiceStatus = ControlService(
		_hServiceTinky,
		SERVICE_CONTROL_STOP,
		(LPSERVICE_STATUS)&__serviceStatusProcess
	);

	if (!__controlServiceStatus)
	{
		/** Closes the service object handle **/
		__closeServiceHandle();

		std::cout << "(-) failed stopping service {" << _tinkyServiceName << "} handle." << std::endl;
		return (false);
	}

	/** Closes the service object handle **/
	__closeHandleStatus = __closeServiceHandle();
	if (!__closeHandleStatus)
		return (false);

	return true;
}

bool Tinky::deleteService(void)
{
	return false;
}

BOOL Tinky::__openService(void)
{
	_hServiceTinky = OpenService(
		_hServiceControlManager,
		_tinkyServiceName.c_str(),
		GENERIC_ALL
	);

	if (_hServiceTinky == NULL)
	{
		std::cout << "(-) service {tinky} is not installed." << std::endl;
		return (false);
	}

	return (true);
}

BOOL Tinky::__closeServiceHandle(void)
{
	BOOL	__closeHandleStatus;

	/** Closes the service object handle **/
	__closeHandleStatus = CloseServiceHandle(
		_hServiceTinky
	);
	if (__closeHandleStatus == 0)
	{
		std::cout << "(-) failed to close service {" << _tinkyServiceName << "} handle." << std::endl;
		return (false);
	}

	return (true);
}
