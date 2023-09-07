#include "Tinky.hpp"


int     main( int argc, char **argv )
{
	if (argc < 2)
	{
		std::cout << "[X] Usage: ./svc [install/start/stop/delete]" << std::endl;
		exit(1);
	}
	try {
		Tinky	__tinkyService(L"tinky");

		std::cout << "Started." << std::endl;
		if (strcmp(argv[1], "install") == 0)
		{
			if (__tinkyService.createService())
				std::cout << "Service {tinky} installed successfully." << std::endl;
			else
				std::cout << "(-) failed to creates a service object." << std::endl;
		}
		else if (strcmp(argv[1], "start") == 0)
		{
			__tinkyService.startService();
			std::cout << "Service {tinky} started successfully.";
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