#include "GUI.hpp"



int		main(int argc, char **argv)
{
	std::atomic<bool> isRunning;
	GUI gui;

	isRunning = true;
	while (isRunning)
	{
		gui.events(isRunning);
		gui.update();
		gui.render();
	}

    return 0;
}
