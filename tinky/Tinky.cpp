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
	CloseServiceHandle(_hServiceControlManager);
	return ;
}

std::string Tinky::getServiceName(void) const
{
	return (_tinkyServiceName);
}

bool Tinky::createService(void)
{
	BOOL	__closeHandleStatus;
	char Path[260];

	if (!GetCurrentDirectory(260, Path))
	{
		printf("Cannot install service (%ld)\n", GetLastError());
		return false;
	}
	strcat_s(Path, "\\svc.exe");

	/**
	* Check if a service is already installed/created if not install/create it
	*/
	_hServiceTinky = OpenService(
		_hServiceControlManager,
		_tinkyServiceName.c_str(),
		GENERIC_ALL
	);

	if (_hServiceTinky != NULL)
	{
		/** Closes the service object handle **/
		CloseServiceHandle(_hServiceTinky);

		std::cout << "(-) service {tinky} is already installed." << std::endl;
		exit(EXIT_FAILURE);
	}

	/**
	* Retrieves the fully qualified path for the file that contains the specified module.
	* The module must have been loaded by the current process.
	*/
	//__nSize = GetModuleFileNameW(
	//	NULL,
	//	*__ipFileName,
	//	1024
	//);
	//if (__nSize < 1024)
	//	__ipFileName[__nSize] = '\0';
	

	/* Creates a service object and adds it to the specified service control manager database. */
	_hServiceTinky = CreateService(
		_hServiceControlManager,
		_tinkyServiceName.c_str(),
		_tinkyServiceName.c_str(),
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		Path,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);
	if (_hServiceTinky == NULL)
		return (false);
	/** Closes the service object handle **/

	__closeHandleStatus = CloseServiceHandle(
		_hServiceTinky
	);
	if (__closeHandleStatus == 0)
	{
		std::cout << "(-) failed to close service {tinky} handle." << std::endl;
		exit(EXIT_FAILURE);
	}
	return true;
}

bool Tinky::startService(void)
{
	//LPWSTR* __ipFileName = new LPWSTR[1024];
	BOOL	__closeHandleStatus;
	BOOL	__startServiceStatus;
	//DWORD	__nSize;


	/**
	* Check if a service is already installed/created if not install/create it
	*/
	_hServiceTinky = OpenService(
		_hServiceControlManager,
		_tinkyServiceName.c_str(),
		GENERIC_ALL
	);

	if (_hServiceTinky == NULL)
	{
		std::cout << "(-) service {tinky} is not installed." << std::endl;
		exit(EXIT_FAILURE);
	}

	/*__nSize = GetFullPathNameW(
		L".\\hello.exe",
		1024,
		*__ipFileName,
		NULL
	);
	std::cout << __ipFileName << std::endl;*/
	// Path to the executable you want to run as the service
	std::string exePath = "C:\\Users\\Devicekiller402\\Projects\\tinky-winkey\\tinky\\hello.exe"; // Update this with the correct path

	
	char Path[260];

	if (!GetCurrentDirectory(260, Path))
	{
		printf("Cannot install service (%ld)\n", GetLastError());
		return false;
	}
	strcat_s(Path, "\\svc.exe");

	// Prepare an array of null-terminated strings
	LPCSTR serviceArgs[] = { Path, nullptr };
	/** Starts a service object. **/
	__startServiceStatus =  StartService(
		_hServiceTinky,
		1,
		serviceArgs
	);

	if (__startServiceStatus == 0)
	{
		std::cout << "(-) failed to start service {tinky} object. ERROR: " << GetLastError() << " "  << std::endl;
		exit(EXIT_FAILURE);
	}

	/** Closes the service object handle **/
	__closeHandleStatus = CloseServiceHandle(
		_hServiceTinky
	);
	if (__closeHandleStatus == 0)
	{
		std::cout << "(-) failed to close service {tinky} handle." << std::endl;
		exit(EXIT_FAILURE);
	}
	//delete __ipFileName;

	return true;
}

bool Tinky::stopService(void)
{
	return false;
}

bool Tinky::deleteService(void)
{
	return false;
}
