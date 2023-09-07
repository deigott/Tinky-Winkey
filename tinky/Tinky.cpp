#include "Tinky.hpp"

Tinky::Tinky( LPCWSTR serviceName )
{
	/** TO-DO: Establishes a connection to the service control manager
	 ** And opens the specified service control manager database.
	 **/
	_hServiceControlManager =  OpenSCManagerW(
		NULL,
		NULL,
		SC_MANAGER_ALL_ACCESS
	);

	if (_hServiceControlManager == NULL)
	{
		std::cout << "(-) failed to establishe a connection to the SCM. Error: " << GetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}

	// Correctly copy the service name
	_tinkyServiceName = serviceName;
	return ;
}

//Tinky::Tinky( const Tinky& copy )
//{
//	*this = copy;
//	return ;
//}

Tinky::~Tinky( void )
{
	CloseServiceHandle(_hServiceControlManager);
	return ;
}

LPCWSTR Tinky::getServiceName(void) const
{
	return (_tinkyServiceName);
}

bool Tinky::createService(void)
{
	LPWSTR	*__ipFileName = new LPWSTR[1024];
	DWORD	__nSize;

	/**
	* Check if a service is already installed/created if not install/create it
	*/
	_hServiceTinky = OpenServiceW(
		_hServiceControlManager,
		_tinkyServiceName,
		SC_MANAGER_ALL_ACCESS
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
	__nSize = GetModuleFileNameW(
		NULL,
		*__ipFileName,
		1024
	);
	if (__nSize < 1024)
		__ipFileName[__nSize] = '\0';
	

	/* Creates a service object and adds it to the specified service control manager database. */
	_hServiceTinky = CreateServiceW(
		_hServiceControlManager,
		L"tinky",
		L"tinky",
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		*__ipFileName,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);
	if (_hServiceTinky == NULL)
		return (false);
	/** Closes the service object handle **/

	CloseServiceHandle(
		_hServiceTinky
	);
	return true;
}

bool Tinky::startService(void)
{
	/**
	* Check if a service is already installed/created if not install/create it
	*/
	_hServiceTinky = OpenServiceW(
		_hServiceControlManager,
		_tinkyServiceName,
		SC_MANAGER_ALL_ACCESS
	);

	if (_hServiceTinky != NULL)
	{
		/** Closes the service object handle **/
		CloseServiceHandle(_hServiceTinky);

		std::cout << "(-) service {tinky} is already installed." << std::endl;
		exit(EXIT_FAILURE);
	}
	return false;
}

bool Tinky::stopService(void)
{
	return false;
}

bool Tinky::deleteService(void)
{
	return false;
}
