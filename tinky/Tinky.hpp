#ifndef TINKEY_HPP
# define TINKEY_HPP

# include <windows.h>
# include <winsvc.h>
# include <iostream>
# include <string>
# include <tchar.h>
# include <strsafe.h>
# include <TlHelp32.h>


# define SVCNAME TEXT("tinklogger")
# define WINKEY_PATH TEXT("C:\\Users\\Devicekiller402\\Projects\\tinky-winkey\\winkey.exe")


#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")

typedef struct	__SERVICE_STATUS
{
	SERVICE_STATUS_HANDLE	__hServiceStatusHandle;
	SERVICE_STATUS			__ServiceStatus;
	HANDLE                  __serviceStopEvent;
	HANDLE					__winkeyPrimaryToken;
}				SERVICE_STATUS_GLB;

class Tinky {
public:
	Tinky( const std::string &serviceName );
	Tinky( const Tinky &copy );
	~Tinky( void );

	// Specify Getters section
	std::string		getServiceName(void) const;

	// TO-DO: Here we handle creation/starting/stoping of our tinky service
	bool			createService(void);
	bool			startService(void);
	bool			stopService(void);
	bool			deleteService(void);
private:
	Tinky(void);

	BOOL	__openService(void);
	BOOL	__closeServiceHandle(void);


	std::string	_tinkyServiceName;
	SC_HANDLE	_hServiceControlManager;
	SC_HANDLE	_hServiceTinky;
};

#endif
