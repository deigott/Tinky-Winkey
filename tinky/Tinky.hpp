#ifndef TINKEY_HPP
# define TINKEY_HPP

# include <windows.h>
# include <winsvc.h>
# include <iostream>
# include <string>


class Tinky {
public:
	Tinky( LPCWSTR serviceName );
	//Tinky( const Tinky &copy );
	~Tinky( void );

	// Specify Getters section
	LPCWSTR		getServiceName(void) const;

	// TO-DO: Here we handle creation/starting/stoping of our tinky service
	bool			createService(void);
	bool			startService(void);
	bool			stopService(void);
	bool			deleteService(void);
private:
	Tinky(void);

	LPCWSTR		_tinkyServiceName;
	SC_HANDLE	_hServiceControlManager;
	SC_HANDLE	_hServiceTinky;
};

#endif
