# include <iostream>
# include <string>

int     main( int argc, char **argv )
{
	if (argc < 2)
	{
		std::cout << "[X] Usage: ./svc [install/start/stop/delete]" << std::endl;
		exit(1);
	}
	else
	{
		if (strcmp(argv[1], "install") == 0)
		{
			std::cout << "Service {tinky} installed successfully.";
		}
		else if (strcmp(argv[1], "start") == 0)
		{
			std::cout << "Service {tinky} started successfully.";
		}
		else if (strcmp(argv[1], "stop") == 0)
		{
			std::cout << "Service {tinky} stopped successfully.";
		}
		else if (strcmp(argv[1], "delete") == 0)
		{
			std::cout << "Service {tinky} deleted successfully.";
		}
	}
	return (0);
}